#include <iostream>
#include <cassert>
#include "Mundo.h"
#include "freeglut.h"
#include "Reglas.h"
#include "ETSIDI.h"

using namespace std;

ClassReglas reglas; 
static ClassMundo* mundoPtr = nullptr;

// Para el parpadeo de la exclamacion
//bool visibleExclamacion = true;
//int tiempoParpadeo = 500;


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

	if (hay_promo && (key == 'd' || key == 't' || key == 'c') || key == 'a') {
		ObjTablero->promocionarPieza(*piezaPromo, key, static_cast<int>(var_));
		hay_promo = false;
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



void ClassMundo::inicializa()
{

	mundoPtr = this; 

	// Inicializa el tablero según la variante seleccionada
	// Inicializa el tablero según la variante seleccionada
	//int var = PreguntarVariante();
	int var = TipoTablero;

	var_ = static_cast<Variante>(var);

	//std::cout << var_ << endl;

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

	// Dibujo del fondo
	float AwX = -4.0f;
	float AwY = -4.0f;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ETSIDI::getTexture("imagenes/campo.png").id);
	glDisable(GL_LIGHTING);
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	if (TipoTablero == 1)
	{
		glTexCoord2d(0, 1);  glVertex2d(0 + AwX, 0 + AwY);
		glTexCoord2d(1, 1);  glVertex2d(12 + AwX, 0 + AwY);
		glTexCoord2d(1, 0);  glVertex2d(12 + AwX, 12 + AwY);
		glTexCoord2d(0, 0);  glVertex2d(0 + AwX, 12 + AwY);
	}
	else if (TipoTablero == 2)
	{
		glTexCoord2d(0, 1);  glVertex2d(0 + AwX, 0 + AwY);
		glTexCoord2d(1, 1);  glVertex2d(18 + AwX, 0 + AwY);
		glTexCoord2d(1, 0);  glVertex2d(18 + AwX, 18 + AwY);
		glTexCoord2d(0, 0);  glVertex2d(0 + AwX, 18 + AwY);
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);


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
	

		//promocion

	ClassPieza* piezaFinal = ObjTablero->getPieza(clicada);

	if (piezaFinal) {
		if (reglas.get_Promocion(*piezaFinal, static_cast<int>(var_))) {
			hay_promo = true;
			piezaPromo = piezaFinal;
		}
	}
	//llamo a la promocion en tecla

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
		ETSIDI::printxy("  Dama:  d", 5, 2);
		ETSIDI::printxy("  Torre: t", 5, 1);
	}
	if (var_ == 2) {
		ETSIDI::printxy("  Alfil: a", 5, 2); 
		ETSIDI::printxy("  Caballo: c", 5, 1);
		ETSIDI::printxy("  Torre: t", 5, 0);
	}
}