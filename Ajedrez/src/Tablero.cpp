#include "Tablero.h"
#include "freeglut.h"

//TableroGL::TableroGL(float tamCasilla):
//    tamCasilla(tamCasilla), posX(0), posY(0), posZ(0) 
//{
//    // Colores por defecto (marfil y marrón)
//    colorClaro[0] = 0.96f; colorClaro[1] = 0.96f; colorClaro[2] = 0.86f;
//    colorOscuro[0] = 0.55f; colorOscuro[1] = 0.27f; colorOscuro[2] = 0.07f;
//}

void ClassTablero::dibuja(int filas, int columnas, float tamCasilla) {
   
    this->tamCasilla = tamCasilla;

    colorClaro[0] = 0.96f; colorClaro[1] = 0.96f; colorClaro[2] = 0.86f;
    colorOscuro[0] = 0.55f; colorOscuro[1] = 0.27f; colorOscuro[2] = 0.07f;

    glPushMatrix();
    glTranslatef(posX, posY, posZ);

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            // Alternar colores de casillas
            bool esClara = (i + j) % 2 == 0;
            float x = j * tamCasilla;
            float z = i * tamCasilla;

            dibujarCasilla(x, z, esClara);
        }
    }

    // Dibujar borde del tablero
    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, 0.01f, 0);
    glVertex3f(filas * tamCasilla, 0.01f, 0);
    glVertex3f(filas * tamCasilla, 0.01f, filas * tamCasilla);
    glVertex3f(0, 0.01f, filas * tamCasilla);
    glEnd();

    glPopMatrix();
}

void ClassTablero::dibujarCasilla(float x, float z, bool esClara) {
    if (esClara) {
        glColor3fv(colorClaro);
    }
    else {
        glColor3fv(colorOscuro);
    }

    glBegin(GL_QUADS);
    glVertex3f(x, z, 0);
    glVertex3f(x + tamCasilla, z, 0);
    glVertex3f(x + tamCasilla, z+tamCasilla,0);
    glVertex3f(x, z+tamCasilla,0 );
    glEnd();
}

void ClassTablero::setPosicion(float x, float y, float z) {
    posX = x;
    posY = y;
    posZ = z;
}

bool ClassTablero::esPosicionValida(const Vector2D& posicion) const {
	// Verifica si la posición está dentro de los límites del tablero
	return (1);
}

bool ClassTablero::estaOcupada(const Vector2D& posicion) const {
	// Verifica si la posición está ocupada por una pieza
	return (1);
}
