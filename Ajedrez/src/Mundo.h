#pragma once
#include"Tablero.h"

class ClassMundo {

	ClassTablero* ObjTablero; //Puntero a la clase que contiene el tablero

public:
	// Cosntructor, inicializa el puntero a null
	ClassMundo() 
		: ObjTablero(nullptr), x_ojo(0), y_ojo(0), z_ojo(0)	
	{}
	// Destructor, libera la memoria del tablero
	~ClassMundo() {
		delete ObjTablero;
	}

	void tecla(unsigned char key);
	void tecla_especial(unsigned char key);
	void inicializa(int Variante);
	void rotarOjo();
	bool turno(bool turn=1);//lo pongo a 1 porque empiezan blancas
	void mueve();
	void dibuja();

	double x_ojo;
	double y_ojo;
	double z_ojo;

	///// Tets ///// (No hacer mucho caso jeje)
	void testPeonBloqueado();
	void testPeonMovimientoBasico();
	void runAllTests();



	int getFilas() const;
	int getColumnas() const;
};