#include <iostream>
#include <cassert>
#include "Mundo.h"
#include "freeglut.h"
#include "Reglas.h"

using namespace std;

ClassReglas reglas; 
// Variables para controlar los temporizadores
int tiempo_inicial = 0;
const int INTERVALO_TEMPORIZADOR = 1000; // 1000ms

void ClassMundo::tecla(unsigned char key) {

}
void ClassMundo::tecla_especial(unsigned char key) {

}

void onTimer(int value) {
	reglas.actualiza_tiempo(); // Actualiza el tiempo restante
	glutTimerFunc(INTERVALO_TEMPORIZADOR, onTimer, 0); // Reprograma el temporizador
	glutPostRedisplay(); // Redibuja la ventana
}


void ClassMundo::inicializa(int Variante) {

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
	tiempo_inicial = glutGet(GLUT_ELAPSED_TIME); // Guarda el tiempo inicial
	reglas.inicia_temporizador(8); // 8s de prueba

	reglas.inicia_temporizador(8); // 8s de prueba
	int tiempo_actual = glutGet(GLUT_ELAPSED_TIME); // Obtener tiempo transcurrido

	// Actualizar temporizador cada segundo
	if (tiempo_actual - tiempo_inicial >= INTERVALO_TEMPORIZADOR) {
		reglas.actualiza_tiempo();
		tiempo_inicial = tiempo_actual;
	}

}
void ClassMundo::rotarOjo() {
	double dist = sqrt(x_ojo * x_ojo + z_ojo * z_ojo);
	double ang = atan2(z_ojo, x_ojo);
	ang += 0.01;
	x_ojo = dist * cos(ang);
	z_ojo = dist * sin(ang);
}

bool ClassMundo::turno(bool turn) {
	
	

	turn = !turn; //guardo el nuevo valor de la variable turno
	return turn; //para cambiar de turno
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