#include <iostream>
#include <cassert>
#include "Mundo.h"
#include "freeglut.h"
#include "Reglas.h"
#include "ETSIDI.h"
#include "IA.h"

using namespace std;

ClassReglas reglas; 
static ClassMundo* mundoPtr = nullptr;

IA miIA(IA::elegirEstrategiaAleatoria()); 
// Para probar una en concreto, puedes usar:
// IA miIA(IA::TipoIA::Agresiva); // O cualquier otro tipo de IA
// IA miIA(IA::TipoIA::Defensiva); // O cualquier otro tipo de IA
// IA miIA(IA::TipoIA::Tactica); // O cualquier otro tipo de IA
// IA miIA(IA::TipoIA::Aleatoria); // O cualquier otro tipo de IA
// IA miIA(IA::TipoIA::Adaptativa); // O cualquier otro tipo de IA





int ClassMundo::PreguntarVariante() {
	int var = 1; //al fin y al cabo el enum es de enteros
	std::cout << "Selecciona la variante de ajedrez:\n";
	std::cout << "1. Silverman\n";	
	std::cout << "2. Demichess\n";
	std::cin >> var; //sera 1 o 2, hasta que tengamos la interfaz

	return var;
}

void ClassMundo::tecla(unsigned char key) {
	if (key == 'r') {
		reset();

	}
}
void ClassMundo::tecla_especial(unsigned char key) {

}

void ClassMundo::parpadeoExclamacion(int value) {
	if (mundoPtr) {
		mundoPtr->visibleExclamacion = !mundoPtr->visibleExclamacion;
		glutPostRedisplay();
		glutTimerFunc(300, ClassMundo::parpadeoExclamacion, 0);
	}
}



void ClassMundo::inicializa() {

	mundoPtr = this; 

	// Inicializa el tablero según la variante seleccionada

	auto dimensiones = reglas.devolver_forma(var_);

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
	glutTimerFunc(500, ClassMundo::parpadeoExclamacion, 0);

}

