#pragma once
#include"Tablero.h"

class ClassMundo {

	ClassTablero *ObjTablero;

public:

	ClassMundo() : ObjTablero(nullptr) {}
	~ClassMundo() {
		delete ObjTablero;
	}

	void tecla(unsigned char key);
	void tecla_especial(unsigned char key);
	void inicializa(int Variante);
	void rotarOjo();
	void mueve();
	void dibuja();
	void matriz();

	double x_ojo;
	double y_ojo;
	double z_ojo;

	//friend class Pieza;
};