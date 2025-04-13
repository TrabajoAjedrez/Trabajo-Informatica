#pragma once
#include "classrey.h"
#include"classreina.h"
#include"tablerogl.h"
class Tablero {
public:
	TableroGL tablero;
	void tecla(unsigned char key);
	void tecla_especial(unsigned char key);
	void inicializa();
	void rotarOjo();
	void mueve();
	void dibuja();

	double x_ojo;
	double y_ojo;
	double z_ojo;
};