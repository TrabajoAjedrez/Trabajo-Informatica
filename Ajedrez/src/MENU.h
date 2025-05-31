#pragma once
class Menu
{
public:
	int TipoJuego;
	int Lugar;
	int TipoTablero;

	void setTipoJuego(int a){TipoJuego = a;}
	void setLugar(int a){Lugar = a;}
	void setTipoTablero(int a){TipoTablero = a;}

	int getTipoJuego(){return(TipoJuego);}
	int getLugar(){return(Lugar);}
	int getTipoTablero(){return(TipoTablero);}

	void dibujarMenu();

};