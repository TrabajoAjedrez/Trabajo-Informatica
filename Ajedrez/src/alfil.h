
#pragma once

#include <vector>
#include "Pieza.h"
#include "Tablero.h"
#include "ETSIDI.h"


class ClassAlfil : public ClassPieza {

	ETSIDI::SpriteSequence sprite{ "Imagenes/alfilAzul.png",4 };
	ETSIDI::SpriteSequence sprite2{ "Imagenes/alfilRojo.png",4 };

public:
	ClassAlfil(Color color, Vector2D posicion);
	ClassAlfil() = default;

	void dibuja(float x, float y);
	void anima();
	vector<Vector2D> obtenerMovimientosPosibles(const ClassTablero& tablero) const override;

	ClassPieza* clonar() const override;

	//destructor alfil
	~ClassAlfil() override{}
};
