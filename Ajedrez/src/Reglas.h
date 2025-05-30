#pragma once
#include "vector2d.h"
#include "Tablero.h"
#include "Pieza.h"
#include "Mundo.h"
#include <optional>


class ClassReglas
{
protected:
	Vector2D ShapeSilverman = { 4,5 };
	Vector2D ShapeDemi = { 4,8 };
	//int tiempo_restante = 0;
    int tiempo_restante_azules=0;
	int tiempo_restante_rojas = 0;
public:

	bool turno_ = 1; //será 1 si le toca a las azules, 0 si le toca a las rojas
	
	//obtener shape segun las reglas de Demi y de Silverman
	Vector2D devolver_forma(const int& Variante);

	void set_turno();
	bool get_turno()const;
	ClassPieza::Color getColorTurno() const;


	//detectar jaque
	Vector2D buscarRey(const ClassTablero& tablero, ClassPieza::Color colorRey);
	bool hayJaque(const ClassTablero& tablero, ClassPieza::Color colorRey);
	bool hayJaqueMate(const ClassTablero& tablero, ClassPieza::Color colorRey);

	// Posición del rey en jaque, si lo hay
	std::optional<std::pair<Vector2D, ClassPieza::Color>> getReyEnJaque(const ClassTablero& tablero);

	//tablas
	bool hayReyAhogado(const ClassTablero& tablero, ClassPieza::Color colorRey, int turno); //deteta rey ahogado

	//para saber si una posicion esta siendo amenazada (necesario para el mov del rey, detectar jaque no vale en este caso)
	bool PosAmenzada(  Vector2D pos, const ClassTablero& tablero, ClassPieza* Pieza); 

	//PROMOCION
	bool get_Promocion(const ClassPieza& pieza, int var);//identifica si la pieza esta en posicion de promocion


	//FUNCIONES DE TEMPORIZADORES

	void actualiza_tiempo();  // Solo decrementa el tiempo
	void inicia_temporizador(int segundos);//inicia en cada turno
	int get_tiempo_restante_rojas() const;
	int get_tiempo_restante_azules() const;

	//Empate por solo dos reyes
	bool empateReyes(const ClassTablero& tablero) const;

	//pasar el tiempo a string
	string tiempo_string();
};