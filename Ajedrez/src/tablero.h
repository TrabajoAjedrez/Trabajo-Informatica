#pragma once
#include "classrey.h"
#include"classreina.h"
#include"tablerogl.h"
#include"pieza.h"


class Tablero {
private:
	int filas_ ;
	int columnas_;
	Pieza pieza;
public:
	int** tablero; //matriz de punteros a enteros
	//constructor
	Tablero(int filas, int columnas) :
		filas_{filas}, columnas_{columnas}
	{	//ahora dependiendo de la variación ahbrá unas filas y unas columnas
		tablero = new int* [filas];
		for (int i = 0; i < filas; i++) {
			tablero[i] = new int[columnas];
		}

		//tambien inicializo tablerogl
		//tablerogl = new TableroGL;
	}

	//destructor
	~Tablero() {
		for (int i = 0; i < filas_; i++) {
			delete[] tablero[i];
		}
		delete[] tablero;
	}


	void tecla(unsigned char key);
	void tecla_especial(unsigned char key);
	void inicializa();
	void rotarOjo();
	void mueve();
	void dibuja();
	void matriz();

	double x_ojo;
	double y_ojo;
	double z_ojo;
};