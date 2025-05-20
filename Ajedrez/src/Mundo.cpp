#include <iostream>
#include <cassert>
#include "Mundo.h"
#include "freeglut.h"
#include "Reglas.h"

using namespace std;

ClassReglas reglas; 
static ClassMundo* mundoPtr = nullptr;


void ClassMundo::tecla(unsigned char key) {

}
void ClassMundo::tecla_especial(unsigned char key) {

}




void ClassMundo::inicializa() {

	mundoPtr = this; 

	int Variante = reglas.preguntar_variante();
	// Inicializa el tablero según la variante seleccionada

	auto dimensiones = reglas.devolver_forma(Variante);

	ObjTablero = new ClassTablero(dimensiones);


	// Inicializa la posicion de la camara, segun el tablero
	x_ojo = ObjTablero->getFilas() / 2;
	y_ojo = ObjTablero->getColumnas() / 2;
	z_ojo = ObjTablero->getFilas() * 2.0;
	// Se llama al tablero para que inicialice y coloque las piezas en la matriz del tablero
	ObjTablero->ColocarPiezas();
	// Imprimimos el tablero por pantalla
	ObjTablero->ImprimirEnPantalla();

	//runAllTests();


	//temporizador
	reglas.inicia_temporizador(8); // Por ejemplo, 8 segundos
	glutTimerFunc(1000, ClassMundo::onTimer, 0); // Arranca el temporizador

	//cambio de turno al final
	reglas.set_turno();
}

void ClassMundo::onTimer(int value) {
	if (mundoPtr) {
		mundoPtr->temporizador();
		if (reglas.get_tiempo_restante() > 0) {
			glutTimerFunc(1000, ClassMundo::onTimer, 0);
		}
	}
	glutPostRedisplay();
}

void ClassMundo::temporizador() {
	cout << "temporizador..." << endl;
	reglas.actualiza_tiempo();
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
		ObjTablero->getFilas() / 2, ObjTablero->getColumnas() / 2, 0, //NOTESE QUE HEMOS CAMBIADO ESTO
		0.0, 1.0, 0.0); //PARA MIRAR AL CENTRO DE LA ESCENA

	glPushMatrix();
	// Se dibuja el tablero
	ObjTablero->dibuja();
	glPopMatrix();
}

int ClassMundo::getFilas() const {
	return ObjTablero ? ObjTablero->getFilas() : 0;
}

int ClassMundo::getColumnas() const {
	return ObjTablero ? ObjTablero->getColumnas() : 0;

}