#pragma once
#include "vector2d.h"
#include "Tablero.h"
#include "Pieza.h"


class ClassReglas
{
protected:
	bool turno_ = 1; //será 1 si le toca a las azules, 0 si le toca a las rojas
	Vector2D ShapeSilverman = { 4,5 };
	Vector2D ShapeDemi = { 4,8 };
	int tiempo_restante = 0;
    //int tiempo_restante_azules=0;
	//int tiempo_restante_rojas = 0;
public:
	//bool turno_siguiente(bool turno);//avisa de que el jugador ha hecho un movimiento

	//obtener shape segun las reglas de Demi y de Silverman
	Vector2D devolver_forma(const int& Variante);

	void actualiza_tiempo();  // Solo decrementa el tiempo
	void set_turno();
	void inicia_temporizador(int segundos);//inicia en cada turno
	int get_tiempo_restante() const;
	bool get_turno()const;
	bool hayJaque(const ClassTablero& tablero, ClassPieza::Color colorRey);

};