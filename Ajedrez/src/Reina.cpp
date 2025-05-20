#include "Reina.h"
#include "ETSIDI.h"

ClassReina::ClassReina(Color color, Vector2D posicion)
	: ClassPieza(Pieza_t::Reina, color, posicion)
{
	sprite.setCenter(1, 1);
	sprite.setSize(1, 1);
	sprite2.setCenter(1, 1);
	sprite2.setSize(1, 1);
}
void ClassReina::dibuja(float x, float y) {
	glPushMatrix();
	glTranslated(x, y, 0.1);
	if (color == Color::BLANCO)
		sprite.draw();
	else
		sprite2.draw();
	glPopMatrix();
}

void ClassReina::anima() {
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