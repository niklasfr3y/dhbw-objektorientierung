#include "stdafx.h"

#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

#include <vector>
#include <string>
#include <iostream>

#include "Vektor2d.h"
#include <algorithm>    // std::random_shuffle
#include <map>

// Simulationsgeschwindigkeit
const double DT = 100.0;
//Hallo Marvin wie gehts
class GameWindow : public Gosu::Window
{
	enum class Symbols {
		BANANA = 0,
		CHERRY = 1,
		SEVEN = 2,
		BAR = 3,
		BIGWIN = 4,
		PLUM = 5,
		MELON = 6,
		CITRUS = 7,
		ORANGE = 8
	};
	Symbols reference[5][9];
	
	Gosu::Image seven;
	Gosu::Image melon;
	Gosu::Image plum;
	Gosu::Image zitrapattoni;
	Gosu::Image banana;
	Gosu::Image bIGWIN;
	Gosu::Image cherryLady;
	Gosu::Image barBarBar;
	Gosu::Image orange;

	//map<Symbols, Gosu::Image> translate;

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
		seven.draw(300, y_Seven, 0.0);
		graphics().draw_rect(488, 0, 12, 1000, Gosu::Color::BLUE, 1.0);
		melon.draw(500, y_Melon, 0.0);

	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		y_Seven += 10;
		if (y_Seven > 1000) y_Seven = 0;
		y_Melon += 10;
		if (y_Melon > 1000) y_Melon = 0;
	}

	void fillWinner() {
		//shuffle references
		for (int h = 0; h < 5; h++) {
			for (int i = 9; i>0; i--) {
				//get swap index
				int j = rand() % i;
				//swap i with j
				Symbols temp = this -> reference[h][i - 1];
				this->reference[h][i - 1] = this->reference[h][j];
				this->reference[h][j] = temp;
			}
		}
		/*for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 9; j++) {
				std::cout << (int) this->reference[i][j] << ",";
			}
			std::cout << std::endl;
		}*/
	}

	void initReferences() {
		for (int h = 0; h < 5; h++) {
			for (int i = 0; i < 9; i++) {
				this->reference[h][i] = (Symbols)i;
			}
		}
	}

	void fillROLLS() {

	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.initReferences();
	window.fillWinner();
	window.show();
}
