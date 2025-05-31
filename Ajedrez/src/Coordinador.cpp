#pragma once
#include"Tablero.h"
#include "vector2d.h"
#include "raton.h"
#include "mundo.h"
#include "ETSIDI.h"
#include "Coordinador.h"
#include "Reglas.h"
#include "MENU.h"
#include "endfin.h"
#include <vector>

Menu menu;
endfin ENDFIN;
// Constructor
Coordinador::Coordinador()
	: estado(MENU), ObjMundo(nullptr), nivel(1)
{
	ObjMundo = new ClassMundo(); // Reserva memoria para el mundo de juego
	// Aquí puedes inicializar más cosas si necesitas
}

// Destructor
Coordinador::~Coordinador() {
	delete ObjMundo;  // Libera la memoria
}

void Coordinador::dibuja()
{
	switch (estado) {
	case MENU:
		gluLookAt(0, 7.5, 30, // posicion del ojo
			0.0, 7.5, 0.0, // hacia que punto mira (0,7.5,0)
			0.0, 1.0, 0.0); // definimos hacia arriba (eje Y)

		estado;


		menu.dibujarMenu();
		break;
	case JUEGO:
		ObjMundo->dibuja();
		break;
	case GAMEOVER:
		gluLookAt(0, 7.5, 30, // posicion del ojo
			0.0, 7.5, 0.0, // hacia que punto mira (0,7.5,0)
			0.0, 1.0, 0.0); // definimos hacia arriba (eje Y)

		ENDFIN.dibujarGAMEOVER();
		break;

	case YOUWIN:
		gluLookAt(0, 7.5, 30, // posicion del ojo
			0.0, 7.5, 0.0, // hacia que punto mira (0,7.5,0)
			0.0, 1.0, 0.0); // definimos hacia arriba (eje Y)

		ENDFIN.dibujarYOUWIN();
		break;
	}

}

void Coordinador::tecla(unsigned char key) {
	switch (estado) {
	case MENU:
		break;
	case JUEGO:
		if (ObjMundo != nullptr) {
			ObjMundo->tecla(key); // Pasa las teclas al mundo para acciones en el juego
		}
		break;
	case GAMEOVER:
	case YOUWIN:
	//case EMPATE: //Habria que añadirlo
		std::cout << "Tecla '" << key << "' presionada en pantalla de fin de partida. Volviendo al MENU..." << std::endl;
		estado = MENU;
		if (ObjMundo != nullptr) {
			ObjMundo->reset();
		}
		//Resetaer las opciones del menu
		menu.TipoJuego = 0;
		menu.Lugar = 0;
		menu.TipoTablero = 0;

		break;
	}
	glutPostRedisplay();
}

void Coordinador::tecla_especial(unsigned char key)
{
	if (estado == JUEGO)ObjMundo->tecla_especial(key);
}

void Coordinador::mueve()
{
	if (estado == JUEGO)
	{
		ObjMundo->mueve();
		if (ObjMundo->getHayJaqueMateAzul()  == 1) {
			estado = GAMEOVER;
		}
		if(ObjMundo->getHayJaqueMateRojo() == 1) {
			estado = YOUWIN;
		}
	}
}










