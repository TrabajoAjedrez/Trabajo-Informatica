#pragma once

#include <vector>
#include "Pieza.h"
#include "Tablero.h"

class Peon : public ClassPieza{  

public: 
	Peon(Color color, Vector2D posicion);
	vector<Vector2D> obtenerMovimientosPosibles(const ClassTablero& tablero) const;

};
