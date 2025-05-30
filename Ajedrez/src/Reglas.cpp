#include <iostream>
#include <sstream>
#include <string>
#include "freeglut.h"
#include "Reglas.h"
#include "Tablero.h"
#include "Pieza.h"
#include "ETSIDI.h"

using namespace std;

Vector2D ClassReglas::devolver_forma(const int& Variante) {
	switch (Variante)
	{
	case 1:
		return ShapeSilverman;
		break;
	case 2:
		return ShapeDemi;
		break;
	default:
		return{ 0,0 };
		break;
	}
}

void ClassReglas::inicia_temporizador(int segundos) {
	tiempo_restante_rojas = segundos;
	tiempo_restante_azules = segundos;
}

void ClassReglas::actualiza_tiempo() {

	if (turno_ == 1) {
		if (tiempo_restante_azules > 0)
			tiempo_restante_azules--;
	}
	else if (turno_ == 0) {
		if (tiempo_restante_rojas > 0)
			tiempo_restante_rojas--;
	}

}

void ClassReglas::set_turno() {//cambio de turno
	turno_ = !turno_;
}

ClassPieza::Color ClassReglas::getColorTurno() const {
    return turno_ ? ClassPieza::Color::AZUL : ClassPieza::Color::ROJO;
}

int ClassReglas::get_tiempo_restante_azules() const {
		return tiempo_restante_azules;
}

int ClassReglas::get_tiempo_restante_rojas() const {
	return tiempo_restante_rojas;
}

bool ClassReglas::get_turno() const {
	return turno_;
}

