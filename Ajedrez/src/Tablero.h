#pragma once
#include "freeglut.h"
#include "Mundo.h"
//#include "pieza.h"

class ClassTablero {
private:

    int filas_;
    int columnas_;

    int** tablero;

    float tamCasilla;       // Tamaño de cada casilla
    float posX, posY, posZ; // Posición del tablero

    // Colores para las casillas (pueden personalizarse)
    GLfloat colorClaro[3];
    GLfloat colorOscuro[3];

    // Dibuja una casilla individual
    void dibujarCasilla(float x, float z, bool esClara);
public:
    // Constructor: tamaño del tablero (normalmente 8x8) y tamaño de cada casilla
    ClassTablero(int filas, int columnas, float tamCasilla = 1.0f) :
		filas_(filas), columnas_(columnas), tamCasilla(tamCasilla), posX(0), posY(0), posZ(0)
    {
        tablero = new int* [filas];
        for (int i = 0; i < filas; i++) {
            tablero[i] = new int[columnas];
            for (int j = 0; j < columnas; j++) {
                tablero[i][j] = 0;
            }
        }
        colorClaro[0] = 0.96f; colorClaro[1] = 0.96f; colorClaro[2] = 0.86f;
        colorOscuro[0] = 0.55f; colorOscuro[1] = 0.27f; colorOscuro[2] = 0.07f;
    }

    ~ClassTablero() {
        for (int i = 0; i < filas_; i++) {
            delete[] tablero[i];
        }
        delete[] tablero;
    }

	int getFilas() const { return filas_; }
	int getColumnas() const { return columnas_; }

    //TEMPORAL ESTO LUEGO LO CAMBIO
    
    // Dibuja el tablero en la posición actual
    void dibuja();

    // Establece la posición del tablero
    void setPosicion(float x, float y, float z);

    // Obtiene el tamaño total del tablero
    float getTamanoTotal() const;

 //   /////
 //bool esPosicionValida(const Vector2D& posicion) const;

	//bool estaOcupada(const Vector2D& posicion) const;
};

