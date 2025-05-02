//#pragma once
//#include <iostream>
//#include"ETSIDI.h"
//#include <vector2d.h>
//
//using namespace std;
//
//class Pieza {
//
//public: 
//	enum  Pieza_t { Peon=1,Caballo, Torre,Alfil, Reina,Rey };
//	enum class Color { BLANCO, NEGRO };
//
//protected:
//	Pieza_t tipo;
//	Color color;
//	Vector2D pos;  //No se sabe si se va a utilizar
//	ETSIDI::SpriteSequence sprite{ "Imagenes/soldier1.png",4 };
//	ETSIDI::SpriteSequence sprite2{ "Imagenes/peonRojo.png",4 };
//public:
//	Pieza() {
//		
//		sprite.setCenter(1, 1);
//		sprite.setSize(1.5, 1.5);
//		sprite2.setCenter(1, 1);
//		sprite2.setSize(1.5, 1.5);
//	}
//	// Constructor
//	Pieza(Pieza_t t, Color c, Vector2D p);
//
//	// Métodos para consultar las propiedades privadas
//	Pieza_t getTipo()const;
//	Color getColor()const;
//	Vector2D getPos()const;   //No se sabe si se va a utilizar
//
//	// Métodos para modificar las propiedades privadas
//	void setTipo(Pieza_t t);
//	void setColor(Color c);
//	void setPos(Vector2D p);
//
//	//Testeo matriz por pantalla
//	void print( int** tablero, int f, int c);
//
//	//Pone cada ficha en su sitio
//	void coloca(int** tablero, int f, int c);
//
//	//Gestión del Sprite
//	void ubica(int** tablero, int f, int c);
//	void muevePeon() { sprite.loop(); sprite2.loop();}
//	void dibuja(int p, float x, float i);
//
//	bool esPosicionValida(int **tablero ) const;
//};
