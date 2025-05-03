#include"Pieza.h"
#include"vector2d.h"
#include"ETSIDI.h"
#include"freeglut.h"
using namespace std;
// Constructor
ClassPieza::ClassPieza(Pieza_t t, Color c, Vector2D p) : tipo(t), color(c), pos(p) {}

// Métodos para consultar las propiedades privadas
ClassPieza::Pieza_t ClassPieza::getTipo() const { return tipo; }
ClassPieza::Color ClassPieza::getColor() const { return color; }
Vector2D ClassPieza::getPos() const { return pos; }

// Métodos para modificar las propiedades privadas
void ClassPieza::setTipo(Pieza_t t) { tipo = t; }
void ClassPieza::setColor(Color c) { color = c; }
void ClassPieza::setPos(Vector2D p) { pos = p; }
// Método para imprimir la matriz del tablero
void ClassPieza::print( int** tablero,int f, int c)  {
	for (int i = 0; i < f; i++) {
		for (int j = 0; j < c; j++) {
			cout << tablero[i][j] << " ";
		}
		cout << endl;
	}
}
// Método para colocar las piezas en la matriz del tablero ¡¡No el dibujo de las piezas!!
void ClassPieza::coloca(int** tablero, int f, int c) {
	int a = 1, b = 0;
	if (f == 8)
		b = 6;
	else if (f == 5)
		b = 3;

	for (int i = 0; i < f; i++) {
		for (int j = 0; j < c; j++) {
			if (i == a) tablero[i][j] = -1;
			else if (i == b) tablero[i][j] = 1;
		}
	}
}
// Método para dibujar el sprite la pieza
void ClassPieza::dibuja(int p, float x, float y) {
	glPushMatrix();
	glTranslated(x, y, 0.1);
	if (p == 1) {
		if (sprite.getState() == 0)
			sprite.setState(0, false);
		sprite.draw();
	}
	if (p == -1) {
		if (sprite2.getState() == 0)
			sprite2.setState(0, false);
		sprite2.draw();
	}
	glPopMatrix();
}

