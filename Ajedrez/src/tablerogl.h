#pragma once
#include "freeglut.h"

class TableroGL {
public:
    // Constructor: tamaño del tablero (normalmente 8x8) y tamaño de cada casilla
    TableroGL(int dimension = 8, float tamCasilla = 1.0f);

    // Dibuja el tablero en la posición actual
    void dibuja();

    // Establece la posición del tablero
    void setPosicion(float x, float y, float z);

    // Obtiene el tamaño total del tablero
    float getTamanoTotal() const;

private:
    int dimension;          // Dimensión del tablero (8 para ajedrez estándar)
    float tamCasilla;       // Tamaño de cada casilla
    float posX, posY, posZ; // Posición del tablero

    // Colores para las casillas (pueden personalizarse)
    GLfloat colorClaro[3];
    GLfloat colorOscuro[3];

    // Dibuja una casilla individual
    void dibujarCasilla(float x, float z, bool esClara);
};

