#pragma once

#include <vector>
#include "Pieza.h"
#include "Tablero.h"
#include "ETSIDI.h"


class ClassPeon : public ClassPieza{  

	ETSIDI::SpriteSequence sprite{ "Imagenes/soldier1.png",4 };
	ETSIDI::SpriteSequence sprite2{ "Imagenes/peonRojo.png",4 };

public: 
	ClassPeon(Color color, Vector2D posicion);
	ClassPeon() = default;

	vector<Vector2D> obtenerMovimientosPosibles(const ClassTablero& tablero) const;
	void dibuja(float x, float y);
	void anima();
        void mueve(const Vector2D* n, const vector<Vector2D> s)override;
        
};
