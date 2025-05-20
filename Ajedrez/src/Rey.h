#pragma once

#include <vector>
#include "Pieza.h"
#include "Tablero.h"
#include "ETSIDI.h"


class ClassRey : public ClassPieza {

	ETSIDI::SpriteSequence sprite{ "Imagenes/reyAzul.png",4 };
	ETSIDI::SpriteSequence sprite2{ "Imagenes/reyRojo.png",4 };

public:
	ClassRey(Color color, Vector2D posicion);
	ClassRey() = default;

	void dibuja(float x, float y);
	void anima();
};
