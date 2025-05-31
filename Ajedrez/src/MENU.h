#pragma once
class Menu
{
public:
	int TipoJuego;
	int Lugar;
	int TipoTablero;

	ETSIDI::SpriteSequence spriteReinaA{ "Imagenes/reinaAzul.png",4 };
	ETSIDI::SpriteSequence spriteReyR{ "Imagenes/reyRojo.png",4 };
	ETSIDI::SpriteSequence spritelogo{ "Imagenes/logo.png",1 };

	void setTipoJuego(int a)
	{
		TipoJuego = a;
	}
	void setLugar(int a)
	{
		Lugar = a;
	}
	void setTipoTablero(int a)
	{
		TipoTablero = a;
	}


	int getTipoJuego()
	{
		return(TipoJuego);
	}
	int getLugar()
	{
		return(Lugar);
	}
	int getTipoTablero()
	{
		return(TipoTablero);
	}

	//dibujado
	void dibujarMenu();
	void fondo();

	//Sprites
	Menu() {
		spriteReinaA.setCenter(1, 1);
		spriteReinaA.setSize(3, 3);
		spriteReyR.setCenter(1, 1);
		spriteReyR.setSize(3, 3);
		spritelogo.setCenter(1, 1);
		spritelogo.setSize(5, 5);
	}
	void mueve() {
		spriteReinaA.loop();
		spriteReyR.loop();
	}

};

//Menu.h







