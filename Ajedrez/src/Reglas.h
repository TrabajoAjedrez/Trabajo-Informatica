#pragma once
#include "vector2d.h"


class ClassReglas
{
protected:
	bool turno_; //será 1 si le toca a las blancas, 0 si le toca a las negras
	Vector2D ShapeSilverman = { 4,5 };
	Vector2D ShapeDemi = { 4,8 };
	int tiempo_restante;
public:
	//bool turno_siguiente(bool turno);//avisa de que el jugador ha hecho un movimiento

	//obtener shape segun las reglas de Demi y de Silverman
	Vector2D devolver_forma(const int& Variante);

	void actualiza_tiempo();  // Solo decrementa el tiempo
	void inicia_temporizador(int segundos);//inicia en cada turno
	int get_tiempo_restante() const;
};