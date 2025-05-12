#include <iostream>
#include "freeglut.h"
#include "Mundo.h"
#include "Reglas.h"
#include "raton.h"

raton ratonObj; // crea instancia global del ratón


int varianteSeleccionada = 1;

ClassMundo* ObjMundo = nullptr; //Puntero a la clase que contiene el mundo
//enum class Variante { SILVERMAN = 1, DEMICHESS } VarianteSelccionada;

ClassReglas* preglas=nullptr; 

// Variables para controlar los temporizadores
int tiempo_inicial = 0;
const int INTERVALO_TEMPORIZADOR = 1000; // 1000ms

//los callback, funciones que seran llamadas automaticamente por la glut
//cuando sucedan eventos
//NO HACE FALTA LLAMARLAS EXPLICITAMENTE
void OnDraw(void); //esta funcion sera llamada para dibujar
void OnTimer(int value); //esta funcion sera llamada cuando transcurra una temporizacion
void OnKeyboardDown(unsigned char key, int x, int y); //cuando se pulse una tecla	

int PreguntarVariante();

void OnMouseClick(int button, int state, int x, int y) {
	if (ObjMundo) {
		int filas = ObjMundo->getFilas();
		int columnas = ObjMundo->getColumnas();
		ratonObj.mouse(button, state, x, y, filas, columnas, varianteSeleccionada);
	}
}
void OnMouseClickR(int button, int state, int x, int y) {
	ratonObj.click(button, state, x, y);  // imprime coordenadas normalizadas
}


int main(int argc, char* argv[])
{
	ObjMundo = new ClassMundo(); //Creamos el objeto que contiene el mundo

	ClassReglas reglas;
	preglas = &reglas; //preglas apunta a reglas (asignacion)

	int opcion = PreguntarVariante(); //Pedimos la variante de ajedre
	varianteSeleccionada = opcion;

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

	tiempo_inicial = glutGet(GLUT_ELAPSED_TIME); // Guarda el tiempo inicial
	preglas = new ClassReglas(); // Crea el objeto reglas
	preglas->inicia_temporizador(8); // 8s de prueba

	//Registrar los callbacks
	glutDisplayFunc(OnDraw);
	glutTimerFunc(25, OnTimer, 0);//le decimos que dentro de 25ms llame 1 vez a la funcion OnTimer()
	glutKeyboardFunc(OnKeyboardDown);


	glutMouseFunc(OnMouseClick);

	
	ObjMundo->inicializa(opcion); // Inicializamos el mundo con la variante seleccionada

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
	int tiempo_actual = glutGet(GLUT_ELAPSED_TIME); // Obtener tiempo transcurrido

	// Actualizar temporizador cada segundo
	if (tiempo_actual - tiempo_inicial >= INTERVALO_TEMPORIZADOR) {
		if (preglas) {
			preglas->actualiza_tiempo();
		}
		tiempo_inicial = tiempo_actual;
	}

	if (ObjMundo) {
		ObjMundo->mueve();
	}
	
	glutPostRedisplay();
	glutTimerFunc(25, OnTimer, 0); //callback de animacion
}
//Funcion que pregunta al usuario que variante de ajedrez quiere jugar
int PreguntarVariante() {
	int variante;
	std::cout << "Selecciona la variante de ajedrez:\n";
	std::cout << "1. Silverman\n";
	std::cout << "2. Demichess\n";
	std::cin >> variante;

	return variante;
}