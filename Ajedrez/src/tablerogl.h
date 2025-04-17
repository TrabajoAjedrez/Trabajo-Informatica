#pragma once
#include "freeglut.h"
#include "tablero.h"

class TableroGL {
private:

    float tamCasilla;       // Tamaño de cada casilla
    float posX, posY, posZ; // Posición del tablero

    // Colores para las casillas (pueden personalizarse)
    GLfloat colorClaro[3];
    GLfloat colorOscuro[3];

    // Dibuja una casilla individual
    void dibujarCasilla(float x, float z, bool esClara);
public:
    // Constructor: tamaño del tablero (normalmente 8x8) y tamaño de cada casilla
    TableroGL(float tamCasilla = 1.0f):
        tamCasilla(tamCasilla), posX(0), posY(0), posZ(0)
    {
        colorClaro[0] = 0.96f; colorClaro[1] = 0.96f; colorClaro[2] = 0.86f;
        colorOscuro[0] = 0.55f; colorOscuro[1] = 0.27f; colorOscuro[2] = 0.07f;
    }

    //TEMPORAL ESTO LUEGO LO CAMBIO
    
    // Dibuja el tablero en la posición actual
    void dibuja(int filas, int columnas, float tamCasilla = 1.0f);

    // Establece la posición del tablero
    void setPosicion(float x, float y, float z);

    // Obtiene el tamaño total del tablero
    float getTamanoTotal() const;


};

