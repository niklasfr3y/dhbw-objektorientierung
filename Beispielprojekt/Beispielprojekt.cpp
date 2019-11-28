#include "stdafx.h"

#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

#include <vector>
#include <string>
#include <iostream>

#include "Vektor2d.h"
#include <algorithm>    // std::random_shuffle
#include <map>
#include <iomanip>
#include <sstream>
#include <ctime>

using namespace std;

// Simulationsgeschwindigkeit
const double DT = 100.0;

class GameWindow : public Gosu::Window
{
	enum class Symbols {
		BANANA = 0,
		CHERRY = 1,
		PLUM = 2,
		MELON = 3,
		CITRUS = 4,
		ORANGE = 5,
		BAR = 6,
		SEVEN = 7,
		BIGWIN = 8
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
	Gosu::Image pointer;

	std::map<Symbols, Gosu::Image> translation;

	int counter_rotations[5];
	int winners[5];
	int factor[5] = { 20,20,20,20,20 };
	Symbols winner_matrix[5][3];
	Symbols winning_lines[9][5];

	bool started;

	double amount;
	Gosu::Font f_amount = Gosu::Font(50, "amount");

	int x_mouse;
	int y_mouse;

	double credit;
	double payout;

public:

	GameWindow()
		: Window(1600, 1000), seven("DIE SIEBEN DU HUND.png"), melon("MELOOOONE.png"), plum("IT'S  A MOTHERFUCKIN' PLUM.png"), zitrapattoni("ZITR(APATT)ONI.png")
		, banana("BANANANAANAS.png"), bIGWIN("JAAACKPOOOOOT.png"), cherryLady("JUICYCHERRY.png"), barBarBar("ES REGNET BARES BITCHES.png"), orange("ORANGEMORANGE BLYAT.png")
		, background("Slotti.png"), innen("Innen.png"), pointer("pointer.png")


	{
		this->credit = 500;
		this->payout = 0;
		this->amount = 0.1;
		this->started = false;
		set_caption("Gosu Tutorial Game mit Git");
	}

	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		background.draw(0, 0, 1.0);
		innen.draw(300, 150, 0.0);

		std::stringstream stream;
		stream << std::fixed << std::setprecision(2) << this->amount << " $";
		std::string s = stream.str();
		this->f_amount.draw(s, 80, 430, 2.0, 1.0, 1.0, Gosu::Color::WHITE, Gosu::AlphaMode::AM_DEFAULT);

		// +- Einsätze
		graphics().draw_rect(70, 350, 30, 80, Gosu::Color::BLUE, 1.0);
		graphics().draw_rect(200, 350, 50, 80, Gosu::Color::BLUE, 1.0);
		//Max Einsatz
		//graphics().draw_rect(49, 749, 200, 100, Gosu::Color::BLUE, 0.0);
		// gamble
		//graphics().draw_rect(1349, 349, 200, 200, Gosu::Color::BLUE, 0.0);
		//graphics().draw_rect(488, 0, 12, 1000, Gosu::Color::BLUE, 1.0);

		fillRollsVisual();
		this->pointer.draw(this->x_mouse, this->y_mouse, 10.0);
	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		this->x_mouse = input().mouse_x();
		this->y_mouse = input().mouse_y();
		if (!this->started && this->x_mouse >= 70 && this->x_mouse <= 100 && this->y_mouse >= 350 && this->y_mouse <= 430 && input().down(Gosu::MS_LEFT)) {
			if (this->amount == 0.10) {
				return;
			}
			else if (this->amount == 0.20) {
				this->amount = 0.1;
			}
			else if (this->amount == 0.50) {
				this->amount = 0.2;
			}
			else if (this->amount == 1.0) {
				this->amount = 0.5;
			}
			else if (this->amount == 2.0) {
				this->amount = 1.0;
			}
			else if (this->amount == 5.0) {
				this->amount = 2.0;
			}
			else if (this->amount == 10.0) {
				this->amount = 5.0;
			}
			else {
				this->amount = 10.0;
			}
			//wait to avoid multiple presses
			Sleep(200);
		}

