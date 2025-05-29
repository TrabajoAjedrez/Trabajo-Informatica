#pragma once

#include <vector>
#include "Pieza.h"
#include "Tablero.h"
#include "ETSIDI.h"


class ClassReina : public ClassPieza {

	ETSIDI::SpriteSequence sprite{ "Imagenes/reinaAzul.png",4 };
	ETSIDI::SpriteSequence sprite2{ "Imagenes/reinaRoja.png",4 };

public:
	ClassReina(Color color, Vector2D posicion);
	ClassReina() = default;

	void dibuja(float x, float y);
	void anima();
	vector<Vector2D> obtenerMovimientosPosibles(const ClassTablero& tablero) const override;

	ClassPieza* clonar() const override;

	//destructor reina
	~ClassReina() override {}
};
