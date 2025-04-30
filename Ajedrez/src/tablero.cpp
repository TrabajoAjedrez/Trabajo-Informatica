#include "tablero.h"
#include "freeglut.h"
#include"pieza.h"
#include"tablerogl.h"
TableroGL tablerogl;

void Tablero::tecla(unsigned char key) {

}
void Tablero::tecla_especial(unsigned char key) {

}
void Tablero::inicializa() {
	x_ojo = 4;
	y_ojo = 8;
	z_ojo = 10;
	
	pieza.coloca( tablero,  filas_,  columnas_ );
	pieza.print(tablero, filas_, columnas_);
}
void Tablero::rotarOjo() {

}
void Tablero::mueve() {
	pieza.muevePeon();
}
void Tablero::dibuja() {
	gluLookAt(x_ojo, y_ojo, z_ojo,
		0.0, 0.0, 0.0, //NOTESE QUE HEMOS CAMBIADO ESTO
		0.0, 1.0, 0.0); //PARA MIRAR AL CENTRO DE LA ESCENA

	glPushMatrix();
	glTranslatef(-4.0f, 0.0f, -4.0f);  // Ajusta si tu tablero va de (0,0) a (8,8)
	tablerogl.dibuja(filas_, columnas_); // Llama al método de dibujo del tablero
	glPopMatrix();
	pieza.ubica( tablero, filas_, columnas_);
}

void Tablero::matriz() {
	int** tablero;
}