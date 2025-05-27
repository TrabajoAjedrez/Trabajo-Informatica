#include "Casilla.h"
#include "freeglut.h"

namespace {
    const ClassColor COLOR_CASILLA_CLARA(230, 200, 150); // beige claro
    const ClassColor COLOR_CASILLA_OSCURA(90, 60, 40);   // marrón oscuro
}

float ClassCasilla::tamCasilla = 1.0f;

void ClassCasilla::dibujar(float x, float z) const {
    const ClassColor& color = esClara ? COLOR_CASILLA_CLARA : COLOR_CASILLA_OSCURA;

    glDisable(GL_LIGHTING);
    color.pon_color();

    glBegin(GL_QUADS);
    glVertex3f(x, z, 0);
    glVertex3f(x + tamCasilla, z, 0);
    glVertex3f(x + tamCasilla, z + tamCasilla, 0);
    glVertex3f(x, z + tamCasilla, 0);
    glEnd();

    glEnable(GL_LIGHTING);
}
