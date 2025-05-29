#pragma once
#include"Tablero.h"
#include "vector2d.h"
#include "raton.h"
#include <vector>

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
	bool haySeleccionActiva = false; //por que no son public?
	bool hayJaqueAzul = false;
	bool hayJaqueMateAzul = false;
	bool hayJaqueRojo = false;
	bool hayJaqueMateRojo = false;
	bool hayReyAhogadoAzul = false;
	bool hayReyAhogadoRojo = false;

	float tiempoRebote = 0.0f;
	bool visibleExclamacion = true;
	static void parpadeoExclamacion(int value);



public:
	// Cosntructor, inicializa el puntero a null
	ClassMundo(double xo = 0, double yo = 0, double zo = 0, Variante var = Demi)
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

	void procesaMovimiento(const Vector2D& origen, const Vector2D& destino);
	bool intentaMover(const Vector2D& origen, const Vector2D& destino);
	bool verificaEstadoDelJuego();
	void actualizaTurno();





	//funciones de tiempo
	static void onTimer(int value); //para convertir el puntero en metodo
	void temporizador();
	void imprime_tiempo(const char* text, float x, float y);
	void dibujarExclamacionSobreRey(const Vector2D& posRey, ClassPieza::Color color, float tiempoRebote);
	// Se podria mejorar
	void seleccionarCasilla(const Vector2D& clicada);
	//raton& getRaton() { return ratonObj; }

	int getFilas() const;
	int getColumnas() const;

	void reset(); //poner jaques a 0 y lo que se necesite en resetar en mundo

};