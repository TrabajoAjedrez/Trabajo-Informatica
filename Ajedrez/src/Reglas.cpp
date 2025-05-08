#include "Reglas.h"


//bool turno_siguiente(const bool turno) {
//	if (turno == false)
//		turno
//}

Vector2D ClassReglas::devolver_forma(const int& Variante) {
	switch (Variante)
	{
	case 1:
		return ShapeSilverman;
		break;
	case 2:
		return ShapeDemi;
		break;
	default:
		return{ 0,0 };
		break;
	}
}