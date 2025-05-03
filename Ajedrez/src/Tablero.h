#pragma once

#include <vector>
#include "freeglut.h"
#include "Pieza.h"

class ClassTablero {
private:
    int filas_; // Numero de filas del tablero
    int columnas_; // Numero de columnas del tablero
	int** tablero; // Matriz que representa el tablero

    float tamCasilla;       // Tamaño de cada casilla
    float posX, posY, posZ; // Posición del tablero en el espacio

    ClassPieza ObjPieza; // Objeto de la clase pieza

    // Colores para las casillas (pueden personalizarse)
    GLfloat colorClaro[3];
    GLfloat colorOscuro[3];

public:
    // Constructor: tamaño del tablero y tamaño de cada casilla
    ClassTablero(int filas, int columnas, float tamCasilla = 1.0f) :
		filas_(filas), columnas_(columnas), tamCasilla(tamCasilla), posX(0), posY(0), posZ(0)
    {
		// Inicializar el tablero
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
	// Destructor: libera la memoria del tablero
    ~ClassTablero() {
        for (int i = 0; i < filas_; i++) {
            delete[] tablero[i];
        }
        delete[] tablero;
    }

	// Métodos para obtener información del tablero
	int getFilas() const { return filas_; }
	int getColumnas() const { return columnas_; }
    int** getTablero() const { return tablero; }
    // Dibuja el tablero
    void dibuja();
    // Establece la posición del tablero
    void setPosicion(float x, float y, float z);
    // Dibuja una casilla en la posicion (x,z)
    void dibujarCasilla(float x, float z, bool esClara);
    // Ubica las piezas en el tablero
    void UbicaPieza();
    // Coloca las piezas en el tablero
    void ColocarPiezas();
	// Anima las piezas en el tablero
    void AnimaPiezas();
    // Imprime cosas en pantalla
    void ImprimirEnPantalla();
};