		if (!this->started && this->x_mouse >= 200 && this->x_mouse <= 250 && this->y_mouse >= 350 && this->y_mouse <= 430 && input().down(Gosu::MS_LEFT)) {
			if (this->amount == 0.10) {
				this->amount = 0.2;
			}
			else if (this->amount == 0.20) {
				this->amount = 0.5;
			}
			else if (this->amount == 0.50) {
				this->amount = 1.0;
			}
			else if (this->amount == 1.0) {
				this->amount = 2.0;
			}
			else if (this->amount == 2.0) {
				this->amount = 5.0;
			}
			else if (this->amount == 5.0) {
				this->amount = 10.0;
			}
			else if (this->amount == 10.0) {
				this->amount = 20.0;
			}
			else {
				return;
			}
			//wait to avoid multiple presses
			Sleep(200);
		}

		if (!this->started && this->x_mouse >= 49 && this->x_mouse <= 249 && this->y_mouse >= 749 && this->y_mouse <= 849 && input().down(Gosu::MS_LEFT)) {
			this->amount = 20.0;
		}

		//gamble logic
		//
		//

		//
		//
		if (this->payout > 0 && !this->started && this->x_mouse >= 1349 && this->x_mouse <= 1549 && this->y_mouse >= 349 && this->y_mouse <= 549 && input().down(Gosu::MS_LEFT)) {
			cout << "gamble" << endl;
			//wait to avoid multiple presses
			Sleep(200);
		}

		if (this->amount <= this->credit && !this->started && this->x_mouse >= 1349 && this->x_mouse <= 1549 && this->y_mouse >= 749 && this->y_mouse <= 849 && input().down(Gosu::MS_LEFT)) {
			fillRollsMatrix();
			initReferences();
			this->payout = 0;
			this->credit -= this->amount;
			this->started = true;
		}

		if (started) {
			//spin wheels until winner coordinate is reached
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 9; j++) {
					this->reference_Co[i][j] += factor[i];
					//check if winner coordinate is reached
					if (this->reference_Co[i][j] == (this->winners[i] - this->counter_rotations[i] * 1800)) {
						factor[i] = 0;
						break;
					}
					//increase counter for rotations
					if (this->reference_Co[i][j] > 1799) {
						this->reference_Co[i][j] = 0;
						if (j == 0) {
							this->counter_rotations[i] += 1;
						}
					}
				}
			}
			//create matrix of only the visible symbols
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

				//reset counters and spinning factors
				for (int i = 0; i < 5; i++) {
					this->counter_rotations[i] = 0;
					this->factor[i] = 20;
				}
				this->started = false;

