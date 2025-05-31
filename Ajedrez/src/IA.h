#pragma once
#include <vector>
#include "vector2d.h"
#include "Tablero.h"
#include "Reglas.h"
#include "ValidadorDeMovimientos.h"
#include <random>

enum class TipoIA {
    Agresiva = 0,
    Defensiva = 1,
    Tactica = 2,
    Aleatoria = 3,
	Adaptativa = 4,
};
class ClassTablero;
class ClassReglas;

class IA {
private:
    
    TipoIA tipo;
    
public:
	// Constructor 
    IA(TipoIA tipo_) : tipo(tipo_) {}

	// Constructor por defecto que elige una estrategia aleatoria
    IA() {tipo = elegirEstrategiaAleatoria();}

    static TipoIA elegirEstrategiaAleatoria();
    bool ejecutarMovimiento(ClassTablero* tablero, ClassReglas& reglas);

private:
	// Métodos para cada tipo de movimiento
    bool movimientoAgresivo(ClassTablero* tablero, ClassReglas& reglas);
    bool movimientoDefensivo(ClassTablero* tablero, ClassReglas& reglas);
    bool movimientoTactico(ClassTablero* tablero, ClassReglas& reglas);
    bool movimientoAleatorio(ClassTablero* tablero, ClassReglas& reglas);
    bool movimientoAdaptativo(ClassTablero* tablero, ClassReglas& reglas);

	// Métodos para evaluar movimientos sin ejecutarlos
    int evaluarMovimientoAgresivo(ClassTablero* tablero, ClassReglas& reglas);
    int evaluarMovimientoDefensivo(ClassTablero* tablero, ClassReglas& reglas);
    int evaluarMovimientoTactico(ClassTablero* tablero, ClassReglas& reglas);
    bool capturaPropiaVentajosa(ClassTablero* tablero, ClassReglas& reglas, const Vector2D& origen, const Vector2D& destino);
};

