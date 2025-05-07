#pragma once

#include <vector>
#include "freeglut.h"
#include "Pieza.h"
//#include "Peon.h"

class ClassTablero {
private:
    int filas_; // Numero de filas del tablero
    int columnas_; // Numero de columnas del tablero
	std::vector<std::vector<ClassPieza*>> tablero; // Tablero de piezas

    float tamCasilla;       // Tamaño de cada casilla
    float posX, posY, posZ; // Posición del tablero en el espacio

    //ClassPieza ObjPieza; // Objeto de la clase pieza

    // Opciones para el tablero
    //std::vector<std::vector<ClassPieza*>> piezas_Clean; 
    //ClassPieza*** piezas_NoClean;

    // Colores para las casillas (pueden personalizarse)
    GLfloat colorClaro[3];
    GLfloat colorOscuro[3];

public:
    // Constructor: tamaño del tablero y tamaño de cada casilla
    ClassTablero(int filas, int columnas, float tamCasilla = 1.0f) :
		filas_(filas), columnas_(columnas), tamCasilla(tamCasilla), posX(0), posY(0), posZ(0)
    {
		// Inicializar el tablero
        tablero.resize(filas_, std::vector<ClassPieza*>(columnas_, nullptr));

        colorClaro[0] = 0.96f; colorClaro[1] = 0.96f; colorClaro[2] = 0.86f;
        colorOscuro[0] = 0.55f; colorOscuro[1] = 0.27f; colorOscuro[2] = 0.07f;
    }
	// Métodos para obtener información del tablero
	int getFilas() const { return filas_; }
	int getColumnas() const { return columnas_; }
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
    // Imprime cosas en pantalla
    void ImprimirEnPantalla();
    // Animar piezas
    void AnimaPiezas();
	// Metodos para comprobar posiciones
    bool esPosicionValida(const Vector2D& pos) const;
    bool estaOcupada(const Vector2D& pos) const;
};

