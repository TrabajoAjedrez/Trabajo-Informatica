#include "ValidadorDeMovimientos.h"
#include "pieza.h"
#include "Rey.h"

bool ValidadorDeMovimientos::esMovimientoLegal(const ClassTablero& tableroOriginal,
    const Vector2D& origen,
    const Vector2D& destino,
    ClassPieza::Color turno) {
    // Copia del tablero (asegúrate de que tienes constructor copia bien definido)
    ClassTablero tablero = tableroOriginal;

    // Obtener pieza a mover
    ClassPieza* pieza = tablero.getPieza(origen);
    if (!pieza || pieza->getColor() != turno) return false;

    // Mover la pieza en la copia
    tablero.moverPieza(origen, destino);

    // Verificar si el rey del jugador sigue en jaque
    ClassReglas reglas;
    return !reglas.hayJaque(tablero, turno);
}