				calcPayout();
				this->credit += this->payout;
				cout << this->credit << endl;
				/*
				//print for debug
				for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 5; j++) {
				std::cout << (int)this->winner_matrix[j][i] << ",";
				}
				cout << endl;
				}
				cout << endl;
				*/
			}
		}
	}

	void fillWinningLines() {
		//Line 1 horizontal
		this->winning_lines[0][0] = this->winner_matrix[0][0];
		this->winning_lines[0][1] = this->winner_matrix[1][0];
		this->winning_lines[0][2] = this->winner_matrix[2][0];
		this->winning_lines[0][3] = this->winner_matrix[3][0];
		this->winning_lines[0][4] = this->winner_matrix[4][0];
		//Line 2 horizontal
		this->winning_lines[1][0] = this->winner_matrix[0][1];
		this->winning_lines[1][1] = this->winner_matrix[1][1];
		this->winning_lines[1][2] = this->winner_matrix[2][1];
		this->winning_lines[1][3] = this->winner_matrix[3][1];
		this->winning_lines[1][4] = this->winner_matrix[4][1];
		//Line 3 horizontal
		this->winning_lines[2][0] = this->winner_matrix[0][2];
		this->winning_lines[2][1] = this->winner_matrix[1][2];
		this->winning_lines[2][2] = this->winner_matrix[2][2];
		this->winning_lines[2][3] = this->winner_matrix[3][2];
		this->winning_lines[2][4] = this->winner_matrix[4][2];
		//Line 4 triangle
		this->winning_lines[3][0] = this->winner_matrix[0][0];
		this->winning_lines[3][1] = this->winner_matrix[1][1];
		this->winning_lines[3][2] = this->winner_matrix[2][2];
		this->winning_lines[3][3] = this->winner_matrix[3][1];
		this->winning_lines[3][4] = this->winner_matrix[4][0];
		//Line 5 triangle
		this->winning_lines[4][0] = this->winner_matrix[0][2];
		this->winning_lines[4][1] = this->winner_matrix[1][1];
		this->winning_lines[4][2] = this->winner_matrix[2][0];
		this->winning_lines[4][3] = this->winner_matrix[3][1];
		this->winning_lines[4][4] = this->winner_matrix[4][2];
		//Line 6 mid-bottom
		this->winning_lines[5][0] = this->winner_matrix[0][1];
		this->winning_lines[5][1] = this->winner_matrix[1][2];
		this->winning_lines[5][2] = this->winner_matrix[2][2];
		this->winning_lines[5][3] = this->winner_matrix[3][2];
		this->winning_lines[5][4] = this->winner_matrix[4][1];
		//Line 7 mid-top
		this->winning_lines[6][0] = this->winner_matrix[0][1];
		this->winning_lines[6][1] = this->winner_matrix[1][0];
		this->winning_lines[6][2] = this->winner_matrix[2][0];
		this->winning_lines[6][3] = this->winner_matrix[3][0];
		this->winning_lines[6][4] = this->winner_matrix[4][1];
		//Line 8 bottom-top
		this->winning_lines[7][0] = this->winner_matrix[0][2];
		this->winning_lines[7][1] = this->winner_matrix[1][2];
		this->winning_lines[7][2] = this->winner_matrix[2][1];
		this->winning_lines[7][3] = this->winner_matrix[3][0];
		this->winning_lines[7][4] = this->winner_matrix[4][0];
		//Line 9 top-bottom
		this->winning_lines[8][0] = this->winner_matrix[0][0];
		this->winning_lines[8][1] = this->winner_matrix[1][0];
		this->winning_lines[8][2] = this->winner_matrix[2][1];
		this->winning_lines[8][3] = this->winner_matrix[3][2];
		this->winning_lines[8][4] = this->winner_matrix[4][2];
	}

	void calcPayout() {
		fillWinningLines();
		for (int i = 0; i < 9; i++) {
			double factorForPayout = 0;
			Symbols current = this->winning_lines[i][0];
			int counter = 1;
			while (current == this->winning_lines[i][counter]) {
				//calculate multiplicator
				switch (counter) {
				case 1: if (int(this->winning_lines[i][0]) >= 6) { factorForPayout = 2; }
						else { factorForPayout = 0; }
						break;
				case 2: if (int(this->winning_lines[i][0]) >= 6) { factorForPayout = 5; }
						else { factorForPayout = 0.5; }
						break;
				case 3: if (int(this->winning_lines[i][0]) >= 6) { factorForPayout = 10; }
						else { factorForPayout = 1; }
						break;
				case 4: if (int(this->winning_lines[i][0]) >= 6) { factorForPayout = 20; }
						else { factorForPayout = 2; }
						break;
				}
				current = this->winning_lines[i][counter];
				counter++;
				if (counter == 5) break;
			}
			this->payout += (this->amount * factorForPayout);
		}
		cout << payout << endl;

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
		srand(time(NULL));
		for (int h = 0; h < 5; h++) {
			for (int i = 9; i > 0; i--) {
				//get swap index
				double zf = rand();
				int j = int(zf) % i;

				//swap i with j
				Symbols temp = this->reference[h][i - 1];
				this->reference[h][i - 1] = this->reference[h][j];
				this->reference[h][j] = temp;
			}
		}

		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 5; j++) {
				std::cout << (int)this->reference[j][i] << ",";
			}
			cout << endl;
		}
		cout << endl;


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
		switch ((Symbols)reference) {
		case Symbols::BANANA: return this->banana;
			break;
		case Symbols::CHERRY: return this->cherryLady;
			break;
		case Symbols::SEVEN: return this->seven;
			break;
		case Symbols::BAR: return this->barBarBar;
			break;
		case Symbols::BIGWIN: return this->bIGWIN;
			break;
		case Symbols::PLUM: return this->plum;
			break;
		case Symbols::MELON: return this->melon;
			break;
		case Symbols::CITRUS: return this->zitrapattoni;
			break;
		case Symbols::ORANGE: return this->orange;
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
		//srand(time(NULL));
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
