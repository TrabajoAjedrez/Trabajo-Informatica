#include "historialmovimientos.h"

std::deque<std::pair<Vector2D, Vector2D>> HistorialMovimientos::historial;

void HistorialMovimientos::registrarMovimiento(const Vector2D& origen, const Vector2D& destino) {
   historial.emplace_back(origen, destino);
   if (historial.size() > maxHistorial)
       historial.pop_front();
}

bool HistorialMovimientos::esRepetido(const Vector2D& origen, const Vector2D& destino) {
    return std::find(historial.begin(), historial.end(), std::make_pair(origen, destino)) != historial.end();
}
void HistorialMovimientos::limpiar() {
    historial.clear();
}