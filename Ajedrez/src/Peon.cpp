#include "Peon.h"
#include "ETSIDI.h"
#include "Reglas.h" //para la promocion

using namespace std;
ClassPeon::ClassPeon(Color color, Vector2D posicion) 
	: ClassPieza(Pieza_t::Peon, color, posicion) 
{
	sprite.setCenter(1, 1);
	sprite.setSize(1, 1);
	sprite2.setCenter(1, 1);
	sprite2.setSize(1, 1);
}

vector<Vector2D> ClassPeon::obtenerMovimientosPosibles(const ClassTablero& tablero) const{
	
	vector<Vector2D> movimientos;
	int direccion;
	if (color == Color::AZUL) {
		direccion = -1;
	}
	else {
		direccion = 1;
	}
	int filaActual = pos.x;
	int columnaActual = pos.y;

	// Movimiento hacia adelante
	Vector2D adelante(filaActual + direccion, columnaActual);
	if (tablero.esPosicionValida(adelante) && !tablero.estaOcupada(adelante)) {
		movimientos.push_back(adelante);	

		// Movimiento doble desde la posición inicial
		int filaInicial = (color == Color::AZUL) ? tablero.getFilas() - 2 : 1;
		if (filaActual == filaInicial) {
			Vector2D dobleAdelante(filaActual + 2 * direccion, columnaActual);
			if (tablero.esPosicionValida(dobleAdelante) && !tablero.estaOcupada(dobleAdelante)) {
				movimientos.push_back(dobleAdelante);
			}
		}
	}
	// Captura en diagonal izquierda
	Vector2D diagonalIzquierda(filaActual + direccion, columnaActual - 1);
	if (tablero.esPosicionValida(diagonalIzquierda) && tablero.esPiezaCapturable(diagonalIzquierda)) {
		movimientos.push_back(diagonalIzquierda);
	}

	// Captura en diagonal derecha
	Vector2D diagonalDerecha(filaActual + direccion, columnaActual + 1);
	if (tablero.esPosicionValida(diagonalDerecha) && tablero.esPiezaCapturable(diagonalDerecha)) {
		movimientos.push_back(diagonalDerecha);
	}

	return movimientos;
}

void ClassPeon::dibuja(float x, float y) {
	glPushMatrix();
	glTranslated(x, y, 0.1);
	if (color == Color::AZUL)
		sprite.draw();
	
	else
		sprite2.draw();
	glPopMatrix();
}

void ClassPeon::anima() {
	if (color == Color::AZUL) {
		if (sprite.getState() == 0)
			sprite.setState(0, false);
		sprite.draw();
	}
	else {
		if (sprite2.getState() == 0)
			sprite2.setState(0, false);
		sprite2.draw();
	}
	sprite.loop();
	sprite2.loop();
}
