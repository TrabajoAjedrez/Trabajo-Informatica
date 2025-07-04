﻿#include <iostream>
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

void ClassMundo::tecla(unsigned char key) {
	if (key == 'r') {
		reset();

	}

	if (hay_promo && (key == 'd' || key == 't' || key == 'c') || key == 'a') {
		ObjTablero->promocionarPieza(*piezaPromo, key, static_cast<int>(var_));
		hay_promo = false;
		esperandoPromocion = false; 
		actualizaTurno();           
		verificaEstadoDelJuego();
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
	
	
	int var = TipoTablero;

	var_ = static_cast<Variante>(var);

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


	//temporizador
	reglas.inicia_temporizador(600); // 10 minutos por jugador
	glutTimerFunc(1000, ClassMundo::onTimer, 0); // Arranca el temporizador
	glutTimerFunc(500, ClassMundo::parpadeoExclamacion, 0);

}

void ClassMundo::onTimer(int value) {
	
	bool turno = reglas.get_turno();
	if (mundoPtr) {
		mundoPtr->temporizador();

		if (mundoPtr->TipoJuego == 2) 
		{
		if (!turno) { // Turno de las rojas
			miIA.ejecutarMovimiento(mundoPtr->ObjTablero, reglas);
		}
	}
		if ((turno && reglas.get_tiempo_restante_azules() > 0) ||
			(!turno && reglas.get_tiempo_restante_rojas() > 0)) {
			glutTimerFunc(1000, ClassMundo::onTimer, 0);
		}
		
	}
	glutPostRedisplay();
}

void ClassMundo::temporizador() {
	reglas.actualiza_tiempo();
}

void ClassMundo::mueve() {
	//el tiempo se tiene que checkear constantemente
	finTiempoAzul = reglas.finTiempoAzul();
	finTiempoRojo = reglas.finTiempoRojo();

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
	ObjTablero->dibuja(TipoTablero);
	glPopMatrix();

	// Dibujo del fondo
	if (Lugar == 1)
	{
		float AwX = -5.0f;
		float AwY = -2.0f;

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, ETSIDI::getTexture("imagenes/castillo.png").id);
		glDisable(GL_LIGHTING);
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		if (TipoTablero == 1)
		{
			glTexCoord2d(0, 1);  glVertex2d(0 + AwX, 0 + AwY);
			glTexCoord2d(1, 1);  glVertex2d(14 + AwX, 0 + AwY);
			glTexCoord2d(1, 0);  glVertex2d(14 + AwX, 14 + AwY);
			glTexCoord2d(0, 0);  glVertex2d(0 + AwX, 14 + AwY);
		}
		else if (TipoTablero == 2)
		{
			glTexCoord2d(0, 1);  glVertex2d(0 + AwX, 0 + AwY - 4);
			glTexCoord2d(1, 1);  glVertex2d(18 + AwX, 0 + AwY - 4);
			glTexCoord2d(1, 0);  glVertex2d(18 + AwX, 18 + AwY - 4);
			glTexCoord2d(0, 0);  glVertex2d(0 + AwX, 18 + AwY - 4);
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
	}
	if (Lugar == 2)
	{

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, ETSIDI::getTexture("imagenes/mazmorra.png").id);
		glDisable(GL_LIGHTING);
		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		if (TipoTablero == 1)
		{
			float AwX = -3.0f;
			float AwY = -3.0f;
			glTexCoord2d(0, 1);  glVertex2d(0 + AwX, 0 + AwY);
			glTexCoord2d(1, 1);  glVertex2d(10 + AwX, 0 + AwY);
			glTexCoord2d(1, 0);  glVertex2d(10 + AwX, 10 + AwY);
			glTexCoord2d(0, 0);  glVertex2d(0 + AwX, 10 + AwY);
		}
		else if (TipoTablero == 2)
		{
			float AwX = -5.0f;
			float AwY = -2.0f;
			glTexCoord2d(0, 1);  glVertex2d(0 + AwX, 0 + AwY - 4);
			glTexCoord2d(1, 1);  glVertex2d(18 + AwX, 0 + AwY - 4);
			glTexCoord2d(1, 0);  glVertex2d(18 + AwX, 18 + AwY - 4);
			glTexCoord2d(0, 0);  glVertex2d(0 + AwX, 18 + AwY - 4);
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
	}
	if (Lugar == 3)
	{
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
	}

	string textoRojas = "Tiempo rojas: " + reglas.tiempo_string_rojas();
	string textoAzules = "Tiempo azules: " + reglas.tiempo_string_azules();

	imprime_tiempo(textoRojas.c_str(),5);//c_str para de string a char*
	imprime_tiempo(textoAzules.c_str(), 4);

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



	if (hayReyAhogadoAzul || hayReyAhogadoRojo || reglas.empateReyes(*ObjTablero)) {
		std::cout << "Tablas!.\n";
		ETSIDI::play("sonidos/tablas.wav");
		hayempate = true;
	}
	if (hayJaqueMateAzul) std::cout << "Mate al azul\n";
	if (hayJaqueMateRojo) std::cout << "Mate al rojo\n";
	
	if (finTiempoAzul) cout << "Tiempo agotado para las azules!\n";
	if (finTiempoRojo) cout << "Tiempo agotado para las rojas!\n";
	return hayJaqueMateAzul || hayJaqueMateRojo || hayempate;
}

void ClassMundo::procesaMovimiento(const Vector2D& origen, const Vector2D& destino) {


	ClassPieza::Color turnoActual = reglas.get_turno() ? ClassPieza::Color::AZUL : ClassPieza::Color::ROJO;

	if (!ValidadorDeMovimientos::esMovimientoLegal(*ObjTablero, origen, destino, turnoActual)) {
		std::cout << "Movimiento invalido: tu rey quedaria en jaque." << std::endl;
		return;
	}

	ClassPieza* piezaEnOrigen = ObjTablero->getPieza(origen);
	bool eraPeon = piezaEnOrigen && piezaEnOrigen->getTipo() == ClassPieza::Peon;
	ClassPieza::Color colorPiezaMovida = ClassPieza::Color::AZUL; // Valor por defecto
	if (piezaEnOrigen) { // Asegurarse que la pieza existe antes de obtener su color
		colorPiezaMovida = piezaEnOrigen->getColor();
	}

	if (!intentaMover(origen, destino)) {
		haySeleccionActiva = false; 
		ObjTablero->limpiarResaltados();
		return;
	}

	ObjTablero->limpiarCasillaEnPasante();

		if (eraPeon) { // Usamos la información guardada de 'eraPeon'.
		int diffFilas = destino.x - origen.x; // Diferencia con signo.
		if (abs(diffFilas) == 2) { // Movió dos casillas.
			Vector2D casillaSaltada;
			if (colorPiezaMovida == ClassPieza::Color::ROJO) { // ROJO mueve en dirección positiva de filas.
				casillaSaltada = Vector2D(origen.x + 1, origen.y);
				}
			else { // AZUL mueve en dirección negativa de filas.
				casillaSaltada = Vector2D(origen.x - 1, origen.y);
				}
			ObjTablero->setCasillaEnPasante(casillaSaltada, colorPiezaMovida);
			std::cout << "Captura al paso posible en (" << casillaSaltada.x << "," << casillaSaltada.y << ") contra el peon "
				<< (colorPiezaMovida == ClassPieza::Color::ROJO ? "ROJO" : "AZUL") << "." << std::endl;
			}
		}

		//promocion

		ClassPieza* piezaEnDestino = ObjTablero->getPieza(destino);
		if (piezaEnDestino && reglas.get_Promocion(*piezaEnDestino, static_cast<int>(var_))) {
			esperandoPromocion = true;
			hay_promo = true;
			piezaPromo = piezaEnDestino;
			return;
		}
		//llamo a la promocion en tecla

	if (verificaEstadoDelJuego()) return;

	if (!esperandoPromocion)
		actualizaTurno();
}

bool ClassMundo::intentaMover(const Vector2D& origen, const Vector2D& destino) {


	if (!ObjTablero) return false;
	return ObjTablero->moverPieza(origen, destino);
}

void ClassMundo::seleccionarCasilla(const Vector2D& clicada) {

	ClassPieza* pieza = ObjTablero->getPieza(clicada);

	if (hay_promo || esperandoPromocion) {
		std::cout << "Debes promocionar antes de seguir jugando.\n";
		return;
	}

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
		ObjTablero->limpiarResaltados();
		procesaMovimiento(casillaSeleccionada, clicada);
		haySeleccionActiva = false;
	}
	

		

}



