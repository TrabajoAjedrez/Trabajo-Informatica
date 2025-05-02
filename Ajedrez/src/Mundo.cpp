#include <iostream>
#include "Mundo.h"
#include "freeglut.h"
#include"pieza.h"
#include "ClassTablero.h"

Tablero ObjTablero;
using namespace std;

void Mundo::tecla(unsigned char key) {

}
void Mundo::tecla_especial(unsigned char key) {

}
void Mundo::inicializa() {
	x_ojo = filas_/2;
	y_ojo =columnas_/2;
	z_ojo = filas_*2;
	
	pieza.coloca( tablero,  filas_,  columnas_ );
	pieza.print(tablero, filas_, columnas_);
}
void Mundo::rotarOjo() {
	double dist = sqrt(x_ojo * x_ojo + z_ojo * z_ojo);
	double ang = atan2(z_ojo, x_ojo);
	ang += 0.01;
	x_ojo = dist * cos(ang);
	z_ojo = dist * sin(ang);
}
void Mundo::mueve() {
	pieza.muevePeon();



}
void Mundo::dibuja() {
	gluLookAt(x_ojo, y_ojo, z_ojo,
		filas_/2, columnas_/2, 0, //NOTESE QUE HEMOS CAMBIADO ESTO
		0.0, 1.0, 0.0); //PARA MIRAR AL CENTRO DE LA ESCENA

	glPushMatrix();
	ObjTablero.dibuja(filas_, columnas_); // Llama al método de dibujo del tablero
	glPopMatrix();
	pieza.ubica( tablero, filas_, columnas_);
}

void Mundo::matriz() {
	int** tablero;



}