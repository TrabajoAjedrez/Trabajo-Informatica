#include "tablero.h"
#include "freeglut.h"

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
	tablero.dibuja();                   // Llama al método de dibujo del tablero
	glPopMatrix();
}