#pragma once
#include "vector2d.h"

class ClassMundo;

class raton {
public:
    // Constructor (opcional si quieres inicializar cosas en el futuro)
    raton() {}

    // Método para procesar el ratón
    void click(int button, int state, int x, int y, class ClassMundo* mundo);

    void mouse(int button, int state, int x, int y, int filas, int columnas, int caso, class ClassMundo* mundo);

};//raton.h

