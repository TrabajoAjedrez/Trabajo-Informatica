#pragma once

#include <vector>
#include "freeglut.h"
#include "Pieza.h"
#include "Casilla.h"


class ClassTablero {
private:
    size_t filas_; // Numero de filas del tablero
    size_t columnas_; // Numero de columnas del tablero
	std::vector<std::vector<ClassPieza*>> tablero; // Tablero de piezas

    //float tamCasilla;       // Tama�o de cada casilla
    float posX, posY, posZ; // Posici�n del tablero en el espacio

    std::vector<std::vector<ClassCasilla>> casillasVisuales;

    Vector2D casillaObjetivoEnPasante_ = { -1, -1 }; // Casilla a la que se mover�a el pe�n
    ClassPieza::Color colorPeonVulnerableEnPasante_; // Color del pe�n que avanz� dos casillas

public:
    // Constructor: tama�o del tablero y tama�o de cada casilla
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
        colorPeonVulnerableEnPasante_ = ClassPieza::Color::AZUL;
    }
    
    ClassTablero(const ClassTablero& otro); // Constructor copia

	// M�todos para obtener informaci�n del tablero
	size_t getFilas() const { return filas_; }
	size_t getColumnas() const { return columnas_; }
    float getPosX() const { return posX; }
    float getPosZ() const { return posZ; }

    // Dibuja el tablero
    void dibuja(int TipoTablero);
    void dibujaBordeMedieval(float anchoCasillas, float altoCasillas, float grosorDelBorde, float zOffsetMarco);
    // Establece la posici�n del tablero
    void setPosicion(float x, float y, float z);
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
    bool moverPieza(const Vector2D& origen, const Vector2D& destino);
    ClassPieza* getPieza(const Vector2D& pos) const;
    // Verifica si una casilla est� dentro de los l�mites del tablero
    bool estaDentro(const Vector2D& casilla) const;

    //limpiar tablero
    void clear();
    //reiniciar partida
    void reset();

    void resaltarMovimientos(const std::vector<Vector2D>& movimientos);
    void limpiarResaltados();
    
    void dibujarExclamacionSobreRey(const Vector2D& posRey, ClassPieza::Color color, float tiempoRebote);


    bool esPiezaCapturable(const Vector2D& pos, ClassPieza::Color color) const;

    //creacion de piezas en promocion
    void promocionarPieza(const ClassPieza& pieza,char seleccion, int var);


    Vector2D getCasillaObjetivoEnPasante() const { return casillaObjetivoEnPasante_; }
    ClassPieza::Color getColorPeonVulnerableEnPasante() const { return colorPeonVulnerableEnPasante_; }
    void setCasillaEnPasante(const Vector2D& casilla, ClassPieza::Color colorPeonVulnerable);
    void limpiarCasillaEnPasante();

    //DESTRUCTOR
    ~ClassTablero();
};

