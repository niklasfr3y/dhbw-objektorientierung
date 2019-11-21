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
	Gosu::Image innen;

	std::map<Symbols, Gosu::Image> translation;

	int counter_rotations[5];
	int winners[5];
	int factor[5] = { 10,10,10,10,10 };
	Symbols winner_matrix[5][3];

	bool started = false;

public:

	GameWindow()
		: Window(1600, 1000), seven("DIE SIEBEN DU HUND.png"), melon("MELOOOONE.png"), plum("IT'S  A MOTHERFUCKIN' PLUM.png"), zitrapattoni("ZITR(APATT)ONI.png")
		, banana("BANANANAANAS.png"), bIGWIN("JAAACKPOOOOOT.png"), cherryLady("JUICYCHERRY.png"), barBarBar("ES REGNET BARES BITCHES.png"), orange("ORANGEMORANGE BLYAT.png")
		, background("Slotti.png"), innen("Innen.png")


	{
		set_caption("Gosu Tutorial Game mit Git");
	}

	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		background.draw(0, 0, 1.0);
		innen.draw(300, 150, 0.0);
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
		int x = input().mouse_x();
		int y = input().mouse_y();
		if (!this->started && x >= 1349 && x <= 1549 && y >= 749 && y <= 849 && input().down(Gosu::MS_LEFT)) {
			fillRollsMatrix();
			initReferences();
			this->started = true;
		}
		
		if (started) {
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 9; j++) {
					this->reference_Co[i][j] += factor[i];
					if (this->reference_Co[i][j] == (this->winners[i] - this->counter_rotations[i] * 1800)) {
						factor[i] = 0;
						break;
					}
					if (this->reference_Co[i][j] > 1799) {
						this->reference_Co[i][j] = 0;
						if (j == 0) {
							this->counter_rotations[i] += 1;
						}
					}
				}
			}
			if (factor[0] == 0 && factor[1] == 0 && factor[2] == 0 && factor[3] == 0 && factor[4] == 0) {
				int index = 0;
				for (int i = 0; i < 5; i++) {
					for (int j = 0; j < 9; j++) {
						if (this->reference_Co[i][j] == 200) {
							this->winner_matrix[i][index] = this->reference[i][j % 9];
							this->winner_matrix[i][index + 1] = this->reference[i][(j + 1) % 9];
							this->winner_matrix[i][index + 2] = this->reference[i][(j + 2) % 9];
						}
					}
					index = 0;
				}
				for (int i = 0; i < 5; i++) {
					this->counter_rotations[i] = 0;
					this->factor[i] = 10;
				}
				this->started = false;

				/*
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 5; j++) {
						std::cout << (int)this->winner_matrix[j][i] << ",";
					}
					cout << endl;
				}
				cout << endl;*/

			}
		}
	}

	void fillRollsMatrix() {
		for (int h = 0; h < 5; h++) {
			counter_rotations[h] = 0;
			this->winners[h] = returnWinningIndex();
			for (int i = 0; i < 9; i++) {
				this->reference[h][i] = (Symbols)i;
			}
		}

		//shuffle references
		for (int h = 0; h < 5; h++) {
			for (int i = 9; i > 0; i--) {
				//get swap index
				double zf = rand();
				int j = int(zf) % i;

				//swap i with j
				Symbols temp = this -> reference[h][i - 1];
				this->reference[h][i - 1] = this->reference[h][j];
				this->reference[h][j] = temp;
			}
		}
		/*
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 5; j++) {
				std::cout << (int)this->reference[j][i] << ",";
			}
			cout << endl;
		}
		cout << endl;
		*/

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
				map<Symbols, Gosu::Image>::iterator it = this->translation.find(this->reference[i][j]);
				it->second.draw(x_co, this->reference_Co[i][j], 0.5);
			}
			x_co += 200;
		}
	}

	int returnWinningIndex() {
		//decide on which Symbol to stop
		int w = 5400 + (std::rand() % (9000 - 5400 + 1));
		w /= 200;
		return ceil(w) * 200;
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
