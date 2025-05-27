#include "Casilla.h"
#include "freeglut.h"
#include <math.h>

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

    if (resaltada) {
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4ub(0, 255, 0, 128);  // verde semitransparente
        float radio = tamCasilla * 0.25f;
        float centerX = x + tamCasilla / 2.0f;
        float centerY = z + tamCasilla / 2.0f;  // ahora usamos Y, no Z
        float zPlano = 0.02f;  // profundidad fija

        glBegin(GL_TRIANGLE_FAN);
        glVertex3f(centerX, centerY, zPlano);  // centro

        for (int i = 0; i <= 20; ++i) {
            float angle = 2.0f * 3.1415926f * i / 20;
            float dx = cos(angle) * radio;
            float dy = sin(angle) * radio;
            glVertex3f(centerX + dx, centerY + dy, zPlano);
        }
        glEnd();

        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
    }
}
