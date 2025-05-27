#include "Tablero.h"
#include "freeglut.h"
#include "Peon.h"
#include "Reina.h"
#include "Rey.h"
#include "torre.h"
#include "caballo.h"
#include "alfil.h"

void ClassTablero::dibuja() {
 
	// Colores para las casillas (pueden personalizarse)
    glPushMatrix();
	glTranslatef(posX, posY, posZ); // Trasladar el tablero a la posición deseada
	// Bucle para dibujar cada casilla del tablero
    float tamCasilla = ClassCasilla::getTamCasilla();
    for (int i = 0; i < filas_; i++) {
        for (int j = 0; j < columnas_; j++) {
            int filaVisual = getFilas() - 1 - i;

            float x = j * tamCasilla;
            float z = filaVisual * tamCasilla;
            casillasVisuales[i][j].dibujar(x, z);
        }
    }
	// Luego de dibujar el tablero, dibujamos y ubicamos el dibujo de las piezas ¡¡Unicamente en el espacio 2D del tablero, pero no en la matriz!!
    UbicaPieza();
    // Dibujar borde del tablero
    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, 0.01f, 0);
    glVertex3f(filas_ * tamCasilla, 0.01f, 0);
    glVertex3f(filas_ * tamCasilla, 0.01f, filas_ * tamCasilla);
    glVertex3f(0, 0.01f, filas_ * tamCasilla);
    glEnd();

    glPopMatrix();
}

// Establece la posición del tablero en el espacio
void ClassTablero::setPosicion(float x, float y, float z) {
    posX = x;
    posY = y;
    posZ = z;
} 

// Dibuja las piezas en el tablero
void ClassTablero::UbicaPieza() {
    for (int i = 0; i < filas_; ++i) {
        for (int j = 0; j < columnas_; ++j) {

            //float x = (columnas_ - j);
           // float z = (filas_ - i);

            float x = j+1;
            float z = (filas_-i);

            if (tablero[i][j] != nullptr)
                tablero[i][j]->dibuja(x, z);  // método que llamará al sprite según color o tipo
        }
    }
}

// Pone los -1 y 1 o lo numeros que representan una pieza en la matriz del tablero
void ClassTablero::ColocarPiezas() {
    //colocar peones
    for (int j = 0; j < columnas_; ++j) {
        tablero[1][j] = new ClassPeon(ClassPieza::Color::ROJO, Vector2D(1, j));
        tablero[filas_ - 2][j] = new ClassPeon(ClassPieza::Color::AZUL, Vector2D(filas_ - 2, j));
    }
    //colocar reinas
    if (filas_ == 5) {
        tablero[0][1] = new ClassReina(ClassPieza::Color::ROJO, Vector2D(0, 1));
        tablero[4][1] = new ClassReina(ClassPieza::Color::AZUL, Vector2D(4,1));
    }
    //colocar reyes
    if (filas_ == 5) {
        tablero[0][2] = new ClassRey(ClassPieza::Color::ROJO, Vector2D(0, 2));
        tablero[4][2] = new ClassRey(ClassPieza::Color::AZUL, Vector2D(4, 2));
    }
    else {
        tablero[0][0] = new ClassRey(ClassPieza::Color::ROJO, Vector2D(0, 0));
        tablero[7][0] = new ClassRey(ClassPieza::Color::AZUL, Vector2D(7, 0));
    }
    //colocar torres   
    if (filas_ == 5) {
        tablero[0][0] = new ClassTorre(ClassPieza::Color::ROJO, Vector2D(0, 0));
        tablero[0][3] = new ClassTorre(ClassPieza::Color::ROJO, Vector2D(0, 3));
        tablero[4][0] = new ClassTorre(ClassPieza::Color::AZUL, Vector2D(4, 0));
        tablero[4][3] = new ClassTorre(ClassPieza::Color::AZUL, Vector2D(4, 3));
    }
    else {  
        tablero[0][3] = new ClassTorre(ClassPieza::Color::ROJO, Vector2D(0, 3));
        tablero[7][3] = new ClassTorre(ClassPieza::Color::AZUL, Vector2D(7, 3));
    }
    //colocar caballo
    if (filas_ == 8) {
        tablero[0][2] = new ClassCaballo(ClassPieza::Color::ROJO, Vector2D(0, 2));
        tablero[7][2] = new ClassCaballo(ClassPieza::Color::AZUL, Vector2D(7, 2));
        //colocar alfil
        if (filas_ == 8) {
            tablero[0][1] = new ClassAlfil(ClassPieza::Color::ROJO, Vector2D(0, 1));
            tablero[7][1] = new ClassAlfil(ClassPieza::Color::AZUL, Vector2D(7, 1));
        }
    }
}
// Anima las piezas en el tablero
void ClassTablero::AnimaPiezas() {
    for (int i = 0; i < filas_; ++i) {
        for (int j = 0; j < columnas_; ++j) {
            if (tablero[i][j] != nullptr)
                tablero[i][j]->anima();  // método que llamará al sprite según color o tipo
        }
    }
}
void ClassTablero::ImprimirEnPantalla() {
    for (int i = 0; i < filas_; i++) {
        for (int j = 0; j < columnas_; j++) {
            cout << tablero[i][j] << " ";
        }
        cout << endl;
    }
}
// Metodos para comprobar posiciones
bool ClassTablero::esPosicionValida(const Vector2D& pos) const {
	return pos.x >= 0 && pos.x < filas_ && pos.y >= 0 && pos.y < columnas_; // Comprobar si la posición está dentro de los límites del tablero
}
bool ClassTablero::estaOcupada(const Vector2D& pos) const {
    if (!esPosicionValida(pos)) return false;
    return tablero[pos.x][pos.y] != nullptr;
}

ClassPieza* ClassTablero::getPieza(const Vector2D& pos) const {
    if (!esPosicionValida(pos)) return nullptr;
    return tablero[pos.x][pos.y];
}
bool ClassTablero::estaDentro(const Vector2D& casilla) const {
    int fila = casilla.x;
    int col = casilla.y;
    return fila >= 0 && fila < getFilas() &&
        col >= 0 && col < getColumnas();
}
void ClassTablero::moverPieza(const Vector2D& origen, const Vector2D& destino) {
    if (!esPosicionValida(origen) || !esPosicionValida(destino))
        return;

    ClassPieza* pieza = getPieza(origen);
    if (!pieza)
        return;

    tablero[destino.x][destino.y] = pieza;
    pieza->setPos(Vector2D(destino.x, destino.y));
    tablero[origen.x][origen.y] = nullptr;

    std::cout << "Pieza movida de (" << origen.x << ", " << origen.y
        << ") a (" << destino.x << ", " << destino.y << ")" << std::endl;
}

void ClassTablero::clear() {
    for (int i = 0; i < filas_; i++) {
        for (int j = 0; j < columnas_; j++) {
            //borrar piezas y apuntar puntero a null
            delete tablero[i][j];
            tablero[i][j] = nullptr;
        }
    }

}

void ClassTablero::reset() {
    clear();
    ColocarPiezas();

}

void ClassTablero::resaltarMovimientos(const std::vector<Vector2D>& movimientos) {
    for (const auto& m : movimientos) {
        if (estaDentro(m))
            casillasVisuales[m.x][m.y].setResaltada(true);
    }
}

void ClassTablero::limpiarResaltados() {
    for (auto& fila : casillasVisuales)
        for (auto& cas : fila)
            cas.setResaltada(false);
}
