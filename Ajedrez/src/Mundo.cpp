#include <iostream>
#include "Mundo.h"
#include "freeglut.h"

using namespace std;

void ClassMundo::tecla(unsigned char key) {

}
void ClassMundo::tecla_especial(unsigned char key) {

}
void ClassMundo::inicializa(int Variante) {
	
	switch (Variante) 
	{
	case 1: 
		ObjTablero = new ClassTablero(4, 5); 
		break;
	case 2: 
		ObjTablero = new ClassTablero(8, 4); 
		break;
	default: 
		 break;
	}

	x_ojo = ObjTablero->getFilas() / 2;
	y_ojo = ObjTablero->getColumnas() / 2;
	z_ojo = ObjTablero->getFilas() * 2.0;

	ObjTablero->colocarPiezas();
}
void ClassMundo::rotarOjo() {
	double dist = sqrt(x_ojo * x_ojo + z_ojo * z_ojo);
	double ang = atan2(z_ojo, x_ojo);
	ang += 0.01;
	x_ojo = dist * cos(ang);
	z_ojo = dist * sin(ang);
}
void ClassMundo::mueve() {

	ObjTablero->muevePiezas();

}
void ClassMundo::dibuja() {
	gluLookAt(x_ojo, y_ojo, z_ojo,
		ObjTablero->getFilas()/2, ObjTablero->getColumnas()/2, 0, //NOTESE QUE HEMOS CAMBIADO ESTO
		0.0, 1.0, 0.0); //PARA MIRAR AL CENTRO DE LA ESCENA

	glPushMatrix();
	ObjTablero->dibuja(); // Llama al método de dibujo del tablero
	glPopMatrix();
	//pieza.ubica( ObjTablero, filas_, columnas_);
}
