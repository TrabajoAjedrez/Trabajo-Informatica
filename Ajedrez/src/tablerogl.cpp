#include "TableroGL.h"
#include "freeglut.h"

//TableroGL::TableroGL(float tamCasilla):
//    tamCasilla(tamCasilla), posX(0), posY(0), posZ(0) 
//{
//    // Colores por defecto (marfil y marrón)
//    colorClaro[0] = 0.96f; colorClaro[1] = 0.96f; colorClaro[2] = 0.86f;
//    colorOscuro[0] = 0.55f; colorOscuro[1] = 0.27f; colorOscuro[2] = 0.07f;
//}

void TableroGL::dibuja(int filas, int columnas, float tamCasilla) {
   
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

//float TableroGL::getTamanoTotal() const {
//    return filas_*columnas_ * tamCasilla;
//}