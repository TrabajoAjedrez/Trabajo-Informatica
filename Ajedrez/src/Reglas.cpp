#include <iostream>
#include "freeglut.h"
#include "Reglas.h"
#include "Tablero.h"
#include "Pieza.h"

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
		//std::cout << "Quedan " << tiempo_restante << " segundos" << std::endl;
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

bool ClassReglas::hayJaque(const ClassTablero& tablero, ClassPieza::Color colorRey) {
    Vector2D posRey{ -1, -1 };

    // Buscar la posición del rey del color dado
    for (int i = 0; i < tablero.getFilas(); ++i) {
        for (int j = 0; j < tablero.getColumnas(); ++j) {
            ClassPieza* pieza = tablero.getPieza({ i, j });
            if (pieza && pieza->getTipo() == ClassPieza::Pieza_t::Rey && pieza->getColor() == colorRey) {
                posRey = { i, j };
                break;
            }
        }
    }

    if (posRey.x == -1) return false; // Rey no encontrado

    // Verificar si alguna pieza enemiga puede atacar al rey
    for (int i = 0; i < tablero.getFilas(); ++i) {
        for (int j = 0; j < tablero.getColumnas(); ++j) {
            ClassPieza* pieza = tablero.getPieza({ i, j });
            if (pieza && pieza->getColor() != colorRey) {
                auto movimientos = pieza->obtenerMovimientosPosibles(tablero);
                for (const auto& mov : movimientos) {
                    if (mov == posRey) {
                        std::cout << "¡Jaque al rey " << (colorRey == ClassPieza::Color::AZUL ? "azul" : "rojo") << "!" << std::endl;
                        return true;
                    }
                }
            }
        }
    }

    return false;
}
