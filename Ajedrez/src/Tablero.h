#pragma once

#include <vector>
#include "freeglut.h"
#include "Pieza.h"
#include "Casilla.h"

class ClassTablero {
private:
    int filas_; // Numero de filas del tablero
    int columnas_; // Numero de columnas del tablero
	std::vector<std::vector<ClassPieza*>> tablero; // Tablero de piezas

    //float tamCasilla;       // Tamaño de cada casilla
    float posX, posY, posZ; // Posición del tablero en el espacio

    std::vector<std::vector<ClassCasilla>> casillasVisuales;

public:
    // Constructor: tamaño del tablero y tamaño de cada casilla
    ClassTablero(Vector2D dim) :
		columnas_(dim.x), filas_(dim.y), posX(0), posY(0), posZ(0)
    {
        ClassCasilla::setTamCasilla(1.0f);
		// Inicializar el tablero
        tablero.resize(filas_, std::vector<ClassPieza*>(columnas_, nullptr));
        casillasVisuales.resize(filas_, std::vector<ClassCasilla>(columnas_));
        for (int i = 0; i < filas_; ++i) {
            for (int j = 0; j < columnas_; ++j) {
                bool clara = (i + j) % 2 == 0;
                casillasVisuales[i][j] = ClassCasilla(clara);
            }
        }
    }
    
	// Métodos para obtener información del tablero
	int getFilas() const { return filas_; }
	int getColumnas() const { return columnas_; }
    float getPosX() const { return posX; }
    float getPosZ() const { return posZ; }

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

    ////
    //void moverPieza();
    void moverPieza(const Vector2D& origen, const Vector2D& destino);
    ClassPieza* getPieza(const Vector2D& pos) const;
    // Verifica si una casilla está dentro de los límites del tablero
    bool estaDentro(const Vector2D& casilla) const;

    //limpiar tablero
    void clear();
    //reiniciar partida
    void reset();

    void resaltarMovimientos(const std::vector<Vector2D>& movimientos);
    void limpiarResaltados();


    //DESTRUCTOR
    ~ClassTablero();
};

