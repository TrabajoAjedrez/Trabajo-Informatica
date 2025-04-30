#include"pieza.h"
#include"vector2d.h"
#include"ETSIDI.h"
#include"freeglut.h"
using namespace std;
void Pieza::print( int** tablero,int f, int c)  {
	for (int i = 0; i < f; i++) {
		for (int j = 0; j < c; j++) {
			cout << tablero[i][j] << " ";
		}
		cout << endl;
	}
}
void Pieza::coloca(int** tablero, int f, int c) {
	for (int i = 0; i < f; i++) {
		for (int j = 0; j < c; j++) {
			if (i == 1) tablero[i][j] = -1;
			else if (i == 6) tablero[i][j] = 1;
		}
	}
}
void Pieza::ubica( int** tablero, int f, int c) {

	int p;
	for (int i = 0; i < f; i++) {
		for (int j = 0; j < c; j++) {
			if (tablero[i][j] == 1) {
				p = 1;
				float x = j * 1.0f-3;
				float z = i *0.5f;
				dibuja(p, x, z);
			}
		}
	}
}
void Pieza::dibuja(int p, float x, float z) {
	glPushMatrix();
	glTranslated(x, 0.00000001, z);
		if (sprite.getState() == 0)
			sprite.setState(0, false);
		sprite.draw();
	//fin del codigo incluido
	glPopMatrix();
}