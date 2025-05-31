#pragma once
#include"Tablero.h"
#include "vector2d.h"
#include "raton.h"
#include "mundo.h"
#include "ETSIDI.h"
#include "Coordinador.h"
#include "Menu.h"
#include <vector>

void drawButton3D(float x1, float y1, float x2, float y2, float z, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex3f(x1, y1, z);
    glVertex3f(x2, y1, z);
    glVertex3f(x2, y2, z);
    glVertex3f(x1, y2, z);
    glEnd();
}

void draw3DText(const char* texto, float x, float y, float z, float size, float r, float g, float b) {
    // Calcula el ancho total del texto
    float ancho = 0.0f;
    for (const char* c = texto; *c; ++c)
        ancho += glutStrokeWidth(GLUT_STROKE_ROMAN, *c);

    // Alto de la fuente
    float alto = 119.0f; // Este valor es fijo para esta fuente

    glPushMatrix();

    //Mueve al centro deseado (x, y, z)
    glTranslatef(x, y, z);

    //Centra el texto: mueve hacia atrás la mitad del ancho y hacia abajo la mitad del alto
    glTranslatef(-ancho * size / 2.0f, -alto * size / 2.0f, 0.0f);

    glScalef(size, size, size);
    glColor3f(r, g, b);
    glLineWidth(2.2f);

    for (const char* c = texto; *c; ++c)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);

    glPopMatrix();
}


void drawButtonWithText(float x1, float y1, float x2, float y2, float z, float r, float g, float b, const char* texto, float rL, float gL, float bL) {
     draw3DText(texto, (x1 + x2) / 2, (y1 + y2) / 2, 0.0f, 0.0075f, rL, gL, bL);
    drawButton3D(x1, y1, x2, y2, z, r, g, b);
}


void Menu::dibujarMenu() {

    draw3DText("Chess Quest", 0.5f, 17.0f, 0.0f, 0.009f, 1.0f, 1.0f, 1.0f);



    draw3DText("Tipo de juego:", -10.0f, 15.0f, 0.0f, 0.005f, 1.0f, 1.0f, 1.0f);
    drawButtonWithText(
        -10.0f - 1.0f, 12.0f, 0.0f - 1.0f, 14.0f, 0.0f,
        (TipoJuego == 1 ? 0.0f : 0.0f),
        (TipoJuego == 1 ? 0.8f : 0.4f),
        (TipoJuego == 1 ? 0.0f : 0.0f),
        "Jugador vs Jugador", 1.0f, 1.0f, 1.0f

    );

    // "Jugador vs IA" (igual lógica)
    drawButtonWithText(
        -10.0f + 10.0f + 2.0f, 12.0f, 0.0f + 10.0f + 2.0f, 14.0f, 0.0f,
        (TipoJuego == 2 ? 0.0f : 0.0f),    // r
        (TipoJuego == 2 ? 0.8f : 0.4f),    // g
        (TipoJuego == 2 ? 0.0f : 0.0f),    // b
        "Jugador vs IA", 1.0f, 1.0f, 1.0f
    );


    draw3DText("Lugar:", -11.2f, 10.0f, 0.0f, 0.005f, 1.0f, 1.0f, 1.0f);
    drawButtonWithText(-10.0f - 1.0f, 7.0f, -5.0f, 9.0f, 0.0f,
        (Lugar == 1 ? 0.2f : 1.0f),    // r
        (Lugar == 1 ? 0.2f : 1.0f),    // g
        (Lugar == 1 ? 0.6f : 1.0f),    // b 
        "Castillo", 0.0f, 0.0f, 0.0f);
    drawButtonWithText(-10.0f + 8.0f, 7.0f, -5.0f + 8.0f, 9.0f, 0.0f,
        (Lugar == 2 ? 0.5f : 1.0f),    // r
        (Lugar == 2 ? 0.0f : 1.0f),    // g
        (Lugar == 2 ? 0.0f : 1.0f),    // b 
        "Mazmorra", 0.0f, 0.0f, 0.0f);
    drawButtonWithText(-10.0f + 16.0f, 7.0f, -5.0f + 16.0f, 9.0f, 0.0f,
        (Lugar == 3 ? 0.0f : 1.0f),    // r
        (Lugar == 3 ? 0.7f : 1.0f),    // g
        (Lugar == 3 ? 0.0f : 1.0f),    // b  
        "Campo", 0.0f, 0.0f, 0.0f);

    draw3DText("Tipo de tablero:", -9.7f, 5.0f, 0.0f, 0.005f, 1.0f, 1.0f, 1.0f);
    drawButtonWithText(-10.0f - 1.0f, 2.0f, 0.0f - 1.0f, 4.0f, 0.0f,
        (TipoTablero == 1 ? 0.5f : 0.35f),    // r
        (TipoTablero == 1 ? 0.5f : 0.35f),    // g
        (TipoTablero == 1 ? 0.0f : 0.35f),    // b 
        "Silverman", 1.0f, 1.0f, 1.0f);
    drawButtonWithText(-10.0f + 10.0f + 2.0f, 2.0f, 0.0f + 10.0f + 2.0f, 4.0f, 0.0f,
        (TipoTablero == 2 ? 0.0f : 0.35f),    // r
        (TipoTablero == 2 ? 0.5f : 0.35f),    // g
        (TipoTablero == 2 ? 0.5f : 0.35f),    // b 
        "Demichess", 1.0f, 1.0f, 1.0f);

    drawButtonWithText(-10.0f - 1.0f + 6.0f, -2.0f, 0.0f - 1.0f + 6.0f, 0.0f, 0.0f, 0.0f, 0.65f, 0.0f, "Jugar", 1.0f, 1.0f, 1.0f);
}