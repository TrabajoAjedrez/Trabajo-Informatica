#include "TableroGL.h"
#include "freeglut.h"

TableroGL::TableroGL(int dimension, float tamCasilla)
    : dimension(dimension), tamCasilla(tamCasilla), posX(0), posY(0), posZ(0) {
    // Colores por defecto (marfil y marrón)
    colorClaro[0] = 0.96f; colorClaro[1] = 0.96f; colorClaro[2] = 0.86f;
    colorOscuro[0] = 0.55f; colorOscuro[1] = 0.27f; colorOscuro[2] = 0.07f;
}

void TableroGL::dibuja() {
    glPushMatrix();
    glTranslatef(posX, posY, posZ);

    for (int fila = 0; fila < dimension; ++fila) {
        for (int col = 0; col < dimension; ++col) {
            // Alternar colores de casillas
            bool esClara = (fila + col) % 2 == 0;
            float x = col * tamCasilla;
            float z = fila * tamCasilla;

            dibujarCasilla(x, z, esClara);
        }
    }

    // Dibujar borde del tablero
    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, 0.01f, 0);
    glVertex3f(dimension * tamCasilla, 0.01f, 0);
    glVertex3f(dimension * tamCasilla, 0.01f, dimension * tamCasilla);
    glVertex3f(0, 0.01f, dimension * tamCasilla);
    glEnd();

    glPopMatrix();
}

void TableroGL::dibujarCasilla(float x, float z, bool esClara) {
    if (esClara) {
        glColor3fv(colorClaro);
    }
    else {
        glColor3fv(colorOscuro);
    }

    glBegin(GL_QUADS);
    glVertex3f(x, 0, z);
    glVertex3f(x + tamCasilla, 0, z);
    glVertex3f(x + tamCasilla, 0, z + tamCasilla);
    glVertex3f(x, 0, z + tamCasilla);
    glEnd();
}

void TableroGL::setPosicion(float x, float y, float z) {
    posX = x;
    posY = y;
    posZ = z;
}

float TableroGL::getTamanoTotal() const {
    return dimension * tamCasilla;
}