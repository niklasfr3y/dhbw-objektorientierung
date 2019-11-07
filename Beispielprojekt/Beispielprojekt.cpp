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
	int y1 = 150;
	int y2 = 200;
	int y3 = 250;
	int y4 = 300;
	int y5 = 350;
	int y6 = 400;
	int y7 = 450;

public:
	Gosu::Image bild;
	GameWindow()
		: Window(800, 600)
		, bild("rakete.png")
	{
		set_caption("Gosu Tutorial Game mit Git");
	}

	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		bild.draw(400, y1, 0.0);
		bild.draw(400, y2, 0.0);
		bild.draw(400, y3, 0.0);
		bild.draw(400, y4, 0.0);
		bild.draw(400, y5, 0.0);
		bild.draw(400, y6, 0.0);
		
	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		y1 = (y1 + 20) % 450;
		if (y1 == 0) {
			y1 = 150;
		}y2 = (y2 + 20) % 450;
		if (y2 == 0) {
			y2 = 150;
		}y3 = (y3 + 20) % 450;
		if (y3 == 0) {
			y3 = 150;
		}y4 = (y4 + 20) % 450;
		if (y4 == 0) {
			y4 = 150;
		}y5 = (y5 + 20) % 450;
		if (y5 == 0) {
			y5 = 150;
		}y6 = (y6 + 20) % 450;
		if (y6 == 0) {
			y6 = 150;
		}
	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}
