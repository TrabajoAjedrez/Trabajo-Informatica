#pragma once
#include"Tablero.h"
#include "vector2d.h"
#include "raton.h"

class ClassMundo {
public:
	enum Variante { Silverman4x5, Demi };
	double x_ojo;
	double y_ojo;
	double z_ojo;
	Variante var_;

protected:

	ClassTablero* ObjTablero; //Puntero a la clase que contiene el tablero
	//raton ratonObj; // crea instancia global del ratón
	Vector2D  casillaSeleccionada;
	bool haySeleccionActiva = false;//? no deberia ser public?
	

public:


	// Cosntructor, inicializa el puntero a null
	ClassMundo(double xo=0, double yo=0,double zo=0, Variante var=Demi) 
		: ObjTablero(nullptr), x_ojo(xo), y_ojo(yo), z_ojo(zo), var_(var)
	{}
	// Destructor, libera la memoria del tablero
	~ClassMundo() {
		delete ObjTablero;
	}

	int PreguntarVariante();
	void tecla(unsigned char key);
	void tecla_especial(unsigned char key);
	void inicializa();
	void rotarOjo();
	void mueve();
	void dibuja();

	//Funciones de temporizador
	static void onTimer(int value); //para convertir el puntero en metodo
	void temporizador();
	void imprime_tiempo(const char* text, float x, float y);

	// Se podria mejorar
	void mueve_pieza(const Vector2D& clicada);
	//raton& getRaton() { return ratonObj; }

	

	int getFilas() const;
	int getColumnas() const;

};