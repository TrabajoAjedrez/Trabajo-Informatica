#include <iostream>
#include "tablero.h"
#include "freeglut.h"


TableroGL tablerogl;
using namespace std;

void Tablero::tablero_tamanio() {

	int tecla;
	cout << "introduce 1 o 2" << endl;
	cin >> tecla;

	switch (tecla) {
	case 1://caso Silverman 4x5
		filas_ = 5;
		columnas_ = 4;
		break;
	case 2:
		filas_ = 8;
		columnas_ = 4;
		break;
	default:
		cout << "error en el switch" << endl;
		break;
	}

}

void Tablero::tecla(unsigned char key) {

}
void Tablero::tecla_especial(unsigned char key) {

}
void Tablero::inicializa() {
	x_ojo = 0.1;
	y_ojo = 15;
	z_ojo = 4;
}
void Tablero::rotarOjo() {

}
void Tablero::mueve() {

}
void Tablero::dibuja() {
	gluLookAt(x_ojo, y_ojo, z_ojo,
		0.0, 0.0, 0.0, //NOTESE QUE HEMOS CAMBIADO ESTO
		0.0, 1.0, 0.0); //PARA MIRAR AL CENTRO DE LA ESCENA

	glPushMatrix();
	glTranslatef(-4.0f, 0.0f, -4.0f);  // Ajusta si tu tablero va de (0,0) a (8,8)
	tablerogl.dibuja(filas_, columnas_);                   // Llama al método de dibujo del tablero
	glPopMatrix();
}