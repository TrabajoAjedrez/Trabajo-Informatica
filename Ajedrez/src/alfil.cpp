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

vector<Vector2D> ClassAlfil::obtenerMovimientosPosibles(const ClassTablero& tablero) const  {

	vector<Vector2D> movimientos;

	for (int i=1; i < tablero.getColumnas()+1; i++) { // se puede mover en diagonal como columnas haya
		//calculo de posiciones
		Vector2D DiagSupDer{ i,i };
		Vector2D DiagSupIz{ i,-i };
		Vector2D DiagInfDer{ -i,i };
		Vector2D DiagInfIz{ -i,-i };

		//añadir los movimientos al vector
		if (tablero.esPosicionValida(pos + DiagSupDer))
			movimientos.push_back(pos + DiagSupDer);
		if (tablero.esPosicionValida(pos + DiagInfDer))
			movimientos.push_back(pos + DiagInfDer);
		if (tablero.esPosicionValida(pos + DiagSupIz))
			movimientos.push_back(pos + DiagSupIz);
		if (tablero.esPosicionValida(pos + DiagInfIz))
			movimientos.push_back(pos + DiagInfIz);
	}
	return movimientos;
}