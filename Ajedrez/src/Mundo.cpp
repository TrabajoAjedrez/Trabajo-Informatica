#include <iostream>
#include <cassert>
#include "Mundo.h"
#include "freeglut.h"
#include "Reglas.h"

using namespace std;

ClassReglas reglas;

void ClassMundo::tecla(unsigned char key) {

}
void ClassMundo::tecla_especial(unsigned char key) {

}
void ClassMundo::inicializa(int Variante) {

	// Inicializa el tablero según la variante seleccionada

	auto dimensiones = reglas.devolver_forma(Variante);

	ObjTablero = new ClassTablero(dimensiones);

	/*switch (Variante) 
	{
	case 1: 
		ObjTablero = new ClassTablero(4, 5); 
		break;
	case 2: 
		ObjTablero = new ClassTablero(8, 4); 
		break;
	default:
		 break;
	}*/

	// Inicializa la posicion de la camara, segun el tablero
	x_ojo = ObjTablero->getFilas() / 2;
	y_ojo = ObjTablero->getColumnas() / 2;
	z_ojo = ObjTablero->getFilas() * 2.0;
	// Se llama al tablero para que inicialice y coloque las piezas en la matriz del tablero
	ObjTablero->ColocarPiezas();
	// Imprimimos el tablero por pantalla
	ObjTablero->ImprimirEnPantalla();

	//runAllTests();

}
void ClassMundo::rotarOjo() {
	double dist = sqrt(x_ojo * x_ojo + z_ojo * z_ojo);
	double ang = atan2(z_ojo, x_ojo);
	ang += 0.01;
	x_ojo = dist * cos(ang);
	z_ojo = dist * sin(ang);
}
void ClassMundo::mueve() {
	// Se llama al tablero para que animar las piezas
	ObjTablero->AnimaPiezas();
}

void ClassMundo::dibuja() {
	gluLookAt(x_ojo, y_ojo, z_ojo,
		ObjTablero->getFilas()/2, ObjTablero->getColumnas()/2, 0, //NOTESE QUE HEMOS CAMBIADO ESTO
		0.0, 1.0, 0.0); //PARA MIRAR AL CENTRO DE LA ESCENA

	glPushMatrix();
	// Se dibuja el tablero
	ObjTablero->dibuja();
	glPopMatrix();
}