#pragma once
#include <iostream>
#include"ETSIDI.h"
using namespace std;

class Pieza {
	enum  Pieza_t { Peon=1,Caballo, Torre,Alfil, Reina,Rey };
	ETSIDI::SpriteSequence sprite{ "Imagenes/soldier1.png",4 };
public:
	Pieza() {
		sprite.setCenter(1, 0);
		sprite.setSize(1.5, 1.5);
	}
	void print( int** tablero, int f, int c);
	void coloca(int** tablero, int f, int c);
	void ubica(int** tablero, int f, int c);
	void muevePeon() { sprite.loop(); }
	void dibuja(int p, float x, float i);
};
