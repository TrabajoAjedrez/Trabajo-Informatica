#pragma once
#include"Tablero.h"
#include "vector2d.h"
#include "raton.h"

class ClassMundo {

	ClassTablero* ObjTablero; //Puntero a la clase que contiene el tablero
	//raton ratonObj; // crea instancia global del rat�n
	Vector2D  casillaSeleccionada;
	bool haySeleccionActiva = false;

public:
	// Cosntructor, inicializa el puntero a null
	ClassMundo() 
		: ObjTablero(nullptr), x_ojo(0), y_ojo(0), z_ojo(0)	
	{}
	// Destructor, libera la memoria del tablero
	~ClassMundo() {
		delete ObjTablero;
	}

	static void onTimer(int value); //para convertir el puntero en metodo
	void temporizador();
	void tecla(unsigned char key);
	void tecla_especial(unsigned char key);
	void inicializa(int Variante);
	void rotarOjo();
	void mueve();
	void dibuja();

	// M�todo para seleccionar una casilla // Se podria mejorar
	void seleccionarCasilla(const Vector2D& clicada);
	//raton& getRaton() { return ratonObj; }

	double x_ojo;
	double y_ojo;
	double z_ojo;

	int getFilas() const;
	int getColumnas() const;

};