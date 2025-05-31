#pragma once
#include "vector2d.h"

class ClassMundo;

class raton {
public:
    // Constructor 
    raton() {}

    Vector2D mouse(int button, int state, int x, int y, int filas, int columnas, ClassMundo* mundo) const;

};