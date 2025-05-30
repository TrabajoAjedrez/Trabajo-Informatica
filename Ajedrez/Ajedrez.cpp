#include <iostream>
#include "freeglut.h"
#include "raton.h"
#include "Coordinador.h"
#include"ETSIDI.h"

raton ratonObj; // crea instancia global del rat�n
Coordinador* coordinador = nullptr; 

void OnDraw(void); 
void OnTimer(int value); 
void OnKeyboardDown(unsigned char key, int x, int y); 

void OnMouseClick(int button, int state, int x, int y) {
	if (coordinador->ObjMundo) {
		int filas = coordinador->ObjMundo->getFilas();
		int columnas = coordinador->ObjMundo->getColumnas();
		ratonObj.mouse(button, state, x, y, filas, columnas, coordinador->ObjMundo);
	}
}

int main(int argc, char* argv[])
{
	coordinador = new Coordinador();
	//Inicializar el gestor de ventanas GLUT
	//y crear la ventana
	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Ajedrez");

	//habilitar luces y definir perspectiva
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(40.0, 800 / 600.0f, 0.1, 150);

	//Registrar los callbacks
	glutDisplayFunc(OnDraw);
	glutTimerFunc(25, OnTimer, 0);//le decimos que dentro de 25ms llame 1 vez a la funcion OnTimer()
	glutKeyboardFunc(OnKeyboardDown);

	glutMouseFunc([](int button, int state, int x, int y) {
		if (coordinador) {
			if (coordinador->estado == Coordinador::MENU) {
				if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
				{
					int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
					int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
					float normalizedX = (float)x / windowWidth;
					float normalizedY = (float)y / windowHeight;
					//std::cout << "Click en MENU: (" << normalizedX << ", " << normalizedY << ")" << std::endl;
					if (normalizedX >= 0.1225 && normalizedX <= 0.46 && normalizedY <= 0.28333 && normalizedY >= 0.2)
					{
						//jugador vs jugador
						std::cout << "jugador vs jugador" << std::endl;
						coordinador->menu.setTipoJuego(1);
						ETSIDI::play("sonidos/click.wav");

					}
					if (normalizedX >= 0.57 && normalizedX <= 0.915 && normalizedY <= 0.28333 && normalizedY >= 0.2)
					{
						//jugador vs IA
						std::cout << "jugador vs IA" << std::endl;
						coordinador->menu.setTipoJuego(2);
						ETSIDI::play("sonidos/click.wav");
					}
					if (normalizedX >= 0.1225 && normalizedX <= 0.32375 && normalizedY <= 0.51667 && normalizedY >= 0.43)
					{
						//castillo
						std::cout << "castillo" << std::endl;
						coordinador->menu.setLugar(1);
						ETSIDI::play("sonidos/click.wav");
					}
					if (normalizedX >= 0.43125 && normalizedX <= 0.6025 && normalizedY <= 0.52833 && normalizedY >= 0.43)
					{
						//mazmorra
						std::cout << "mazmorra" << std::endl;
						coordinador->menu.setLugar(2);
						ETSIDI::play("sonidos/click.wav");
					}
					if (normalizedX >= 0.70625 && normalizedX <= 0.87875 && normalizedY <= 0.52833 && normalizedY >= 0.43)
					{
						//campo
						std::cout << "campo" << std::endl;
						coordinador->menu.setLugar(3);
						ETSIDI::play("sonidos/click.wav");
					}
					if (normalizedX >= 0.12125 && normalizedX <= 0.46125 && normalizedY <= 0.745 && normalizedY >= 0.65833)
					{
						//silverman
						std::cout << "silverman" << std::endl;
						coordinador->menu.setTipoTablero(1);
						ETSIDI::play("sonidos/click.wav");
					}
					if (normalizedX >= 0.56625 && normalizedX <= 0.91125 && normalizedY <= 0.745 && normalizedY >= 0.65833)
					{
						//demichess
						std::cout << "demichess" << std::endl;
						coordinador->menu.setTipoTablero(2);
						ETSIDI::play("sonidos/click.wav");
					}
					if (normalizedX >= 0.3275 && normalizedX <= 0.67 && normalizedY <= 0.931667 && normalizedY >= 0.843333)
					{
						//jugar

						if ((coordinador->menu.getTipoJuego() == 1 || coordinador->menu.getTipoJuego() == 2)
							&&
							(coordinador->menu.getLugar() == 1 || coordinador->menu.getLugar() == 2 || coordinador->menu.getLugar() == 3)
							&&
							(coordinador->menu.getTipoTablero() == 1 || coordinador->menu.getTipoTablero() == 2))
						{

							//std::cout << "jugar" << std::endl;
							coordinador->estado = Coordinador::JUEGO;
							coordinador->ObjMundo->setTipoJuego(coordinador->menu.getTipoJuego());
							coordinador->ObjMundo->setLugar(coordinador->menu.getLugar());
							coordinador->ObjMundo->setTipoTablero(coordinador->menu.getTipoTablero());
							coordinador->ObjMundo->inicializa();

						}
					}
				}


			}
			else if (coordinador->estado == Coordinador::JUEGO) {
				if (state != GLUT_DOWN)return;
				if (coordinador->ObjMundo)
				{
					int filas = coordinador->ObjMundo->getFilas();
					int columnas = coordinador->ObjMundo->getColumnas();
					Vector2D casillaSeleccionada = ratonObj.mouse(button, state, x, y, filas, columnas, coordinador->ObjMundo);
					coordinador->ObjMundo->seleccionarCasilla(casillaSeleccionada);
				}
			}
		}
		});

	//pasarle el control a GLUT,que llamara a los callbacks
	glutMainLoop();

	return 0;
}

void OnDraw(void)
{
	//Borrado de la pantalla	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Para definir el punto de vista
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//no borrar esta linea ni poner nada despues
	coordinador->dibuja(); 

	//ObjMundo->dibuja();
	glutSwapBuffers();
}
void OnKeyboardDown(unsigned char key, int x_t, int y_t)
{
	coordinador->tecla(key); 
	//poner aqui el c�digo de teclado
	glutPostRedisplay();
}

void OnTimer(int value)
{
	if (coordinador) {
		coordinador->mueve();
	}
	glutPostRedisplay();
	glutTimerFunc(25, OnTimer, 0); //callback de animacion
}






