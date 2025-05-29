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

	// Todas las combinaciones posibles del movimiento en L del caballo
	vector<Vector2D> posiblesMovimientos = {
		{2, 1}, {1, 2}, {-1, 2}, {-2, 1},
		{-2, -1}, {-1, -2}, {1, -2}, {2, -1}
	};

	for (const auto& movimiento : posiblesMovimientos) {
		Vector2D destino = pos + movimiento;
		if (tablero.esPosicionValida(destino))
			movimientos.push_back(destino);
	}
	return movimientos;
}

ClassPieza* ClassCaballo::clonar() const {
	return new ClassCaballo(*this);
}
