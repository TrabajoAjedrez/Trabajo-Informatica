#include "Tablero.h"
#include "freeglut.h"
#include "Peon.h"
#include "Reina.h"
#include "Rey.h"
#include "torre.h"
#include "caballo.h"
#include "alfil.h"
#include "ETSIDI.h"

ClassTablero::ClassTablero(const ClassTablero& otro) {
    filas_ = otro.filas_;
    columnas_ = otro.columnas_;

    tablero.resize(filas_, std::vector<ClassPieza*>(columnas_, nullptr));

    for (int i = 0; i < filas_; ++i) {
        for (int j = 0; j < columnas_; ++j) {
            if (otro.tablero[i][j]) {
                tablero[i][j] = otro.tablero[i][j]->clonar();
            }
        }
    }
}

void ClassTablero::dibuja( int TipoTablero) {
 
	// Colores para las casillas (pueden personalizarse)
    glPushMatrix();

    if (TipoTablero == 1)
        setPosicion(0, 0, 0);
    else
        setPosicion(2, -2, 0);

        glTranslatef(posX, posY, posZ); // Trasladar el tablero a la posición deseada
        // Bucle para dibujar cada casilla del tablero
        float tamCasilla = ClassCasilla::getTamCasilla();
        for (int i = 0; i < filas_; i++) {
            for (int j = 0; j < columnas_; j++) {
                int filaVisual = getFilas() - 1 - i; //Para que el resaltado se dibuje donde debe

                float x = j * tamCasilla;
                float z = filaVisual * tamCasilla;
                casillasVisuales[i][j].dibujar(x, z);
            }
        }

	// Luego de dibujar el tablero, dibujamos y ubicamos el dibujo de las piezas ¡¡Unicamente en el espacio 2D del tablero, pero no en la matriz!!
    UbicaPieza();
    // Dibujar borde del tablero
    /*
    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0, 0.01f, 0);
    glVertex3f(filas_ * tamCasilla, 0.01f, 0);
    glVertex3f(filas_ * tamCasilla, 0.01f, filas_ * tamCasilla);
    glVertex3f(0, 0.01f, filas_ * tamCasilla);
    glEnd();
    */
    glPopMatrix();
}

// Establece la posición del tablero en el espacio
void ClassTablero::setPosicion(float x, float y, float z) {
    posX = x;
    posY = y;
    posZ = z;
} 

// Dibuja las piezas en el tablero
void ClassTablero::UbicaPieza() {
    for (int i = 0; i < filas_; ++i) {
        for (int j = 0; j < columnas_; ++j) {

            //float x = (columnas_ - j);
           // float z = (filas_ - i);

            float x = j+1;
            float z = (filas_-i);

            if (tablero[i][j] != nullptr)
                tablero[i][j]->dibuja(x, z);  // método que llamará al sprite según color o tipo
        }
    }
}

// Pone los -1 y 1 o lo numeros que representan una pieza en la matriz del tablero
void ClassTablero::ColocarPiezas() {
    //colocar peones
    ETSIDI::play("sonidos/inicio.wav");
    for (int j = 0; j < columnas_; ++j) {
        tablero[1][j] = new ClassPeon(ClassPieza::Color::ROJO, Vector2D(1, j));
        tablero[filas_ - 2][j] = new ClassPeon(ClassPieza::Color::AZUL, Vector2D(filas_ - 2, j));
    }
    //colocar reinas
    if (filas_ == 5) {
        tablero[0][1] = new ClassReina(ClassPieza::Color::ROJO, Vector2D(0, 1));
        tablero[4][1] = new ClassReina(ClassPieza::Color::AZUL, Vector2D(4,1));
    }
    //colocar reyes
    if (filas_ == 5) {
        tablero[0][2] = new ClassRey(ClassPieza::Color::ROJO, Vector2D(0, 2));
        tablero[4][2] = new ClassRey(ClassPieza::Color::AZUL, Vector2D(4, 2));
    }
    else {
        tablero[0][0] = new ClassRey(ClassPieza::Color::ROJO, Vector2D(0, 0));
        tablero[7][0] = new ClassRey(ClassPieza::Color::AZUL, Vector2D(7, 0));
    }
    //colocar torres   
    if (filas_ == 5) {
        tablero[0][0] = new ClassTorre(ClassPieza::Color::ROJO, Vector2D(0, 0));
        tablero[0][3] = new ClassTorre(ClassPieza::Color::ROJO, Vector2D(0, 3));
        tablero[4][0] = new ClassTorre(ClassPieza::Color::AZUL, Vector2D(4, 0));
        tablero[4][3] = new ClassTorre(ClassPieza::Color::AZUL, Vector2D(4, 3));
    }
    else {  
        tablero[0][3] = new ClassTorre(ClassPieza::Color::ROJO, Vector2D(0, 3));
        tablero[7][3] = new ClassTorre(ClassPieza::Color::AZUL, Vector2D(7, 3));
    }
    //colocar caballo
    if (filas_ == 8) {
        tablero[0][2] = new ClassCaballo(ClassPieza::Color::ROJO, Vector2D(0, 2));
        tablero[7][2] = new ClassCaballo(ClassPieza::Color::AZUL, Vector2D(7, 2));
        //colocar alfil
        if (filas_ == 8) {
            tablero[0][1] = new ClassAlfil(ClassPieza::Color::ROJO, Vector2D(0, 1));
            tablero[7][1] = new ClassAlfil(ClassPieza::Color::AZUL, Vector2D(7, 1));
        }
    }
}
// Anima las piezas en el tablero
void ClassTablero::AnimaPiezas() {
    for (int i = 0; i < filas_; ++i) {
        for (int j = 0; j < columnas_; ++j) {
            if (tablero[i][j] != nullptr)
                tablero[i][j]->anima();  // método que llamará al sprite según color o tipo
        }
    }
}
void ClassTablero::ImprimirEnPantalla() {
    for (int i = 0; i < filas_; i++) {
        for (int j = 0; j < columnas_; j++) {
            cout << tablero[i][j] << " ";
        }
        cout << endl;
    }
}
// Metodos para comprobar posiciones
bool ClassTablero::esPosicionValida(const Vector2D& pos) const {
	return pos.x >= 0 && pos.x < filas_ && pos.y >= 0 && pos.y < columnas_; // Comprobar si la posición está dentro de los límites del tablero
}
bool ClassTablero::estaOcupada(const Vector2D& pos) const {
    if (!esPosicionValida(pos)) return false;
    return tablero[pos.x][pos.y] != nullptr;
}

