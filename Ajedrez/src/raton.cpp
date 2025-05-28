#include "raton.h"
#include <iostream>
#include "freeglut.h" // Para GLUT
#include "Mundo.h"

//void raton::click(int button, int state, int x, int y, ClassMundo* mundo) {
//    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
//        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
//        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
//
//        float normalizedX = (float)x / windowWidth;
//        float normalizedY = (float)y / windowHeight;
//
//        std::cout << "Coordenadas raton normalizadas: (" << normalizedX << ", " << normalizedY << ")" << std::endl;
//    }
//}

Vector2D raton::mouse(int button, int state, int x, int y, int filas, int columnas, ClassMundo* mundo) const {

    int caso = mundo->var_;
    // Manejo del clic izquierdo del raton
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// Normalizar las coordenadas del ratón a un rango de 0 a 1
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        float normalizedX = (float)x / windowWidth;
        float normalizedY = (float)y / windowHeight;

        // Margenes del tablero segun la variante
        float tableroXMin, tableroXMax, tableroYMin, tableroYMax;

        // Elegir el tablero según el caso
        if (caso == 1) {
            tableroXMin = 0.2925f;
            tableroXMax = 0.70625f;
            tableroYMin = 0.085f;
            tableroYMax = 0.77333f;
        }
        else if (caso == 2) {
            tableroXMin = 0.2425f;
            tableroXMax = 0.49875f;
            tableroYMin = 0.0f;
            tableroYMax = 0.67f;
        }
        else {
            std::cout << "Caso de tablero no reconocido  (click izquierdo) " << std::endl;
			return Vector2D(-1, -1); // Retorna un valor inválido si el caso no es reconocido
        }

        // Verificar si el clic está dentro del área del tablero
        if (normalizedX >= tableroXMin && normalizedX <= tableroXMax &&
            normalizedY >= tableroYMin && normalizedY <= tableroYMax) {

            // Calcular tamaño de cada casilla
            float anchoCasilla = (tableroXMax - tableroXMin) / columnas;
            float altoCasilla = (tableroYMax - tableroYMin) / filas;
            int col = (normalizedX - tableroXMin) / anchoCasilla;
            int row = (normalizedY - tableroYMin) / altoCasilla;

            //Ajustes finales por seguridad
            if (row >= 0 && row < filas && col >= 0 && col < columnas) {
                std::cout << "Casilla seleccionada: (" << row << ", " << col << ")  (click izquierdo) " << std::endl;
				return Vector2D(row, col); // Retorna las coordenadas de la casilla seleccionada
            }

        }
        else {
            std::cout << "Click fuera del tablero  (click izquierdo) " << std::endl;
			return Vector2D(-1, -1); // Retorna un valor inválido si el clic está fuera del tablero
        }
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        // Normalizar las coordenadas del ratón a un rango de 0 a 1
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        float normalizedX = (float)x / windowWidth;
        float normalizedY = (float)y / windowHeight;

        // Margenes del tablero segun la variante
        float tableroXMin, tableroXMax, tableroYMin, tableroYMax;

        // Elegir el tablero según el caso
        if (caso == 1) {
            tableroXMin = 0.2925f;
            tableroXMax = 0.70625f;
            tableroYMin = 0.085f;
            tableroYMax = 0.77333f;
        }
        else if (caso == 2) {
            tableroXMin = 0.2425f;
            tableroXMax = 0.49875f;
            tableroYMin = 0.0f;
            tableroYMax = 0.67f;
        }
        else {
            std::cout << "Caso de tablero no reconocido (click derecho) " << std::endl;
			return Vector2D(-1, -1); // Retorna un valor inválido si el caso no es reconocido
        }

        // Verificar si el clic está dentro del área del tablero
        if (normalizedX >= tableroXMin && normalizedX <= tableroXMax &&
            normalizedY >= tableroYMin && normalizedY <= tableroYMax) {

            // Calcular tamaño de cada casilla
            float anchoCasilla = (tableroXMax - tableroXMin) / columnas;
            float altoCasilla = (tableroYMax - tableroYMin) / filas;
            int col = (normalizedX - tableroXMin) / anchoCasilla;
            int row = (normalizedY - tableroYMin) / altoCasilla;

            //Ajustes finales por seguridad
            if (row >= 0 && row < filas && col >= 0 && col < columnas) {
                std::cout << "Casilla seleccionada: (" << row << ", " << col << ") (click derecho) " << std::endl;
                return Vector2D(row, col); // Retorna las coordenadas de la casilla seleccionada
            }
        }
        else {
            std::cout << "Click fuera del tablero  (click derecho) " << std::endl;
			return Vector2D(-1, -1); // Retorna un valor inválido si el clic está fuera del tablero
        }
    }
}

