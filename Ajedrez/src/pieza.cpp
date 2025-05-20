#include"Pieza.h"
#include"vector2d.h"
#include"ETSIDI.h"
#include"freeglut.h"
#include <iostream>
#include <vector>

using namespace std;
// Constructor
ClassPieza::ClassPieza(Pieza_t t, Color c, Vector2D p) : tipo(t), color(c), pos(p) {}

// Métodos para consultar las propiedades privadas
ClassPieza::Pieza_t ClassPieza::getTipo() const { return tipo; }
ClassPieza::Color ClassPieza::getColor() const { return color; }
Vector2D ClassPieza::getPos() const { return pos; }

// Métodos para modificar las propiedades privadas
void ClassPieza::setTipo(Pieza_t t) { tipo = t; }
void ClassPieza::setColor(Color c) { color = c; }
void ClassPieza::setPos(Vector2D p) { pos = p; }