void ClassMundo::imprime_tiempo(const char* text, float y) {
	bool turn = reglas.get_turno();
	if(turn==0)
		ETSIDI::setTextColor(1, 0, 0);
	else if(turn==1)
		ETSIDI::setTextColor(0, 0, 1);
	ETSIDI::setFont("fuentes/Bitwise.ttf", 16);
	float x;
	if (var_ == 1)
		x = 5;
	else if (var_ == 2)
		x = 7;
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
	finTiempoAzul = 0;
	finTiempoRojo = 0;
	hayempate = 0;
}

void ClassMundo::mensajePromo() {
	ETSIDI::setTextColor(1, 1, 0);
	ETSIDI::setFont("fuentes/Bitwise.ttf", 16);
	int x;//distinto para cada variante
	if (var_ == 1) {
		x = 5;
		ETSIDI::printxy("  Dama:  d", x, 1);
		ETSIDI::printxy("  Torre: t", x, 0);
	}
	if (var_ == 2) {
		x = 7;
		ETSIDI::printxy("  Alfil: a", x, 1); 
		ETSIDI::printxy("  Caballo: c", x, 0);
		ETSIDI::printxy("  Torre: t", x, -1);
	}
	ETSIDI::printxy("PROMOCION", x, 3);
	ETSIDI::setFont("fuentes/Bitwise.ttf", 15);
	ETSIDI::printxy("Elegir pieza:", x, 2);
	ETSIDI::setFont("fuentes/Bitwise.ttf", 14);
}