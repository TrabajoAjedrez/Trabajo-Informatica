#pragma once
#include"vector2d.h"

class raton {
private:
    Vector2D casilla;
    bool clickDI;//derecho: 1    izquierdo: 0
    bool tablero;//click dentro del tablero:1     fuera del tablero:0


public:

    // Constructor (opcional si quieres inicializar cosas en el futuro)
    raton() {}

    // Método para procesar el ratón
    void click(int button, int state, int x, int y);

    void mouse(int button, int state, int x, int y, int filas, int columnas, int caso);

};//raton.h



