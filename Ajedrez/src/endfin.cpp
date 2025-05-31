#pragma once
#include"Tablero.h"
#include "vector2d.h"
#include "raton.h"
#include "mundo.h"
#include "ETSIDI.h"
#include "Coordinador.h"
#include "endfin.h"
#include <vector>

void drawButton3D2(float x1, float y1, float x2, float y2, float z, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex3f(x1, y1, z);
    glVertex3f(x2, y1, z);
    glVertex3f(x2, y2, z);
    glVertex3f(x1, y2, z);
    glEnd();
}

void draw3DText2(const char* texto, float x, float y, float z, float size, float r, float g, float b) {
    // 1. Calcula el ancho total del texto (en unidades de GLUT "stroke")
    float ancho = 0.0f;
    for (const char* c = texto; *c; ++c)
        ancho += glutStrokeWidth(GLUT_STROKE_ROMAN, *c);

    // 2. Alto de la fuente GLUT_STROKE_ROMAN (~119 unidades)
    float alto = 119.0f; // Este valor es fijo para esta fuente

    glPushMatrix();

    // 3. Mueve al centro deseado (x, y, z)
    glTranslatef(x, y, z);

    // 4. Centra el texto: mueve hacia atrás la mitad del ancho y hacia abajo la mitad del alto
    glTranslatef(-ancho * size / 2.0f, -alto * size / 2.0f, 0.0f);

    // 5. Escala el texto
    glScalef(size, size, size);

    // 6. Color y grosor
    glColor3f(r, g, b);
    glLineWidth(2.2f);

    // 7. Dibuja el texto
    for (const char* c = texto; *c; ++c)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);

    glPopMatrix();
}


void drawButtonWithText2(float x1, float y1, float x2, float y2, float z, float r, float g, float b, const char* texto, float rL, float gL, float bL) {
    //draw3DText("Jugador vs Jugador", (x1+x2)/2, (y1 + y2) / 2, 0.0f, 0.01f, 1.0f, 1.0f, 1.0f);
    draw3DText2(texto, (x1 + x2) / 2, (y1 + y2) / 2, 0.0f, 0.0075f, rL, gL, bL);
    drawButton3D2(x1, y1, x2, y2, z, r, g, b);

}


void endfin::dibujarGAMEOVER() {
    /*
    float AwXR = -7.0f;
    float AwYR = 5.0f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ETSIDI::getTexture("imagenes/CHESSQUEST.png").id);
    glDisable(GL_LIGHTING);
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 1);
    glTexCoord2d(0, 1);  glVertex2d(0 + AwXR, 0 + AwYR);
    glTexCoord2d(1, 1);  glVertex2d(14 + AwXR, 0 + AwYR);
    glTexCoord2d(1, 0);  glVertex2d(14 + AwXR, 14 + AwYR);
    glTexCoord2d(0, 0);  glVertex2d(0 + AwXR, 14 + AwYR);
    glEnd();
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    */
    //drawButtonWithText2(-10.0f + 10.0f + 2.0f, 2.0f, 0.0f - 1.0f, 14.0f, 0.0f, 0.0f, 1.0f, 0.0f, "GAME OVER", 1.0f, 1.0f, 1.0f);
    float AwX = -7.0f;
    float AwY = 0.0f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ETSIDI::getTexture("imagenes/derrota.png").id);
    glDisable(GL_LIGHTING);
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 1);
    glTexCoord2d(0, 1);  glVertex2d(0 + AwX, 0 + AwY);
    glTexCoord2d(1, 1);  glVertex2d(14 + AwX, 0 + AwY);
    glTexCoord2d(1, 0);  glVertex2d(14 + AwX, 14 + AwY);
    glTexCoord2d(0, 0);  glVertex2d(0 + AwX, 14 + AwY);
    glEnd();
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    
}
void endfin::dibujarYOUWIN() {

    //drawButtonWithText2(-10.0f + 10.0f + 2.0f, 2.0f, 0.0f - 1.0f, 14.0f, 0.0f, 0.0f, 1.0f, 0.0f, "GAME OVER", 1.0f, 1.0f, 1.0f);
    float AwX = -7.0f;
    float AwY = 0.0f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ETSIDI::getTexture("imagenes/victoria.png").id);
    glDisable(GL_LIGHTING);
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 1);
    glTexCoord2d(0, 1);  glVertex2d(0 + AwX, 0 + AwY);
    glTexCoord2d(1, 1);  glVertex2d(14 + AwX, 0 + AwY);
    glTexCoord2d(1, 0);  glVertex2d(14 + AwX, 14 + AwY);
    glTexCoord2d(0, 0);  glVertex2d(0 + AwX, 14 + AwY);
    glEnd();
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);


}

void endfin::dibujarTABLAS() {

    //drawButtonWithText2(-10.0f + 10.0f + 2.0f, 2.0f, 0.0f - 1.0f, 14.0f, 0.0f, 0.0f, 1.0f, 0.0f, "GAME OVER", 1.0f, 1.0f, 1.0f);
    float AwX = -7.0f;
    float AwY = 0.0f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ETSIDI::getTexture("imagenes/tablas.png").id);
    glDisable(GL_LIGHTING);
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 1);
    glTexCoord2d(0, 1);  glVertex2d(0 + AwX, 0 + AwY);
    glTexCoord2d(1, 1);  glVertex2d(14 + AwX, 0 + AwY);
    glTexCoord2d(1, 0);  glVertex2d(14 + AwX, 14 + AwY);
    glTexCoord2d(0, 0);  glVertex2d(0 + AwX, 14 + AwY);
    glEnd();
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);


}





