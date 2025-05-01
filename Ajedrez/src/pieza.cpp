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
void Pieza::ubica( int** tablero, int f, int c) {

	for (int i = 0; i < f; i++) {

		for (int j = 0; j < c; j++) {
			float x = j + 0.5;
			float z = i + 0.5;
				dibuja(tablero[i][j], x, z);
	
			
		}

	}
}
void Pieza::dibuja(int p, float x, float z) {
	glPushMatrix();
	glTranslated(x, 1, z);
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
	//fin del codigo incluido
	glPopMatrix();
}
vector<Vector2D> Pieza::obtenerMovimientosPosibles(const vector<vector<int>>& tablero){
	vector<Vector2D> movimientos; //Variable donde guardaremos los movimientos posibles y la que devuelve la funcion
	int filaActual = posicion.y;
	int columnaActual = posicion.x;

}