#pragma once

#include "Tablero.h"
#include "Reglas.h"
#include "vector2d.h"

class ValidadorDeMovimientos {
public:
    static bool esMovimientoLegal(const ClassTablero& tableroOriginal,const Vector2D& origen,const Vector2D& destino, ClassPieza::Color turno);
};
