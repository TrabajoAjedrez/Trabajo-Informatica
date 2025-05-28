#pragma once

#include <vector>
#include "Pieza.h"
#include "Tablero.h"
#include "ETSIDI.h"


class ClassCaballo : public ClassPieza {

	ETSIDI::SpriteSequence sprite{ "Imagenes/caballoAzul.png",4 };
	ETSIDI::SpriteSequence sprite2{ "Imagenes/caballoRojo.png",4 };

public:
	ClassCaballo(Color color, Vector2D posicion);
	ClassCaballo() = default;

	void dibuja(float x, float y);
	void anima();
	vector<Vector2D> obtenerMovimientosPosibles(const ClassTablero& tablero) const override;

	//destructor de caballo
	~ClassCaballo() override {}
};
