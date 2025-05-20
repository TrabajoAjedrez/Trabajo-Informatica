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
void ClassTorre::dibuja(float x, float y) {
	glPushMatrix();
	glTranslated(x, y, 0.1);
	if (color == Color::BLANCO)
		sprite.draw();
	else
		sprite2.draw();
	glPopMatrix();
}

void ClassTorre::anima() {
	if (color == Color::BLANCO) {
		if (sprite.getState() == 0)
			sprite.setState(0, false);
		sprite.draw();
	}
	else {
		if (sprite.getState() == 0)
			sprite2.setState(0, false);
		sprite.draw();
	}
	sprite.loop();
	sprite2.loop();
}