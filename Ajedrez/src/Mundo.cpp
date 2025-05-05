#include <iostream>
#include <cassert>
#include "Mundo.h"
#include "freeglut.h"
#include "Peon.h"

using namespace std;

void ClassMundo::tecla(unsigned char key) {

}
void ClassMundo::tecla_especial(unsigned char key) {

}
void ClassMundo::inicializa(int Variante) {

	// Inicializa el tablero según la variante seleccionada
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

// Metodos para tests (No hacer mucho caso tampoco)
// Test 1: Peón blanco desde posición inicial
void ClassMundo::testPeonMovimientoBasico() {
	ClassTablero tablero(8, 4);
	ClassPeon peon(ClassPieza::Color::BLANCO, Vector2D(2, 2));

	int** t = tablero.getTablero();
	for (int i = 0; i < tablero.getFilas(); ++i)
		for (int j = 0; j < tablero.getColumnas(); ++j)
			t[i][j] = 0;

	auto movs = peon.obtenerMovimientosPosibles(tablero);

	assert(movs.size() == 2);
	std::cout << "testPeonMovimientoBasico passed\n";
}
// Test 2: Peón bloqueado
void ClassMundo::testPeonBloqueado() {
	ClassTablero tablero(8, 8);
	ClassPeon peon(ClassPieza::Color::BLANCO, Vector2D(4, 6));

	tablero.getTablero()[3][2] = 1;

	auto movs = peon.obtenerMovimientosPosibles(tablero);
	assert(movs.size() == 0);

	std::cout << " testPeonBloqueado passed\n";
}
void ClassMundo::runAllTests() {
	testPeonMovimientoBasico();
	//testPeonBloqueado();
}