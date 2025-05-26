#pragma once
#pragma once

#include <vector>
#include "Pieza.h"
#include "Tablero.h"
#include "ETSIDI.h"


class ClassTorre : public ClassPieza {

	ETSIDI::SpriteSequence sprite{ "Imagenes/torreAzul.png",4 };
	ETSIDI::SpriteSequence sprite2{ "Imagenes/torreRoja.png",4 };

public:
	ClassTorre(Color color, Vector2D posicion);
	ClassTorre() = default;

	void dibuja(float x, float y);
	void anima();
	vector<Vector2D> obtenerMovimientosPosibles(const ClassTablero& tablero) const override;
};


