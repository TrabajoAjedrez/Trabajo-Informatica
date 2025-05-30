#pragma once  
#include <iostream>  
#include "ETSIDI.h"  
#include <vector2d.h>  
#include <vector>  

class ClassTablero;  

using namespace std;  

class ClassPieza {  

public:  
   enum Pieza_t { Peon = 1, Caballo, Torre, Alfil, Reina, Rey };  
   enum class Color { ROJO, AZUL };  

protected:  
   Pieza_t tipo; // Tipo de pieza  
   Color color; // Bando de la pieza  
   Vector2D pos; // No se sabe si se va a utilizar  

public:  
   // Constructor - define el tipo, el color y la posicion de una pieza  
   ClassPieza(Pieza_t t, Color c, Vector2D p);  

   // M�todos para consultar las propiedades privadas  
   Pieza_t getTipo() const;  
   Color getColor() const;  
   Vector2D getPos() const; // No se sabe si se va a utilizar  
   // M�todos para modificar las propiedades privadas  
   void setTipo(Pieza_t t);  
   void setColor(Color c);  
   void setPos(Vector2D p);  
   virtual void dibuja(float x, float y) = 0; // en la clase base  
   virtual void anima() = 0;  
   virtual vector<Vector2D> obtenerMovimientosPosibles(const ClassTablero& tablero) const = 0;

   virtual ClassPieza* clonar() const = 0;

   virtual ~ClassPieza() = default;  

};