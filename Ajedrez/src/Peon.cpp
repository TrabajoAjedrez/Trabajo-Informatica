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
		if (filaActual == filaInicial && tablero.getFilas()==8) { //si las filas son 8 estamso en demi
			Vector2D dobleAdelante(filaActual + 2 * direccion, columnaActual);
			if (tablero.esPosicionValida(dobleAdelante) && !tablero.estaOcupada(dobleAdelante)) {
				movimientos.push_back(dobleAdelante);
			}
		}
	}
	// Captura en diagonal izquierda
	Vector2D diagonalIzquierda(filaActual + direccion, columnaActual - 1);
	if (tablero.esPosicionValida(diagonalIzquierda) && tablero.esPiezaCapturable(diagonalIzquierda, getColor())) {
		movimientos.push_back(diagonalIzquierda);
	}

	// Captura en diagonal derecha
	Vector2D diagonalDerecha(filaActual + direccion, columnaActual + 1);
	if (tablero.esPosicionValida(diagonalDerecha) && tablero.esPiezaCapturable(diagonalDerecha, getColor())) {
		movimientos.push_back(diagonalDerecha);
	}

	// Lógica para Captura al Paso
	int filaParaEnPasante;
	if (color == ClassPieza::Color::ROJO) { // Peones ROJOS
		filaParaEnPasante = 4;
	}
	else { // Peones AZULES
		if (tablero.getFilas() == 8) filaParaEnPasante = 3;
		else if (tablero.getFilas() == 5) filaParaEnPasante = 0;
		else filaParaEnPasante = -1; // Valor inválido, no debería ocurrir si las variantes están bien definidas.
	}

	if (filaActual == filaParaEnPasante) {
		// Verificar columna izquierda para posible captura al paso
		Vector2D posibleVictimaIzqPos(filaActual, columnaActual - 1); // Posición de la pieza enemiga que saltó
		Vector2D casillaDestinoEnPassantIzq(filaActual + direccion, columnaActual - 1); // A dónde iría nuestro peón

		if (tablero.getCasillaObjetivoEnPasante() == casillaDestinoEnPassantIzq &&
			tablero.getColorPeonVulnerableEnPasante() != this->color) {

			// Verificamos que realmente haya un peón enemigo en la posición que correspondería
			ClassPieza* victima = tablero.getPieza(posibleVictimaIzqPos);
			if (victima && victima->getTipo() == ClassPieza::Peon && victima->getColor() == tablero.getColorPeonVulnerableEnPasante()) {
				movimientos.push_back(casillaDestinoEnPassantIzq);
			}
		}
		// Verificar columna derecha para posible captura al paso
		Vector2D posibleVictimaDerPos(filaActual, columnaActual + 1); // Posición de la pieza enemiga que saltó
		Vector2D casillaDestinoEnPassantDer(filaActual + direccion, columnaActual + 1); // A dónde iría nuestro peón

		if (tablero.getCasillaObjetivoEnPasante() == casillaDestinoEnPassantDer &&
			tablero.getColorPeonVulnerableEnPasante() != this->color) {
			ClassPieza* victima = tablero.getPieza(posibleVictimaDerPos);
			if (victima && victima->getTipo() == ClassPieza::Peon && victima->getColor() == tablero.getColorPeonVulnerableEnPasante()) {
				movimientos.push_back(casillaDestinoEnPassantDer);
			}
		}
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

ClassPieza* ClassPeon::clonar() const {
	return new ClassPeon(*this);
}