#pragma once
#include <deque>
#include <utility>
#include "vector2d.h"

class HistorialMovimientos {
private:
    static constexpr int maxHistorial = 30;
    static std::deque<std::pair<Vector2D, Vector2D>> historial;

public:
	// Metodos estáticos para manejar el historial de movimientos

    static void registrarMovimiento(const Vector2D& origen, const Vector2D& destino); 
    static bool esRepetido(const Vector2D& origen, const Vector2D& destino);
    static void limpiar();
};

