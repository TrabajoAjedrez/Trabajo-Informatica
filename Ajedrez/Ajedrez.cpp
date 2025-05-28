#include <iostream>
#include "freeglut.h"
#include "Mundo.h"
#include "Reglas.h"
#include "raton.h"


raton ratonObj; // crea instancia global del ratón


//int varianteSeleccionada = 1;


ClassMundo* ObjMundo = nullptr; //Puntero a la clase que contiene el mundo


//los callback, funciones que seran llamadas automaticamente por la glut
//cuando sucedan eventos
//NO HACE FALTA LLAMARLAS EXPLICITAMENTE
void OnDraw(void); //esta funcion sera llamada para dibujar
void OnTimer(int value); //esta funcion sera llamada cuando transcurra una temporizacion
void OnKeyboardDown(unsigned char key, int x, int y); //cuando se pulse una tecla	

void OnMouseClick(int button, int state, int x, int y) {
	if (ObjMundo) {
		int filas = ObjMundo->getFilas();
		int columnas = ObjMundo->getColumnas();
		ratonObj.mouse(button, state, x, y, filas, columnas, ObjMundo);
	}
}

// Sirve Verificar las coordenadas del ratón
//void OnMouseClickR(int button, int state, int x, int y) {
//	ratonObj.click(button, state, x, y, ObjMundo);  // imprime coordenadas normalizadas
//}



int main(int argc, char* argv[])
{
	ObjMundo = new ClassMundo(); //Creamos el objeto que contiene el mundo

	ClassReglas reglas;
	ClassReglas* preglas = &reglas; //preglas apunta a reglas (asignacion)


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
		if (ObjMundo) {
			int filas = ObjMundo->getFilas();
			int columnas = ObjMundo->getColumnas();

			//Obtenemos la casilla pulsada por el raton
			Vector2D casillaSeleccionada = ratonObj.mouse(button, state, x, y, filas, columnas, ObjMundo);
			ObjMundo->mueve_pieza(casillaSeleccionada); // Llama al método de selección de casilla en el mundo
		}
	});

	ObjMundo->inicializa(); // Inicializamos el mundo

	//pasarle el control a GLUT,que llamara a los callbacks
	glutMainLoop();

	delete preglas; 
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
	ObjMundo->dibuja();
	glutSwapBuffers();
}
void OnKeyboardDown(unsigned char key, int x_t, int y_t)
{
	ObjMundo->tecla(key);
	//poner aqui el código de teclado
	
	glutPostRedisplay();
}

void OnTimer(int value)
{

	if (ObjMundo) {
		ObjMundo->mueve();
	}
	
	glutPostRedisplay();
	glutTimerFunc(25, OnTimer, 0); //callback de animacion
}