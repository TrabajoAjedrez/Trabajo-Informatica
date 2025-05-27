#pragma once
#include "Color.h"
#include "pieza.h"

class ClassCasilla {
	bool esClara;
	static float tamCasilla;
	bool resaltada = false;

	ClassPieza* pieza; //Las casillas ahora contienen piezas

public:
	ClassCasilla(bool clara = true) : esClara(clara) {}

	static void setTamCasilla(float tam) { tamCasilla = tam; }
	static float getTamCasilla() { return tamCasilla; }

	void dibujar(float x, float z) const;

	void setResaltada(bool valor) { resaltada = valor; }
	bool estaResaltada() const { return resaltada; }
};


