#include <iostream>
#include <cassert>
#include "Mundo.h"
#include "freeglut.h"
#include "Reglas.h"

using namespace std;

ClassReglas reglas; 
static ClassMundo* mundoPtr = nullptr;


int ClassMundo::PreguntarVariante() {
	int var=1; //al fin y al cabo el enum es de enteros
	std::cout << "Selecciona la variante de ajedrez:\n";
	std::cout << "1. Silverman\n";
	std::cout << "2. Demichess\n";
	std::cin >> var; //sera 1 o 2, hasta que tengamos la interfaz

	return var;
}

void ClassMundo::tecla(unsigned char key) {

}
void ClassMundo::tecla_especial(unsigned char key) {

}

void ClassMundo::inicializa() {

	mundoPtr = this; 

	// Inicializa el tablero según la variante seleccionada
	int var = PreguntarVariante();

	var_ = static_cast<Variante>(var);

	auto dimensiones = reglas.devolver_forma(var);

	ObjTablero = new ClassTablero(dimensiones);

	//de quien es el turno?
	if (reglas.get_turno()) {
		std::cout << "Turno de las azules" << std::endl;
	}
	else {
		std::cout << "Turno de las rojas" << std::endl;
	}


	// Inicializa la posicion de la camara, segun el tablero
	x_ojo = ObjTablero->getFilas() / 2;
	y_ojo = ObjTablero->getColumnas() / 2;
	z_ojo = ObjTablero->getFilas() * 2.0;
	// Se llama al tablero para que inicialice y coloque las piezas en la matriz del tablero
	ObjTablero->ColocarPiezas();
	// Imprimimos el tablero por pantalla
	ObjTablero->ImprimirEnPantalla();

	//runAllTests();

	//temporizador
	reglas.inicia_temporizador(100000); // Por ejemplo, 8 segundos
	glutTimerFunc(1000, ClassMundo::onTimer, 0); // Arranca el temporizador


	//cambio de turno
	//reglas.set_turno();
}

void ClassMundo::onTimer(int value) {
	bool turno = reglas.get_turno();
	if (mundoPtr) {
		mundoPtr->temporizador();
		if (turno == 0) {
			if (reglas.get_tiempo_restante_rojas() > 0) {
				glutTimerFunc(1000, ClassMundo::onTimer, 0);
			}
		}
		if (turno == 1) {
			if (reglas.get_tiempo_restante_azules() > 0) {
				glutTimerFunc(1000, ClassMundo::onTimer, 0);
			}
		}
	}
	glutPostRedisplay();
}

void ClassMundo::temporizador() {
	reglas.actualiza_tiempo();
}

void ClassMundo::rotarOjo() {
	double dist = sqrt(x_ojo * x_ojo + z_ojo * z_ojo);
	double ang = atan2(z_ojo, x_ojo);
	ang += 0.01;
	x_ojo = dist * cos(ang);
	z_ojo = dist * sin(ang);
}


void ClassMundo::mueve() {
	// Se llama al tablero para que animar las piezas
	ObjTablero->AnimaPiezas();
}

void ClassMundo::dibuja() {
	gluLookAt(x_ojo, y_ojo, z_ojo,
		ObjTablero->getFilas() / 2, ObjTablero->getColumnas() / 2, 0, //NOTESE QUE HEMOS CAMBIADO ESTO
		0.0, 1.0, 0.0); //PARA MIRAR AL CENTRO DE LA ESCENA

	glPushMatrix();
	// Se dibuja el tablero
	ObjTablero->dibuja();
	glPopMatrix();


	//creo los string para mostrar en pantalla
	//recurro a la funcion de reglas en la que paso el tiempo a string
	string textoTiempo;
	if(reglas.get_turno() == 0)
		textoTiempo = "Tiempo rojas: " + reglas.tiempo_string();
	else if(reglas.get_turno()==1)
		textoTiempo = "Tiempo azules: " + reglas.tiempo_string();

	imprime_tiempo(textoTiempo.c_str(), 5, 4);//c_str para de string a char*
	
}

int ClassMundo::getFilas() const {
	return ObjTablero ? ObjTablero->getFilas() : 0;
}

int ClassMundo::getColumnas() const {
	return ObjTablero ? ObjTablero->getColumnas() : 0;

}

void ClassMundo::mueve_pieza(const Vector2D& clicada) {

	//cout << "Clic en la casilla (" << clicada.x << ", " << clicada.y << ")\n";

	if (clicada.x < 0 || clicada.y < 0 || !ObjTablero->estaDentro(clicada)) {
		//std::cout << "Click invalido o fuera del tablero\n";
		return;
	}

	//TESTTTSS
	ClassPieza* tpieza = ObjTablero->getPieza(clicada);
	if (tpieza) {
		std::string tipoTexto;
		switch (tpieza->getTipo()) {
		case ClassPieza::Pieza_t::Peon:    tipoTexto = "peon"; break;
		case ClassPieza::Pieza_t::Torre:   tipoTexto = "torre"; break;
		case ClassPieza::Pieza_t::Alfil:   tipoTexto = "alfil"; break;
		case ClassPieza::Pieza_t::Reina:   tipoTexto = "reina"; break;
		case ClassPieza::Pieza_t::Rey:     tipoTexto = "rey"; break;
		case ClassPieza::Pieza_t::Caballo: tipoTexto = "caballo"; break;
		default: tipoTexto = "pieza desconocida"; break;
		}
		std::string colorTexto = (tpieza->getColor() == ClassPieza::Color::AZUL) ? "azul" : "rojo";
		std::cout << "Estas clicando un " << tipoTexto << " " << colorTexto << "\n";
	}
	else {
		std::cout << "Casilla vacia (" << clicada.x << ", " << clicada.y << ")\n";
	}
	//////

	if (!haySeleccionActiva) {
		if (ObjTablero->estaOcupada(clicada)) {
			ClassPieza* p = ObjTablero->getPieza(clicada);
			if (p && ((reglas.get_turno() && p->getColor() == ClassPieza::Color::AZUL) || (!reglas.get_turno() && p->getColor() == ClassPieza::Color::ROJO))) {
				casillaSeleccionada = clicada;
				haySeleccionActiva = true;
			}

		}
	}
	else {
		ClassPieza* pieza = ObjTablero->getPieza(casillaSeleccionada);
		if (pieza) {
			auto movimientos = pieza->obtenerMovimientosPosibles(*ObjTablero);
			bool valido = false;
			for (const auto& m : movimientos) {
				if (m == clicada) {
					valido = true;
					break;
				}
			}
			if (valido) {
				ObjTablero->moverPieza(casillaSeleccionada, clicada);
				// Verificar jaque después del movimiento
				if (reglas.hayJaque(*ObjTablero, ClassPieza::Color::AZUL)) {
					std::cout << "¡El rey azul está en jaque!" << std::endl;
				}
				if (reglas.hayJaque(*ObjTablero, ClassPieza::Color::ROJO)) {
					std::cout << "¡El rey rojo está en jaque!" << std::endl;
				}
				reglas.set_turno(); // Cambia el turno después de mover
			}
		}
		haySeleccionActiva = false;
	}
}


void ClassMundo::imprime_tiempo(const char* text, float x, float y) {
	bool turn = reglas.get_turno();
	if(turn==0)
		ETSIDI::setTextColor(1, 0, 0);
	else if(turn==1)
		ETSIDI::setTextColor(0, 0, 1);
	ETSIDI::setFont("fuentes/Bitwise.ttf", 16);
	ETSIDI::printxy(text, x, y);

}