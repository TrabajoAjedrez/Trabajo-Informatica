#pragma once
#include"Tablero.h"
#include "vector2d.h"
#include "raton.h"
#include "mundo.h"
#include "ETSIDI.h"
#include "Coordinador.h"
#include "MENU.h"
#include <vector>

Menu menu;
// Constructor
Coordinador::Coordinador()
	: estado(MENU), ObjMundo(nullptr), nivel(1)
{
	ObjMundo = new ClassMundo(); // Reserva memoria para el mundo de juego
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
	}
}

void Coordinador::tecla(unsigned char key) {
	switch (estado) {
	case MENU:
		break;
	case JUEGO:
		ObjMundo->tecla(key);             // Gestiona teclas en modo juego
		break;
	}
}

void Coordinador::mueve()
{
	if (estado == JUEGO)ObjMundo->mueve();
}