ClassPieza* ClassTablero::getPieza(const Vector2D& pos) const {
    if (!esPosicionValida(pos)) return nullptr;
    return tablero[pos.x][pos.y];
}
bool ClassTablero::estaDentro(const Vector2D& casilla) const {
    int fila = casilla.x;
    int col = casilla.y;
    return fila >= 0 && fila < getFilas() &&
        col >= 0 && col < getColumnas();
}

bool ClassTablero::moverPieza(const Vector2D& origen, const Vector2D& destino) {
    if (!esPosicionValida(origen) || !esPosicionValida(destino))
        return false;

    ClassPieza* pieza = getPieza(origen);
    if (!pieza)
        return false;

    bool esCapturaAlPaso = false;
    Vector2D posPeonCapturadoAlPaso = { -1, -1 };

    // Comprobar si es una captura al paso
    if (pieza->getTipo() == ClassPieza::Peon && destino == casillaObjetivoEnPasante_ && !estaOcupada(destino) && colorPeonVulnerableEnPasante_ != pieza->getColor()) {

        esCapturaAlPaso = true;
        // Determinar la posición del peón que está siendo capturado al paso.
        // Está en la misma columna que `destino`, pero en la fila de `origen`.
        if (pieza->getColor() == ClassPieza::Color::ROJO) { // Peón ROJO captura (se mueve hacia filas mayores)
            posPeonCapturadoAlPaso = Vector2D(destino.x - 1, destino.y); // El peón capturado está una fila "detrás" del destino del captor.
        }
        else { // Peón AZUL captura (se mueve hacia filas menores)
            posPeonCapturadoAlPaso = Vector2D(destino.x + 1, destino.y); // El peón capturado está una fila "detrás" del destino del captor.
        }
    }

    std::vector<Vector2D> movs = pieza->obtenerMovimientosPosibles(*this);
    if (std::find(movs.begin(), movs.end(), destino) == movs.end()) {
        std::cout << "Movimiento invalido para esta pieza." << std::endl;
        return false;
    }

    ClassPieza* destinoPieza = getPieza(destino);
    if (destinoPieza && destinoPieza->getTipo() == ClassPieza::Pieza_t::Rey) {
        std::cout << "No puedes capturar al rey directamente." << std::endl;
        return false;
    }

    // Si fue captura al paso, eliminar el peón capturado de su posición
    if (esCapturaAlPaso) {
        if (esPosicionValida(posPeonCapturadoAlPaso) && getPieza(posPeonCapturadoAlPaso) && getPieza(posPeonCapturadoAlPaso)->getColor() == colorPeonVulnerableEnPasante_) {
            std::cout << "¡Captura al paso! Peon en (" << posPeonCapturadoAlPaso.x << ", " << posPeonCapturadoAlPaso.y << ") eliminado." << std::endl;
            delete tablero[posPeonCapturadoAlPaso.x][posPeonCapturadoAlPaso.y];
            tablero[posPeonCapturadoAlPaso.x][posPeonCapturadoAlPaso.y] = nullptr;
        }
        else {
            // Esto es un estado de error, indica un problema en la lógica previa.
            std::cerr << "Error en captura al paso: El peon vulnerable no se encontro o no coincide en ("
                << posPeonCapturadoAlPaso.x << ", " << posPeonCapturadoAlPaso.y << ")." << std::endl;
        }
    }
    else {
        //la pieza que voy a comer
        ClassPieza* pieza_des = getPieza(destino);
        if (pieza_des) {
            delete pieza_des;
            cout << "pieza comida" << endl;
        }
    }

    tablero[destino.x][destino.y] = pieza;
    pieza->setPos(Vector2D(destino.x, destino.y));
    tablero[origen.x][origen.y] = nullptr;

    std::cout << "Pieza movida de (" << origen.x << ", " << origen.y
        << ") a (" << destino.x << ", " << destino.y << ")" << std::endl;

    return true;
}

