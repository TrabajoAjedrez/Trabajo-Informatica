#include <iostream>
#include "freeglut.h"
#include "Reglas.h"

using namespace std;

Vector2D ClassReglas::devolver_forma(const int& Variante) {
	switch (Variante)
	{
	case 1:
		return ShapeSilverman;
		break;
	case 2:
		return ShapeDemi;
		break;
	default:
		return{ 0,0 };
		break;
	}
}

void ClassReglas::inicia_temporizador(int segundos) {
	//if()
	tiempo_restante = segundos;
}

void ClassReglas::actualiza_tiempo() {

	if (tiempo_restante > 0) {
		tiempo_restante--;
		std::cout << "Quedan " << tiempo_restante << " segundos" << std::endl;
	}

//	tiempo_inicial = glutGet(GLUT_ELAPSED_TIME); // Guarda el tiempo inicial
}

void ClassReglas::set_turno() {//cambio de turno
	turno_ = !turno_;
}

int ClassReglas::get_tiempo_restante() const {
	return tiempo_restante;
}

bool ClassReglas::get_turno() const {
	return turno_;
}