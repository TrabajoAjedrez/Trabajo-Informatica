//#include"pieza.h"
//#include"vector2d.h"
//#include"ETSIDI.h"
//#include"freeglut.h"
//using namespace std;
//// Constructor
//Pieza::Pieza(Pieza_t t, Color c, Vector2D p) : tipo(t), color(c), pos(p) {}
//
//// Métodos para consultar las propiedades privadas
//Pieza::Pieza_t Pieza::getTipo() const { return tipo; }
//Pieza::Color Pieza::getColor() const { return color; }
//Vector2D Pieza::getPos() const { return pos; }
//
//// Métodos para modificar las propiedades privadas
//void Pieza::setTipo(Pieza_t t) { tipo = t; }
//void Pieza::setColor(Color c) { color = c; }
//void Pieza::setPos(Vector2D p) { pos = p; }
//
//void Pieza::print( int** tablero,int f, int c)  {
//	for (int i = 0; i < f; i++) {
//		for (int j = 0; j < c; j++) {
//			cout << tablero[i][j] << " ";
//		}
//		cout << endl;
//	}
//}
//void Pieza::coloca(int** tablero, int f, int c) {
//	int a = 1, b = 0;
//	if (f == 8)
//		b = 6;
//	else if (f == 5)
//		b = 3;
//
//	for (int i = 0; i < f; i++) {
//		for (int j = 0; j < c; j++) {
//			if (i == a) tablero[i][j] = -1;
//			else if (i == b) tablero[i][j] = 1;
//		}
//	}
//}
//void Pieza::ubica( int** tablero, int f, int c) {
//
//	for (int i = 0; i < f; i++) {
//
//		for (int j = 0; j < c; j++) {
//			float x = j + 0.8;
//			float z = i + 0.9;
//				dibuja(tablero[i][j], x, z);
//	
//			
//		}
//
//	}
//}
//void Pieza::dibuja(int p, float x, float y) {
//	glPushMatrix();
//	glTranslated(x, y, 0.1);
//	if (p == 1) {
//		if (sprite.getState() == 0)
//			sprite.setState(0, false);
//		sprite.draw();
//	}
//	if (p == -1) {
//		if (sprite2.getState() == 0)
//			sprite2.setState(0, false);
//		sprite2.draw();
//	}
//	//fin del codigo incluido
//	glPopMatrix();
//}
//
