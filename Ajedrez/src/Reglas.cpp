#include <iostream>
#include <sstream>
#include <string>
#include "freeglut.h"
#include "Reglas.h"
#include "Tablero.h"
#include "Pieza.h"

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

int ClassReglas::get_tiempo_restante_azules() const {
	return tiempo_restante_azules;
}

int ClassReglas::get_tiempo_restante_rojas() const {
	//cout << "quedan para las rojas" << tiempo_restante_rojas << "s"<<endl;
	return tiempo_restante_rojas;
}
bool ClassReglas::get_turno() const {
	return turno_;
}

bool ClassReglas::hayJaque(const ClassTablero& tablero, ClassPieza::Color colorRey) {
    Vector2D posRey{ -1, -1 };

    // Buscar la posición del rey del color dado
    for (int i = 0; i < tablero.getFilas(); ++i) {
        for (int j = 0; j < tablero.getColumnas(); ++j) {
            ClassPieza* pieza = tablero.getPieza({ i, j });
            if (pieza && pieza->getTipo() == ClassPieza::Pieza_t::Rey && pieza->getColor() == colorRey) {
                posRey = { i, j };
                break;
            }
        }
    }

    if (posRey.x == -1) return false; // Rey no encontrado

    // Verificar si alguna pieza enemiga puede atacar al rey
    for (int i = 0; i < tablero.getFilas(); ++i) {
        for (int j = 0; j < tablero.getColumnas(); ++j) {
            ClassPieza* pieza = tablero.getPieza({ i, j });
            if (pieza && pieza->getColor() != colorRey) {
                auto movimientos = pieza->obtenerMovimientosPosibles(tablero);
                for (const auto& mov : movimientos) {
                    if (mov == posRey) {
                        std::cout << "¡Jaque al rey " << (colorRey == ClassPieza::Color::AZUL ? "azul" : "rojo") << "!" << std::endl;
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool ClassReglas::PosAmenzada( Vector2D pos, const ClassTablero& tablero, ClassPieza* Pieza) {
    for (int i = 0; i < tablero.getFilas(); ++i) {
        for (int j = 0; j < tablero.getColumnas(); ++j) {
            ClassPieza* pieza2 = tablero.getPieza({ i, j });
            if (pieza2 && pieza2->getColor() != Pieza->getColor() && pieza2->getTipo() != Pieza->getTipo() ) { //Llama a movs posible menos de la pieza que ele pasamos para evitar un bucle infinito
                auto movimientos = pieza2->obtenerMovimientosPosibles(tablero);
                for (const auto& mov : movimientos) {
                    if (mov == pos) {
                        return true;
                        std::cout << "Posicion Amenazada: " << pos<< endl;
                    }
                }
            }
        }
    }
    return false;
}


string ClassReglas::tiempo_string() {
    ostringstream os;//en vez de un flujo como ostream, para almacenar el string
    if(turno_==0)
        os << tiempo_restante_rojas;
    else if(turno_==1)
        os << tiempo_restante_azules;

    return os.str();
}
bool ClassReglas::get_Promocion(const ClassPieza& pieza) {

    //primero el arreglo variante
    //Vector2D pos;
    //ClassPieza::Color col;
    //pos=pieza.getPos();
    //col = pieza.getColor();
    ////para las rojas
    //if (col == ClassPieza::Color::ROJO) {
    //    
    //}
    return 0;
}