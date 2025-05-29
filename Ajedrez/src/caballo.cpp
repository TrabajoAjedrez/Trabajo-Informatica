#include "caballo.h"
#include "ETSIDI.h"

ClassCaballo::ClassCaballo(Color color, Vector2D posicion)
	: ClassPieza(Pieza_t::Caballo, color, posicion)
{
	sprite.setCenter(1, 1);
	sprite.setSize(1, 1);
	sprite2.setCenter(1, 1);
	sprite2.setSize(1, 1);
}
void ClassCaballo::dibuja(float x, float y) {
	glPushMatrix();
	glTranslated(x, y, 0.1);
	if (color == Color::AZUL)
		sprite.draw();
	else
		sprite2.draw();
	glPopMatrix();
}

void ClassCaballo::anima() {
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


vector<Vector2D> ClassCaballo::obtenerMovimientosPosibles(const ClassTablero& tablero) const {
	vector<Vector2D> movimientos;
	const int dx[] = { 2, 1, -1, -2, -2, -1, 1, 2 }; //asi queda mejor la diferencia entre la casilla objetivo y en la que está
	const int dy[] = { 1, 2, 2, 1, -1, -2, -2, -1 };

	for (int i = 0; i < 8; ++i) {
		int x = pos.x + dx[i];
		int y = pos.y + dy[i];
		Vector2D destino(x, y);

		if (tablero.esPosicionValida(destino)) {

			movimientos.push_back(destino);
		}
	}

	return movimientos;
}

ClassPieza* ClassCaballo::clonar() const {
	return new ClassCaballo(*this);
}
