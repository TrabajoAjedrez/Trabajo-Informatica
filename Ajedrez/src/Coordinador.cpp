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
		/*
		if (key == 'e' || key == 'E') {   // <--- Aquí era la condición
			estado = JUEGO;               // Cambia el estado a JUEGO
			ObjMundo->inicializa();       // Inicializa el mundo/tablero
		}
		*/
		break;
	case JUEGO:
		/*
		ObjMundo->tecla(key); 
		if (key == 'e' || key == 'E') {   // <--- Aquí era la condición
			estado = GAMEOVER;               // Cambia el estado a JUEGO
			      // Inicializa el mundo/tablero
		}
		*/
		break;
		
		
		
	}
}


void Coordinador::tecla_especial(unsigned char key)
{
	if (estado == JUEGO)ObjMundo->tecla_especial(key);
}


//ClassReglas reglas;

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
//Coordinador.cpp









