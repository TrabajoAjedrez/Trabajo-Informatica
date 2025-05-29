#include "alfil.h"
#include "ETSIDI.h"
#include "vector2d.h"

ClassAlfil::ClassAlfil(Color color, Vector2D posicion)
	: ClassPieza(Pieza_t::Alfil, color, posicion)
{
	sprite.setCenter(1, 1);
	sprite.setSize(0.8, 0.85);
	sprite2.setCenter(1, 1);
	sprite2.setSize(0.8, 0.85);
}
void ClassAlfil::dibuja(float x, float y) {
	glPushMatrix();
	glTranslated(x, y, 0.1);
	if (color == Color::AZUL)
		sprite.draw();
	else
		sprite2.draw();
	glPopMatrix();
}

void ClassAlfil::anima() {
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

vector<Vector2D> ClassAlfil::obtenerMovimientosPosibles(const ClassTablero& tablero) const {
	vector<Vector2D> movimientos;
	int filas = tablero.getFilas();
	int columnas = tablero.getColumnas();

	// Direcciones diagonales: ? ? ? ?
	const int dx[] = { 1, 1, -1, -1 };
	const int dy[] = { 1, -1, 1, -1 };

	for (int dir = 0; dir < 4; ++dir) {
		int x = pos.x + dx[dir];
		int y = pos.y + dy[dir];

		while (x >= 0 && x < filas && y >= 0 && y < columnas) {
			Vector2D destino(x, y);

			if (!tablero.esPosicionValida(destino)) break;

			movimientos.push_back(destino);

			if (tablero.estaOcupada(destino)) break;

			x += dx[dir];
			y += dy[dir];
		}
	}

	return movimientos;
}


