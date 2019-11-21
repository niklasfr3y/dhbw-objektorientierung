#include "stdafx.h"

#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

#include <vector>
#include <string>
#include <iostream>

#include "Vektor2d.h"
#include <algorithm>    // std::random_shuffle
#include <map>

using namespace std;
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
	int reference_Co[5][9];
	Gosu::Image reference_Visual[5][9];

	
	Gosu::Image seven;
	Gosu::Image melon;
	Gosu::Image plum;
	Gosu::Image zitrapattoni;
	Gosu::Image banana;
	Gosu::Image bIGWIN;
	Gosu::Image cherryLady;
	Gosu::Image barBarBar;
	Gosu::Image orange;
	Gosu::Image background;

	std::map<Symbols, Gosu::Image> translation;

	int y_Seven = 0;
	int y_Melon = 0;
public:

	GameWindow()
		: Window(1600, 1000), seven("DIE SIEBEN DU HUND.png"), melon("MELOOOONE.png"), plum("IT'S  A MOTHERFUCKIN' PLUM.png"), zitrapattoni("ZITR(APATT)ONI.png")
		, banana("BANANANAANAS.png"), bIGWIN("JAAACKPOOOOOT.png"), cherryLady("JUICYCHERRY.png"), barBarBar("ES REGNET BARES BITCHES.png"), orange("ORANGEMORANGE BLYAT.png")
		, background("Slotti.png")


	{
		set_caption("Gosu Tutorial Game mit Git");
	}

	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		//background.draw(0, 0, 1.0);
		/*graphics().draw_rect(0, 0, 1600, 100, Gosu::Color::BLUE, 1.0);
		graphics().draw_rect(0, 1000 - 100, 1600, 100, Gosu::Color::BLUE, 1.0); 
		graphics().draw_rect(0, 0, 300, 1000, Gosu::Color::BLUE, 1.0);
		graphics().draw_rect(1600 - 300, 0, 300, 1000, Gosu::Color::BLUE, 1.0);*/
		//graphics().draw_rect(488, 0, 12, 1000, Gosu::Color::BLUE, 1.0);

		fillRollsVisual();

	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 9; j++) {
				this->reference_Co[i][j] += 10;
				if (this->reference_Co[i][j] > 1800) {
					this->reference_Co[i][j] = 0;
				}
			}
		}
	}

	void fillRollsMatrix() {
		for (int h = 0; h < 5; h++) {
			for (int i = 0; i < 9; i++) {
				this->reference[h][i] = (Symbols)i;
			}
		}

		//shuffle references
		for (int h = 0; h < 5; h++) {
			for (int i = 9; i > 0; i--) {
				//get swap index
				double zf = rand();
				//cout << "Zufall = " << zf << endl;
				int j = int(zf) % i;

				//swap i with j
				Symbols temp = this -> reference[h][i - 1];
				this->reference[h][i - 1] = this->reference[h][j];
				this->reference[h][j] = temp;
			}
		}
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 5; j++) {
				std::cout << (int) this->reference[j][i] << ",";
			}
			std::cout << std::endl;
		}
	}

	void initReferences() {
		for (int i = 0; i < 9; i++) {
			translation.insert(std::pair<Symbols, Gosu::Image>((Symbols)i, returnCorrespondingImage(i)));
		}
		
		int y_start = 0;
		for (int h = 0; h < 5; h++) {
			for (int i = 0; i < 9; i++) {
				this->reference_Co[h][i] = y_start;
				y_start += 200;
			}
			y_start = 0;
		}
	}

	Gosu::Image returnCorrespondingImage(int reference) {
		switch (reference) {
		case 0: return this->banana;
			break;
		case 1: return this->cherryLady;
			break;
		case 2: return this->seven;
			break;
		case 3: return this->barBarBar;
			break;
		case 4: return this->bIGWIN;
			break;
		case 5: return this->plum;
			break;
		case 6: return this->melon;
			break;
		case 7: return this->zitrapattoni;
			break;
		case 8: return this->orange;
			break;
		}
	}
	
	void fillRollsVisual() {
		//reference 300 * column_index
		int x_co = 300;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 9; j++) {
				cout << int(this->reference[i][j]) << endl;
				map<Symbols, Gosu::Image>::iterator it = this->translation.find(this->reference[i][j]);
				cout << int(it->first) << endl;
				it->second.draw(x_co, this->reference_Co[i][j], 0.0);
			}
			x_co += 200;
		}
	}

};

// C++ Hauptprogramm
int main()
{
	GameWindow window;

	window.fillRollsMatrix();
	window.initReferences();

	window.show();
}