void ClassTablero::clear() {
    for (int i = 0; i < filas_; i++) {
        for (int j = 0; j < columnas_; j++) {
            //borrar piezas y apuntar puntero a null
            delete tablero[i][j];
            tablero[i][j] = nullptr;
        }
    }
}

void ClassTablero::reset() {
    clear();
    ColocarPiezas();

}

void ClassTablero::resaltarMovimientos(const std::vector<Vector2D>& movimientos) {
    for (const auto& m : movimientos) {
        if (estaDentro(m))
            casillasVisuales[m.x][m.y].setResaltada(true);
    }
}

void ClassTablero::limpiarResaltados() {
    for (auto& fila : casillasVisuales)
        for (auto& cas : fila)
            cas.setResaltada(false);
}


//dibujar la exclamación:
void ClassTablero::dibujarExclamacionSobreRey(const Vector2D& posRey, ClassPieza::Color color, float tiempoRebote) {
    float tamCasilla = ClassCasilla::getTamCasilla();


    // Obtener desplazamiento del tablero
    float offsetX = posX;
    float offsetZ = posZ;

    float x = offsetX + posRey.y * tamCasilla + tamCasilla / 2.0f;
    float y = offsetZ + (filas_ - 1 - posRey.x) * tamCasilla + tamCasilla / 2.0f;


    float rebote = 0.1f * sin(tiempoRebote * 3.0f); // rebote animado con función seno

    glDisable(GL_LIGHTING);
    if (color == ClassPieza::Color::AZUL)
        glColor3f(0.0f, 0.0f, 1.0f);
    else
        glColor3f(1.0f, 0.0f, 0.0f);

    glPushMatrix();
    glTranslatef(x, y + 0.2f + rebote, 1.0f);
    glScalef(0.01f, 0.005f, 1.0f); // tamaño del signo
    glutStrokeCharacter(GLUT_STROKE_ROMAN, '!');
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

bool ClassTablero::esPiezaCapturable(const Vector2D& pos, ClassPieza::Color color) const {
    ClassPieza* p = getPieza(pos);
    return p && !(p->getTipo() == ClassPieza::Pieza_t::Rey && p->getColor() == color);
}

//nueva pieza por promocion
void ClassTablero::promocionarPieza(const ClassPieza& pieza, char seleccion, int var) {
    // char seleccion;
    Vector2D posPromo = pieza.getPos();
    ClassPieza::Color colPromo = pieza.getColor();

    //lo primero es eliminar el peon
    delete tablero[posPromo.x][posPromo.y];
    tablero[posPromo.x][posPromo.y] = nullptr;


    //crear la nueva pieza
    ClassPieza* nuevaPieza = nullptr;

    switch (seleccion) {
    case 'd':
        if (var == 2)
            break;
        else {
            nuevaPieza = new ClassReina(colPromo, posPromo);
            break;
        }
    case 'c':
        if (var == 1)
            break;
        else {
            nuevaPieza = new ClassCaballo(colPromo, posPromo);
            break;
        }
    case 't':
        nuevaPieza = new ClassTorre(colPromo, posPromo);
        break;
    case 'a':
        if (var == 1)
            break;
        else {
            nuevaPieza = new ClassAlfil(colPromo, posPromo);
            break;
        }
    default:
        break;
    }

    tablero[posPromo.x][posPromo.y] = nuevaPieza;

}

void ClassTablero::setCasillaEnPasante(const Vector2D& casilla, ClassPieza::Color colorPeonVulnerable) {
    casillaObjetivoEnPasante_ = casilla;
    colorPeonVulnerableEnPasante_ = colorPeonVulnerable;
}

void ClassTablero::limpiarCasillaEnPasante() {
    casillaObjetivoEnPasante_ = { -1, -1 };
    // si casillaObjetivoEnPasante_ es {-1,-1} indica que no hay captura al paso posible.
}

//destructor. Tablero crea piezas y tablero las destruye (que poetico) --quien ha puesto esto?
ClassTablero::~ClassTablero() {
    clear();  // puede hacerlo directamente por clear no hay que limpiarlas una a una
}
