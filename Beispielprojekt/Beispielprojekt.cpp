#include "stdafx.h"

#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

#include <vector>
#include <string>
#include <iostream>

#include "Vektor2d.h"

// Simulationsgeschwindigkeit
const double DT = 100.0;
//Hallo Marvin wie gehts
class GameWindow : public Gosu::Window
{
	Gosu::Image seven;
	Gosu::Image melon;
	Gosu::Image plum;
	Gosu::Image zitrapattoni;
	Gosu::Image banana;
	Gosu::Image bIGWIN;
	Gosu::Image cherryLady;
	Gosu::Image barBarBar;
	Gosu::Image orange;

	int y_Seven = 0;
	int y_Melon = 0;
public:

	GameWindow()
		: Window(1600, 1000), seven("DIE SIEBEN DU HUND.png"), melon("MELOOOONE.png"), plum("IT'S  A MOTHERFUCKIN' PLUM.png"), zitrapattoni("ZITR(APATT)ONI.png")
		, banana("BANANANAANAS.png"), bIGWIN("JAAACKPOOOOOT.png"), cherryLady("JUICYCHERRY.png"), barBarBar("ES REGNET BARES BITCHES.png"), orange("ORANGEMORANGE BLYAT.png")


	{
		set_caption("Gosu Tutorial Game mit Git");
	}

	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		graphics().draw_rect(0, 0, 1600, 100, Gosu::Color::BLUE, 1.0);
		graphics().draw_rect(0, 1000 - 100, 1600, 100, Gosu::Color::BLUE, 1.0); 
		graphics().draw_rect(0, 0, 300, 1000, Gosu::Color::BLUE, 1.0);
		graphics().draw_rect(1600 - 300, 0, 300, 1000, Gosu::Color::BLUE, 1.0);
		seven.draw(300, y, 0.0);
		seven.draw(488, y, 0.0);

	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		y += 10;
		if (y > 1000) y = 0;
	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}
