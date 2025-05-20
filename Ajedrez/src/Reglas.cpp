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

void ClassReglas::set_turno() {

	turno_ = !turno_; //guardo el nuevo valor de la variable turno
}

void ClassReglas::inicia_temporizador(int segundos) {

	cout << "inicia temporizador" << endl;
	auto turno = ClassReglas::get_turno();
	
	if(turno==1)
		tiempo_restante_blancas = segundos;
	else if (turno == 0)
		tiempo_restante_blancas = segundos;

	}

void ClassReglas::actualiza_tiempo() {

	if (turno_ == 1) {
		if (tiempo_restante_blancas > 0) {
			tiempo_restante_blancas--;
			std::cout << "Quedan " << tiempo_restante_blancas << " segundos para las blancas" << std::endl;
		}
	}
	if (turno_ == 0) {
		if (tiempo_restante_negras > 0) {
			tiempo_restante_negras--;
			std::cout << "Quedan " << tiempo_restante_blancas << " segundos para las negras" << std::endl;
		}
	}
	
}

int ClassReglas::get_tiempo_restante() const {
	if (turno_ == 1)
		return tiempo_restante_blancas;
	else if (turno_ == 0)
		return tiempo_restante_negras;
}