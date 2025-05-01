#pragma once
#include <iostream>
#include <vector>
#include"ETSIDI.h"
#include"vector2d.h"

using namespace std;

class Pieza {
	enum  Pieza_t { Peon=1,Caballo, Torre,Alfil, Reina,Rey };
	enum Color { BLANCO, NEGRO }; //Provisional para saber de que bando es la pieza
	Vector2D posicion; //Variable donde guardar la posicion de la pieza
	Color color;
	
	ETSIDI::SpriteSequence sprite{ "Imagenes/soldier1.png",4 };
	ETSIDI::SpriteSequence sprite2{ "Imagenes/peonRojo.png",4 };

public:
	Pieza() {
		
		sprite.setCenter(1, 1);
		sprite.setSize(1.5, 1.5);
		sprite2.setCenter(1, 1);
		sprite2.setSize(1.5, 1.5);
	}
	void print( int** tablero, int f, int c);
	void coloca(int** tablero, int f, int c);
	void ubica(int** tablero, int f, int c);
	void muevePeon() { sprite.loop(); sprite2.loop();}
	void dibuja(int p, float x, float i);
	//

	//Vector2D obtenerPosicion() const { return posicion; }
	//Color obtenerColor() const { return color; }

	//vector<Vector2D> obtenerMovimientosPosibles(const vector<vector<int>>& tablero); //Funcion que devuelve un vector el cual contendra todos los movimientos posibles
};
