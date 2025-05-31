#pragma once
#include"Tablero.h"
#include "vector2d.h"
#include "raton.h"
#include "mundo.h"
#include "ETSIDI.h"
#include "MENU.h"
#include <vector>

class Coordinador {
public:
    enum EstadoJuego { MENU, JUEGO, GAMEOVER, YOUWIN };
    EstadoJuego estado;
    ClassMundo* ObjMundo = nullptr; //Puntero a la clase que contiene el mundo
    int TipoJuego;
    int Lugar;
    int TipoTablero;
    int nivel;
    Menu menu;

   
    Coordinador();              // <-- Constructor
    ~Coordinador();             // <-- Destructor


    void tecla_especial(unsigned char key);
    void tecla(unsigned char key);
    void mueve();
    void dibuja();

};