#pragma once
#include "vector2d.h"

class ClassMundo;

class raton {
public:
    // Constructor (opcional si quieres inicializar cosas en el futuro)
    raton() {}

    // M�todo para procesar el rat�n
    //void click(int button, int state, int x, int y, class ClassMundo* mundo);

    Vector2D mouse(int button, int state, int x, int y, int filas, int columnas, ClassMundo* mundo) const;



};//raton.h

