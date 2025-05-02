#pragma once

#include <vector>
#include "pieza.h"
#include "ClassTablero.h"

class Peon : public Pieza{  

public: 
	Peon(Color color, Vector2D posicion);
	vector<Vector2D> obtenerMovimientosPosibles(const Tablero& tablero) const;

};