void ClassMundo::onTimer(int value) {
	bool turno = reglas.get_turno();
	if (mundoPtr) {
		mundoPtr->temporizador();


		if (!turno) { // Turno de las rojas
			miIA.ejecutarMovimiento(mundoPtr->ObjTablero, reglas);
		}

		if ((turno && reglas.get_tiempo_restante_azules() > 0) ||
			(!turno && reglas.get_tiempo_restante_rojas() > 0)) {
			glutTimerFunc(1000, ClassMundo::onTimer, 0);
		}

        /*bool turno = reglas.get_turno();
		if (!turno) { // Turno de las rojas
			iaRoja.ejecutarMovimiento(mundoPtr->ObjTablero, mundoPtr->reglas);
		}

		if ((turno && mundoPtr->reglas.get_tiempo_restante_azules() > 0) ||
			(!turno && mundoPtr->reglas.get_tiempo_restante_rojas() > 0)) {
			glutTimerFunc(1000, ClassMundo::onTimer, 0);
		}*/

		/*if (turno == 0) {
			if (reglas.get_tiempo_restante_rojas() > 0) {
				glutTimerFunc(1000, ClassMundo::onTimer, 0);
			}
		}
		if (turno == 1) {
			if (reglas.get_tiempo_restante_azules() > 0) {
				glutTimerFunc(1000, ClassMundo::onTimer, 0);
			}
		}*/
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
	tiempoRebote += 0.05f;

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

	imprime_tiempo(textoTiempo.c_str(), 5, 5);//c_str para de string a char*


	//imprime promocion
	if (hay_promo == true)
		mensajePromo();

	if (visibleExclamacion) {

		static bool estabaEnJaque = false;

		auto reyJaque = reglas.getReyEnJaque(*ObjTablero);
		bool hayJaqueAhora = reyJaque.has_value();

		if (hayJaqueAhora) {
			ObjTablero->dibujarExclamacionSobreRey(reyJaque->first, reyJaque->second, tiempoRebote);

			if (!estabaEnJaque) {
				ETSIDI::play("sonidos/alerta.wav");
			}
		}

		estabaEnJaque = hayJaqueAhora;
	}
}

int ClassMundo::getFilas() const {
	return ObjTablero ? ObjTablero->getFilas() : 0;
}

int ClassMundo::getColumnas() const {
	return ObjTablero ? ObjTablero->getColumnas() : 0;

}

void ClassMundo::actualizaTurno() {
	reglas.set_turno();

	std::string textoTurno = reglas.get_turno() ? "AZUL" : "ROJO";
	std::cout << "Turno del jugador: " << textoTurno << std::endl;
}

bool ClassMundo::verificaEstadoDelJuego() {
	if (!ObjTablero) return false;

	// Verificar jaque y mate para ambos colores
	hayJaqueAzul = reglas.hayJaque(*ObjTablero, ClassPieza::Color::AZUL);
	hayJaqueRojo = reglas.hayJaque(*ObjTablero, ClassPieza::Color::ROJO);

	hayJaqueMateAzul = reglas.hayJaqueMate(*ObjTablero, ClassPieza::Color::AZUL);
	hayJaqueMateRojo = reglas.hayJaqueMate(*ObjTablero, ClassPieza::Color::ROJO);

	hayReyAhogadoAzul = reglas.hayReyAhogado(*ObjTablero, ClassPieza::Color::AZUL, reglas.turno_);
	hayReyAhogadoRojo = reglas.hayReyAhogado(*ObjTablero, ClassPieza::Color::ROJO, reglas.turno_);

	if (hayJaqueMateAzul) std::cout << "Mate al azul\n";
	if (hayJaqueMateRojo) std::cout << "Mate al rojo\n";

	return hayJaqueMateAzul || hayJaqueMateRojo || hayReyAhogadoAzul || hayReyAhogadoRojo;
}


void ClassMundo::procesaMovimiento(const Vector2D& origen, const Vector2D& destino) {

	ClassPieza::Color turnoActual = reglas.get_turno() ? ClassPieza::Color::AZUL : ClassPieza::Color::ROJO;

	if (!ValidadorDeMovimientos::esMovimientoLegal(*ObjTablero, origen, destino, turnoActual)) {
		std::cout << "Movimiento invalido: tu rey quedaria en jaque." << std::endl;
		return;
	}


	if (!intentaMover(origen, destino)) return;

	if (verificaEstadoDelJuego()) return;

	actualizaTurno();
}

bool ClassMundo::intentaMover(const Vector2D& origen, const Vector2D& destino) {


	if (!ObjTablero) return false;
	return ObjTablero->moverPieza(origen, destino);
}

void ClassMundo::seleccionarCasilla(const Vector2D& clicada) {

	ClassPieza* pieza = ObjTablero->getPieza(clicada);

	// TESTTTSS
	if (pieza) {
		std::string tipoTexto;
		switch (pieza->getTipo()) {
		case ClassPieza::Pieza_t::Peon:    tipoTexto = "peon"; break;
		case ClassPieza::Pieza_t::Torre:   tipoTexto = "torre"; break;
		case ClassPieza::Pieza_t::Alfil:   tipoTexto = "alfil"; break;
		case ClassPieza::Pieza_t::Reina:   tipoTexto = "reina"; break;
		case ClassPieza::Pieza_t::Rey:     tipoTexto = "rey"; break;
		case ClassPieza::Pieza_t::Caballo: tipoTexto = "caballo"; break;
		default: tipoTexto = "pieza desconocida"; break;
		}
		ETSIDI::play("sonidos/selec.wav");
		std::string colorTexto = (pieza->getColor() == ClassPieza::Color::AZUL) ? "azul" : "rojo";
		std::cout << "Estas clicando un " << tipoTexto << " " << colorTexto << "\n";
	}

	// Obtener color del turno actual
	ClassPieza::Color turnoActual = reglas.get_turno() ? ClassPieza::Color::AZUL : ClassPieza::Color::ROJO;

	if (!haySeleccionActiva) {
		// Validar que la pieza pertenece al turno actual
		if (pieza && pieza->getColor() == turnoActual) {
			casillaSeleccionada = clicada;
			haySeleccionActiva = true;

			auto movimientosPosibles = pieza->obtenerMovimientosPosibles(*ObjTablero);
			ObjTablero->resaltarMovimientos(movimientosPosibles);
		}
		else {
			std::cout << "No puedes seleccionar una pieza del otro jugador.\n";
		}
	}
	else {
		// Validar que la pieza seleccionada aún pertenece al turno actual
		ClassPieza* piezaSeleccionada = ObjTablero->getPieza(casillaSeleccionada);
		if (!piezaSeleccionada || piezaSeleccionada->getColor() != turnoActual) {
			std::cout << "No puedes mover esa pieza, no es tu turno.\n";
			haySeleccionActiva = false;
			ObjTablero->limpiarResaltados();
			return;
		}

		ObjTablero->limpiarResaltados();
		procesaMovimiento(casillaSeleccionada, clicada);
		haySeleccionActiva = false;
	}
	
	////cout << "Clic en la casilla (" << clicada.x << ", " << clicada.y << ")\n";

	//if (clicada.x < 0 || clicada.y < 0 || !ObjTablero->estaDentro(clicada)) {
	//	//std::cout << "Click invalido o fuera del tablero\n";
	//	return;
	//}

	////TESTTTSS
	//ClassPieza* tpieza = ObjTablero->getPieza(clicada);
	//if (tpieza) {
	//	std::string tipoTexto;
	//	switch (tpieza->getTipo()) {
	//	case ClassPieza::Pieza_t::Peon:    tipoTexto = "peon"; break;
	//	case ClassPieza::Pieza_t::Torre:   tipoTexto = "torre"; break;
	//	case ClassPieza::Pieza_t::Alfil:   tipoTexto = "alfil"; break;
	//	case ClassPieza::Pieza_t::Reina:   tipoTexto = "reina"; break;
	//	case ClassPieza::Pieza_t::Rey:     tipoTexto = "rey"; break;
	//	case ClassPieza::Pieza_t::Caballo: tipoTexto = "caballo"; break;
	//	default: tipoTexto = "pieza desconocida"; break;
	//	}
	//	ETSIDI::play("sonidos/selec.wav");
	//	std::string colorTexto = (tpieza->getColor() == ClassPieza::Color::AZUL) ? "azul" : "rojo";
	//	std::cout << "Estas clicando un " << tipoTexto << " " << colorTexto << "\n";
	//}
	//else {
	//	std::cout << "Casilla vacia (" << clicada.x << ", " << clicada.y << ")\n";
	//}
	////////

	//if (!haySeleccionActiva) {
	//	if (ObjTablero->estaOcupada(clicada)) {
	//		ClassPieza* p = ObjTablero->getPieza(clicada);
	//		if (p && ((reglas.get_turno() && p->getColor() == ClassPieza::Color::AZUL) || (!reglas.get_turno() && p->getColor() == ClassPieza::Color::ROJO))) {
	//			casillaSeleccionada = clicada;
	//			haySeleccionActiva = true;

	//			auto movimientosPosibles = p->obtenerMovimientosPosibles(*ObjTablero);
	//			ObjTablero->resaltarMovimientos(movimientosPosibles);
	//		}

	//	}
	//}
	// 
	//else {
	//	ClassPieza* pieza = ObjTablero->getPieza(casillaSeleccionada);
	//	if (pieza) {
	//		auto movimientos = pieza->obtenerMovimientosPosibles(*ObjTablero);
	//		bool valido = false;
	//		for (const auto& m : movimientos) {
	//			if (m == clicada) {
	//				valido = true;
	//				break;
	//			}
	//		}
	// }
	//		if (valido) {
	//			ObjTablero->moverPieza(casillaSeleccionada, clicada);
	// }
	//			ETSIDI::play("sonidos/mover.wav");

	//			// Verificar jaque después del movimiento
	//			hayJaqueAzul = reglas.hayJaque(*ObjTablero, ClassPieza::Color::AZUL);
	//			hayJaqueRojo = reglas.hayJaque(*ObjTablero, ClassPieza::Color::ROJO);
	//			hayJaqueMateAzul = reglas.hayJaqueMate(*ObjTablero, ClassPieza::Color::AZUL);
	//			hayJaqueMateRojo = reglas.hayJaqueMate(*ObjTablero, ClassPieza::Color::ROJO);
	//			hayReyAhogadoAzul = reglas.hayReyAhogado(*ObjTablero, ClassPieza::Color::ROJO, reglas.turno_); //la funcion analiza si el bando rival ya no le qiedan movimientos
	//			hayReyAhogadoRojo = reglas.hayReyAhogado(*ObjTablero, ClassPieza::Color::AZUL, reglas.turno_);
	//			if (hayJaqueAzul) {
	//				std::cout << "El rey azul esta en jaque!" << std::endl;
	//			}
	//			if (hayJaqueRojo) {
	//				std::cout << "El rey rojo esta en jaque!" << std::endl;
	//			}
	//			if (hayJaqueMateAzul || hayJaqueMateRojo) {
	//				cout << "jaque mate" << endl;
	//			//reset();
	//				
	//			}
	//			else if ((hayReyAhogadoAzul|| hayReyAhogadoRojo) && !(hayJaqueMateAzul || hayJaqueMateRojo) ) {
	//				std::cout << "tablas" << endl;
	//			//reset();
	//			}
	//			else
	//			reglas.set_turno(); // Cambia el turno después de mover


		//promocion

	ClassPieza* piezaFinal = ObjTablero->getPieza(clicada);
		if (piezaFinal){
			if (reglas.get_Promocion(*piezaFinal, static_cast<int>(var_))) {
				hay_promo = true;
				char seleccion;
				cin >> seleccion;//c,t o r
			}
		}
	//		}
	//	}
	//	ObjTablero->limpiarResaltados();
	//	haySeleccionActiva = false;
	//
	//}

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

void ClassMundo::reset() {
	ObjTablero->reset();
	hayJaqueAzul = 0;
	hayJaqueRojo = 0;
	hayJaqueMateAzul = 0;
	hayJaqueMateRojo = 0;
	reglas.turno_ = 1;
	hayReyAhogadoAzul = 0;
	hayReyAhogadoRojo = 0;
}

void ClassMundo::mensajePromo() {
	ETSIDI::setTextColor(1, 1, 0);
	ETSIDI::setFont("fuentes/Bitwise.ttf", 16);
	ETSIDI::printxy("PROMOCION", 5, 4);
	ETSIDI::setFont("fuentes/Bitwise.ttf", 15);
	ETSIDI::printxy("Elegir pieza:", 5, 3);
	ETSIDI::setFont("fuentes/Bitwise.ttf", 14);
	if (var_ == 1) {
		ETSIDI::printxy("  Reina", 5, 2);
		ETSIDI::printxy("  Torre", 5, 1);
	}
	if (var_ == 2) {
		ETSIDI::printxy("  Caballo", 5, 2);
		ETSIDI::printxy("  Torre", 5, 1);
	}
}