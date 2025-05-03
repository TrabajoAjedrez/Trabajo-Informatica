#pragma once

#include <vector>
#include "Pieza.h"
#include "Tablero.h"

class ClassPeon : public ClassPieza{  

public: 
	ClassPeon(Color color, Vector2D posicion);
	vector<Vector2D> obtenerMovimientosPosibles(const ClassTablero& tablero) const;

};
