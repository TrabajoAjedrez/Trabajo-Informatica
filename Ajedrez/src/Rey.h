#pragma once

#include <vector>
#include "Pieza.h"
#include "Tablero.h"
#include "ETSIDI.h"
#include "Reglas.h"


class ClassRey : public ClassPieza {

	ETSIDI::SpriteSequence sprite{ "Imagenes/reyAzul2.png",4 };
	ETSIDI::SpriteSequence sprite2{ "Imagenes/reyRojo.png",4 };
	ClassReglas* reglas;

public:
	ClassRey(Color color, Vector2D posicion);
	ClassRey() = default;

	void dibuja(float x, float y);
	void anima();
	vector<Vector2D> obtenerMovimientosPosibles(const ClassTablero& tablero) const override;

	ClassPieza* clonar() const override;

	//destructor rey
	~ClassRey() override {};
};