Vector2D ClassReglas::buscarRey(const ClassTablero& tablero, ClassPieza::Color colorRey) {
    // Buscar la posición del rey del color dado
    for (int i = 0; i < tablero.getFilas(); ++i) {
        for (int j = 0; j < tablero.getColumnas(); ++j) {
            ClassPieza* pieza = tablero.getPieza({ i, j });
            if (pieza && pieza->getTipo() == ClassPieza::Pieza_t::Rey && pieza->getColor() == colorRey) {
                return { i, j };
            }
        }
    }
    return { -1, -1 };
}
bool ClassReglas::hayJaque(const ClassTablero& tablero, ClassPieza::Color colorRey) {
  
    Vector2D posRey = buscarRey(tablero, colorRey); //buscamos rey

    if (posRey.x == -1) return false; // Rey no encontrado

    // Verificar si alguna pieza enemiga puede atacar al rey
    for (int i = 0; i < tablero.getFilas(); ++i) {
        for (int j = 0; j < tablero.getColumnas(); ++j) {
            ClassPieza* pieza = tablero.getPieza({ i, j });
            if (pieza && pieza->getColor() != colorRey) {
                auto movimientos = pieza->obtenerMovimientosPosibles(tablero);
                for (const auto& mov : movimientos) {
                    if (mov == posRey) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool ClassReglas::hayJaqueMate(const ClassTablero& tablero, ClassPieza::Color colorRey) {
  
    bool jaqueMate = false;
  
    Vector2D posRey = buscarRey(tablero, colorRey);
    int fila = posRey.x;
    int columna = posRey.y;

    ClassPieza* Rey = tablero.getPieza({ fila, columna });
    vector<Vector2D> movsRey = Rey->obtenerMovimientosPosibles(tablero);
    int Nmovs = movsRey.size();

    if (hayJaque(tablero,colorRey) == true && Nmovs <= 0) {
        cout << "JAQUE MATE!! GANAN" << (colorRey == ClassPieza::Color::AZUL ? "ROJO" : "AZUL") << "!" << endl;
        ETSIDI::play("sonidos/victoria.wav");
        return true;
    }
    else
        return false;
}
std::optional<std::pair<Vector2D, ClassPieza::Color>> ClassReglas::getReyEnJaque(const ClassTablero& tablero) {
    if (hayJaque(tablero, ClassPieza::Color::AZUL)) {
        return std::make_pair(buscarRey(tablero, ClassPieza::Color::AZUL), ClassPieza::Color::AZUL);
    }
    if (hayJaque(tablero, ClassPieza::Color::ROJO)) {
        return std::make_pair(buscarRey(tablero, ClassPieza::Color::ROJO), ClassPieza::Color::ROJO);
    }
    return std::nullopt;
}


bool ClassReglas::PosAmenzada( Vector2D pos, const ClassTablero& tablero, ClassPieza* Pieza) {
    vector<Vector2D> movimientos;
    for (int i = 0; i < tablero.getFilas(); ++i) {
        for (int j = 0; j < tablero.getColumnas(); ++j) {
            ClassPieza* pieza2 = tablero.getPieza({ i, j });
            if (pieza2 && pieza2->getColor() != Pieza->getColor() && pieza2->getTipo() != Pieza->getTipo()&& pieza2->getTipo()!=ClassPieza::Peon) { // evitamos el rey y el peon
                 movimientos = pieza2->obtenerMovimientosPosibles(tablero);
             
            }
            else if (pieza2 && pieza2->getTipo() == Pieza->getTipo() && pieza2->getColor() != Pieza->getColor()) {
            movimientos = { {i+ 1, j-1}, {i+1, j+0}, {i+1, j+1}, {i+0, j-1}, {i+0, j+1}, {i-1,j-1}, {i-1,j+ 0}, {i-1, j+1} }; // el rey tambien amenaza pero no podemos llamar a sus movposibles
            }
            else if (pieza2 && pieza2->getTipo() == ClassPieza::Peon && pieza2->getColor() != Pieza->getColor()) {
                if(pieza2->getColor()==ClassPieza::Color::ROJO)
                movimientos = { {i +1, j -1}, {i + 1, j + 1} }; //el peon solo come en diagonal sus movsPosibles son tods no solo los de comer
                else if (pieza2->getColor() == ClassPieza::Color::AZUL)
                    movimientos = { {i - 1, j - 1}, {i - 1, j + 1} }; 

            }
            for (const auto& mov : movimientos) {
                if (tablero.estaDentro(mov) && mov == pos) {

                    return true;
                }
            }
        }
    }
    return false;
}

bool ClassReglas::hayReyAhogado(const ClassTablero& tablero, ClassPieza::Color colorRey, int turno) {

    bool hayjaque = hayJaque(tablero, colorRey);
    Vector2D posRey = buscarRey(tablero, colorRey);
    int fila = posRey.x;
    int columna = posRey.y;

    vector<Vector2D> movimientos;
    int Nmovs = 0;

    bool esturno = false; //el rey ahogado tiene que ser una vez haya movido el oponente

    if (colorRey == ClassPieza::Color::AZUL && turno == 0)
        esturno = true;
    if (colorRey == ClassPieza::Color::ROJO && turno == 1)
        esturno = true;

    for (int i = 0; i < tablero.getFilas(); ++i) {
        for (int j = 0; j < tablero.getColumnas(); ++j) {
            ClassPieza* pieza = tablero.getPieza({ i, j });
            if (pieza && pieza->getColor() == colorRey) {
                auto movimientos = pieza->obtenerMovimientosPosibles(tablero);
                Nmovs += movimientos.size();
            }
        }
    }
    if (!hayjaque && Nmovs <= 0 && esturno) {
        cout << "Rey " << (colorRey == ClassPieza::Color::AZUL ? "AZUL" : "ROJO") << " ahogado. Tablas!" << endl;
        return true;
    }
    else
        return false;
}



string ClassReglas::tiempo_string_rojas() {
    ostringstream os;//en vez de un flujo como ostream, para almacenar el string
    os << tiempo_restante_rojas;

    return os.str();
}
string ClassReglas::tiempo_string_azules() {
    ostringstream os;//en vez de un flujo como ostream, para almacenar el string
    os << tiempo_restante_azules;

    return os.str();
}

bool ClassReglas::get_Promocion(const ClassPieza& pieza, int var) {

    Vector2D pos=pieza.getPos();//en que posicion esta
    ClassPieza::Color col=pieza.getColor(); //que color de peon es
    ClassPieza::Pieza_t tipo = pieza.getTipo();

    int promo_rojas;//fila promocion rojas
    int promo_azules;//fila promocion azules
    if (var == 1) {
        promo_rojas = 4;
        promo_azules = 0;
    }
    else if (var == 2) {
        promo_rojas = 7;
        promo_azules = 0;
    }

    if (tipo == ClassPieza::Pieza_t::Peon) {
        if (col == ClassPieza::Color::AZUL && pos.x == promo_azules) //solo me interesa en que fila esta
            return 1;
        else if (col == ClassPieza::Color::ROJO && pos.x == promo_rojas)
            return 1;
        else
            return 0;
    }

    else
        return 0;

}

bool ClassReglas::empateReyes(const ClassTablero& tablero) const {
	int piezasAzules = 0; //que no sean reyes
	int piezasRojas = 0;
	for (int i = 0; i < tablero.getFilas(); ++i) {
		for (int j = 0; j < tablero.getColumnas(); ++j) {
			ClassPieza* pieza = tablero.getPieza({ i, j });
			if (pieza && pieza->getTipo() != ClassPieza::Pieza_t::Rey) {
				if (pieza->getColor() == ClassPieza::Color::AZUL) {
					piezasAzules++;
				}
				else if (pieza->getColor() == ClassPieza::Color::ROJO) {
					piezasRojas++;
				}
			}
		}
	}
    if (piezasAzules == 0 && piezasRojas == 0) {
		cout << "Empate por solo dos reyes!" << endl;
		return true;
	}
	else
		return false;
}