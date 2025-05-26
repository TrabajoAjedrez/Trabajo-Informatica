#include "Rey.h"
#include "ETSIDI.h"

ClassRey::ClassRey(Color color, Vector2D posicion)
	: ClassPieza(Pieza_t::Rey, color, posicion)
{
	sprite.setCenter(1, 1);
	sprite.setSize(1, 1);
	sprite2.setCenter(1, 1);
	sprite2.setSize(1, 1);
}
void ClassRey::dibuja(float x, float y) {
	glPushMatrix();
	glTranslated(x, y, 0.1);
	if (color == Color::AZUL)
		sprite.draw();
	else
		sprite2.draw();
	glPopMatrix();
}

void ClassRey::anima() {
	if (color == Color::AZUL) {
		if (sprite.getState() == 0)
			sprite.setState(0, false);
		sprite.draw();
	}
	else {
		if (sprite2.getState() == 0)
			sprite2.setState(0, false);
		sprite2.draw();
	}
	sprite.loop();
	sprite2.loop();
}

vector<Vector2D> ClassRey::obtenerMovimientosPosibles(const ClassTablero& tablero) const {  
 vector<Vector2D> movimientos;  
 vector<Vector2D> movsPosibles = { {1, -1}, {1, 0}, {1, 1}, {0, -1}, {0, 1}, {-1, -1}, {-1, 0}, {-1, 1} };  
 bool amenaza = false;  
 for (const auto& movimiento : movsPosibles) {  
     Vector2D destino = pos + movimiento;  
     amenaza = reglas->PosAmenzada(destino, tablero, const_cast<ClassRey*>(this));  //El rey no puede ir a una posicion amenazada
     if (tablero.esPosicionValida(destino) && !amenaza)  
         movimientos.push_back(destino);  
 }  
 return movimientos;  
}