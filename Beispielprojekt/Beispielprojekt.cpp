#include "stdafx.h"

#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

#include <vector>
#include <string>
#include <iostream>

#include "Vektor2d.h"

// Simulationsgeschwindigkeit
const double DT = 100.0;
//Hallo Marvin wie gehts dir heute
class GameWindow : public Gosu::Window
{
	int y = 10;
public:
	Gosu::Image bild;
	GameWindow()
		: Window(1920, 1080)
	{
		set_caption("Slotti - A Knossi Tribute");
	}

	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		graphics().draw_rect
		(
			10, y, 40, 60, Gosu::Color::RED,0.0
			);

	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		y = y ++ %600;
	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}
