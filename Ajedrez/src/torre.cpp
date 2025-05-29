#include "torre.h"
#include "ETSIDI.h"

ClassTorre::ClassTorre(Color color, Vector2D posicion)
	: ClassPieza(Pieza_t::Torre, color, posicion)
{
	sprite.setCenter(1, 0.9);
	sprite.setSize(1, 1);
	sprite2.setCenter(1, 1);
	sprite2.setSize(1, 1);
}

std::vector<Vector2D> ClassTorre::obtenerMovimientosPosibles(const ClassTablero& tablero) const {
	std::vector<Vector2D> movimientos;
	int filas = tablero.getFilas();
	int columnas = tablero.getColumnas();

	// Direcciones: arriba, abajo, izquierda, derecha
	const int dx[] = { -1, 1, 0, 0 };
	const int dy[] = { 0, 0, -1, 1 };

	for (int dir = 0; dir < 4; ++dir) {
		int x = pos.x + dx[dir];
		int y = pos.y + dy[dir];

		while (x >= 0 && x < filas && y >= 0 && y < columnas) {
			Vector2D destino(x, y);

			if (!tablero.estaOcupada(destino)) {
				movimientos.push_back(destino); 
			}
			else {
				movimientos.push_back(destino); 
				break;  
			}

			x += dx[dir];
			y += dy[dir];
		}
	}

	return movimientos;
}

void ClassTorre::dibuja(float x, float y) {
	glPushMatrix();
	glTranslated(x, y, 0.1);
	if (color == Color::AZUL)
		sprite.draw();
	else
		sprite2.draw();
	glPopMatrix();
}

void ClassTorre::anima() {
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

ClassPieza* ClassTorre::clonar() const {
	return new ClassTorre(*this);
}