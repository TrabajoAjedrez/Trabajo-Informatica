#include "Peon.h"

Peon::Peon(Color color, Vector2D posicion) 
	: Pieza(Pieza_t::Peon, color, posicion) 
{}

vector<Vector2D> Peon::obtenerMovimientosPosibles(const ClassTablero& tablero) const {
	
	vector<Vector2D> movimientos;
	int direccion;
	if (color == Color::BLANCO) {
		direccion = -1;
	}
	else {
		direccion = 1;
	}
	int filaActual = pos.y;
	int columnaActual = pos.x;
	int filaInicial=2;

	// Movimiento hacia adelante
	Vector2D adelante(columnaActual, filaActual + direccion);
	if (tablero.esPosicionValida(adelante) && !tablero.estaOcupada(adelante)) {
		movimientos.push_back(adelante);

		// Movimiento doble desde la posición inicial
		if (filaActual == filaInicial) {
			Vector2D dobleAdelante(columnaActual, filaActual + 2 * direccion);
			if (tablero.esPosicionValida(dobleAdelante) && !tablero.estaOcupada(dobleAdelante)) {
				movimientos.push_back(dobleAdelante);
			}
		}
	}

	// Captura en diagonal izquierda
	Vector2D diagonalIzquierda(columnaActual - 1, filaActual + direccion);
	if (tablero.esPosicionValida(diagonalIzquierda) && tablero.estaOcupada(diagonalIzquierda)) {
		movimientos.push_back(diagonalIzquierda);
	}

	// Captura en diagonal derecha
	Vector2D diagonalDerecha(columnaActual + 1, filaActual + direccion);
	if (tablero.esPosicionValida(diagonalDerecha) && tablero.estaOcupada(diagonalDerecha)) {
		movimientos.push_back(diagonalDerecha);
	}

	return movimientos;
}


