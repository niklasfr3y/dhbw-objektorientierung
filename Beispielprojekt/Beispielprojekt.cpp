#include "stdafx.h"

#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

#include <vector>
#include <string>
#include <iostream>

#include "Vektor2d.h"
#include <map>
#include <iomanip>
#include <sstream>
#include <ctime>

using namespace std;

// Simulationsgeschwindigkeit
const double DT = 100.0;

class GameWindow : public Gosu::Window
{
	enum Symbols {
		APPLE = 0,
		CHERRY = 1,
		PLUM = 2,
		MELON = 3,
		CITRUS = 4,
		GRAPE = 5,
		BAR = 6,
		SEVEN = 7,
		BIGWIN = 8
	};

	Symbols reference[5][9];
	int reference_Co[5][9];
	Gosu::Image reference_Visual[5][9];

	Symbols winner_matrix[5][3];
	Symbols winning_lines[9][5];

	Gosu::Image seven;
	Gosu::Image melon;
	Gosu::Image plum;
	Gosu::Image zitrapattoni;
	Gosu::Image apple;
	Gosu::Image bIGWIN;
	Gosu::Image cherryLady;
	Gosu::Image barBarBar;
	Gosu::Image grape;
	Gosu::Image background;
	Gosu::Image innen;
	Gosu::Image pointer;
	Gosu::Image lines_image;
	Gosu::Image info_image;
	Gosu::Image gamble_image;
	Gosu::Image knossi_image;
	Gosu::Image monte_image;

	Gosu::Sample start_sound;

	Gosu::Font f_amount = Gosu::Font(50, "amount");

	std::map<Symbols, Gosu::Image> translation;

	int counter_rotations[5];
	int winners[5];
	int factor[5] = { 20, 25, 25, 20, 25 };

	//to regulate button presses and draw() calls
	bool started;
	bool gamble;
	bool lines;
	bool info;
	bool choice;
	bool knossi;
	bool gamble_switch;
	int gamble_random;

	double x_mouse;
	double y_mouse;

	double credit;
	double amount;
	double payout;
	double gamble_payout;

public:

	GameWindow()
		: Window(1600, 1000), seven("DIE SIEBEN DU HUND.png"), melon("MELOOOONE.png"), plum("IT'S  A MOTHERFUCKIN' PLUM.png"), zitrapattoni("ZITR(APATT)ONI.png")
		, apple("APPLE.png"), bIGWIN("JAAACKPOOOOOT.png"), cherryLady("JUICYCHERRY.png"), barBarBar("ES REGNET BARES BITCHES.png"), grape("GRAPE.png")
		, background("Slotti.png"), innen("Innen.png"), pointer("pointer.png"), lines_image("Linien.png"), info_image("Informationen.png"), gamble_image("Gamble.png")
		, knossi_image("knossi.png"), monte_image("monte.png"), start_sound("start_sound.wav")


	{
		this->credit = 500;
		this->payout = 0;
		this->gamble_payout = 0;
		this->amount = 0.1;
		this->info = false;
		this->started = false;
		this->gamble = false;
		this->choice = false;
		this->knossi = false;		// false == Knossi; true == monte
		this->gamble_switch = false;
		this->lines = false;
		set_caption("Slotti");
	}

	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		this->background.draw(0, 0, 1.0);
		this->pointer.draw(this->x_mouse, this->y_mouse, 10.0, 0.4, 0.4);

		std::stringstream stream_amount;
		stream_amount << std::fixed << std::setprecision(2) << this->amount << " $";
		std::string s_amount = stream_amount.str();
		this->f_amount.draw(s_amount, 80, 430, 2.0, 1.0, 1.0, Gosu::Color::WHITE, Gosu::AlphaMode::AM_DEFAULT);

		std::stringstream stream_credit;
		stream_credit << std::fixed << std::setprecision(2) << this->credit << " $";
		std::string s_credit = stream_credit.str();
		this->f_amount.draw(s_credit, 600, 897, 2.0, 1.0, 1.0, Gosu::Color::WHITE, Gosu::AlphaMode::AM_DEFAULT);

		std::stringstream stream_payout;
		stream_payout << std::fixed << std::setprecision(2) << this->payout << " $";
		std::string s_payout = stream_payout.str();
		this->f_amount.draw(s_payout, 1130, 897, 2.0, 1.0, 1.0, Gosu::Color::WHITE, Gosu::AlphaMode::AM_DEFAULT);

		if (this->gamble) {
			this->gamble_image.draw(300, 150, 3);

			std::stringstream stream_payout;
			stream_payout << std::fixed << std::setprecision(2) << this->payout << " $";
			std::string s_payout = stream_payout.str();
			this->f_amount.draw(s_payout, 520, 305, 4.0, 1.0, 1.0, Gosu::Color::WHITE, Gosu::AlphaMode::AM_DEFAULT);

			std::stringstream stream_gamble;
			stream_gamble << std::fixed << std::setprecision(2) << this->gamble_payout << " $";
			std::string s_gamble = stream_gamble.str();
			this->f_amount.draw(s_gamble, 960, 305, 4.0, 1.0, 1.0, Gosu::Color::WHITE, Gosu::AlphaMode::AM_DEFAULT);

			if (!this->choice && this->gamble_switch) {
				this->knossi_image.draw(715, 457, 4);
			}
			else if (!this->choice && !this->gamble_switch) {
				this->monte_image.draw(715, 457, 4);
			}
			else if (this->choice) {
				if (this->gamble_random == 1) {
					this->knossi_image.draw(715, 457, 4);
					Sleep(500);
				}
				else {
					this->monte_image.draw(715, 457, 4);
					Sleep(500);
				}
				this->gamble_random = rand() % 2;

			}
			//alternates images
			this->gamble_switch = !this->gamble_switch;

		}
		else if (this->lines) {
			this->innen.draw(300, 150, 0);
			this->lines_image.draw(300, 200, 2);
			fillRollsVisual();
		}
		else if (this->info) {
			this->info_image.draw(300, 150, 3);
		}
		else {
			this->innen.draw(300, 150, 0);
			fillRollsVisual();
		}
	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		//mouse press anywhere to return to 'main screen'
		if ((this->lines || this->info) && !this->gamble && this->x_mouse >= 0 && this->x_mouse <= 1600 && this->y_mouse >= 0 && this->y_mouse <= 1000 && input().down(Gosu::MS_LEFT)) {
			this->lines = false;
			this->info = false;
		}

		this->x_mouse = input().mouse_x();
		this->y_mouse = input().mouse_y();

		//increase betting amount
		if (!this->gamble && !this->started && this->x_mouse >= 90 && this->x_mouse <= 125 && this->y_mouse >= 370 && this->y_mouse <= 405 && input().down(Gosu::MS_LEFT)) {
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

		//decrease betting amount
		if (!this->gamble && !this->started && this->x_mouse >= 160 && this->x_mouse <= 200 && this->y_mouse >= 365 && this->y_mouse <= 405 && input().down(Gosu::MS_LEFT)) {
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

		//set max betting amount
		if (!this->started && !this->gamble && this->x_mouse >= 49 && this->x_mouse <= 249 && this->y_mouse >= 749 && this->y_mouse <= 849 && input().down(Gosu::MS_LEFT)) {
			this->amount = 20.0;
		}

		//declare that a choice has been made in 'gamble'
		if (this->gamble && !this->started && this->choice) {
			this->choice = false;
		}

		//gamble 
		if (this->payout > 0 && !this->gamble && !this->choice && !this->started && this->x_mouse >= 1349 && this->x_mouse <= 1549 && this->y_mouse >= 349 && this->y_mouse <= 549 && input().down(Gosu::MS_LEFT)) {
			cout << "gamble" << endl;
			//for drawing purposes
			this->gamble = true;
			this->choice = false;
			this->gamble_payout = this->payout * 2;
			srand(time(NULL));
			this->gamble_random = rand() % 2;
			this->credit -= this->payout;
			//wait to avoid multiple presses
			Sleep(200);
		}

		//button in 'gamble'
		if (this->gamble && !this->started && this->x_mouse >= 700 && this->x_mouse <= 900 && this->y_mouse >= 699 && this->y_mouse <= 799 && input().down(Gosu::MS_LEFT)) {
			cout << "Nehmen" << endl;
			//for drawing purposes
			this->credit += this->payout;
			this->choice = true;
			this->gamble = false;
			//wait to avoid multiple presses
			Sleep(200);
		}

		//button in 'gamble'
		if (this->payout > 0 && this->gamble && !this->started && this->x_mouse >= 425 && this->x_mouse <= 600 && this->y_mouse >= 462 && this->y_mouse <= 652 && input().down(Gosu::MS_LEFT)) {
			cout << "Knossi" << endl;
			this->choice = true;
			this->knossi = true;
			if (this->gamble_random == 1) {
				this->payout = this->gamble_payout;
				this->gamble_payout *= 2;
			}
			else {
				this->payout = 0;
				this->gamble_payout = 0;
			}
			//wait to avoid multiple presses
			Sleep(200);
		}

		//button in 'gamble'
		if (this->payout > 0 && this->gamble && !this->started && this->x_mouse >= 999 && this->x_mouse <= 1174 && this->y_mouse >= 462 && this->y_mouse <= 652 && input().down(Gosu::MS_LEFT)) {
			cout << "Montana" << endl;
			this->choice = true;
			this->knossi = false;
			if (this->gamble_random == 0) {
				this->payout = this->gamble_payout;
				this->gamble_payout *= 2;
			}
			else {
				this->payout = 0;
				this->gamble_payout = 0;
			}
			//wait to avoid multiple presses
			Sleep(200);
		}

		//info '?' field
		if (!this->info && !this->gamble && !this->started && this->x_mouse >= 100 && this->x_mouse <= 200 && this->y_mouse >= 100 && this->y_mouse <= 200 && input().down(Gosu::MS_LEFT)) {
			cout << "Info" << endl;
			this->info = true;
			//wait to avoid multiple presses
			Sleep(200);
		}

		//lines-button showing all possible combinations
		if (!this->lines && !this->gamble && !this->started && this->x_mouse >= 49 && this->x_mouse <= 249 && this->y_mouse >= 599 && this->y_mouse <= 699 && input().down(Gosu::MS_LEFT)) {
			cout << "Linien" << endl;
			this->lines = true;
			//wait to avoid multiple presses
			Sleep(200);
		}

		//start the actual slot machine
		if (this->amount <= this->credit && !this->gamble && !this->started && this->x_mouse >= 1349 && this->x_mouse <= 1549 && this->y_mouse >= 749 && this->y_mouse <= 849 && input().down(Gosu::MS_LEFT)) {
			this->start_sound.play();
			fillRollsMatrix();
			initReferences();
			this->credit -= this->amount;
			this->started = true;
			this->choice = false;
			this->gamble = false;
		}

		if (started) {
			//spin wheels until winner coordinate is reached
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 9; j++) {
					this->reference_Co[i][j] += this->factor[i];
					//check if winner coordinate is reached
					if (this->reference_Co[i][j] == (this->winners[i] - this->counter_rotations[i] * 1800)) {
						this->factor[i] = 0;
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
				}
				this->factor[0] = 25;
				this->factor[1] = 20;
				this->factor[2] = 25;
				this->factor[3] = 20;
				this->factor[4] = 25;

				//reset bool, so that other menu items can be accessed
				this->started = false;

				this->payout = calcPayout();
				this->credit += this->payout;
				cout << this->credit << endl;
			}
		}
	}

	//organizing all winning line possibilities
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

	//calculates the won amount
	double calcPayout() {
		fillWinningLines();
		double temp = 0;
		for (int i = 0; i < 9; i++) {
			double factorForPayout = 0;
			Symbols current = this->winning_lines[i][0];
			int counter = 1;
			while (current == this->winning_lines[i][counter]) {
				//calculate multiplicator
				switch (counter) {
				case 1: if (this->winning_lines[i][0] == Symbols::BAR) { factorForPayout = 1; }
						else if (this->winning_lines[i][0] == Symbols::SEVEN) { factorForPayout = 1.5; }
						else if (this->winning_lines[i][0] == Symbols::BIGWIN) { factorForPayout = 1; }
						else { factorForPayout = 0; }
						break;
				case 2: if (this->winning_lines[i][0] == Symbols::BAR) { factorForPayout = 2; }
						else if (this->winning_lines[i][0] == Symbols::SEVEN) { factorForPayout = 4; }
						else if (this->winning_lines[i][0] == Symbols::BIGWIN) { factorForPayout = 2; }
						else { factorForPayout = 1; }
						break;
				case 3: if (this->winning_lines[i][0] == Symbols::BAR) { factorForPayout = 10; }
						else if (this->winning_lines[i][0] == Symbols::SEVEN) { factorForPayout = 15; }
						else if (this->winning_lines[i][0] == Symbols::BIGWIN) { factorForPayout = 10; }
						else { factorForPayout = 5; }
						break;
				case 4: if (this->winning_lines[i][0] == Symbols::BAR) { factorForPayout = 20; }
						else if (this->winning_lines[i][0] == Symbols::SEVEN) { factorForPayout = 40; }
						else if (this->winning_lines[i][0] == Symbols::BIGWIN) { factorForPayout = 20; }
						else { factorForPayout = 10; }
						break;
				}
				current = this->winning_lines[i][counter];
				counter++;
				if (counter == 5) break;
			}
			temp += (this->amount * factorForPayout);
		}
		return temp;

	}

	//fills the 5 wheels, 9 each, random order
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
			double zf = rand();
			int j = int(zf) % (5 - h);

			//swap h with j
			int temp = this->factor[4 - h];
			this->factor[4 - h] = this->factor[j];
			this->factor[j] = temp;

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
	}

	//sets up a map with Symbols as key, Gosu::Image as value
	void initReferences() {
		for (int i = 0; i < 9; i++) {
			translation.insert(std::pair<Symbols, Gosu::Image>((Symbols)i, returnCorrespondingImage(i)));
		}

		//distribute y coordinate accordingly
		int y_start = 0;
		for (int h = 0; h < 5; h++) {
			for (int i = 0; i < 9; i++) {
				this->reference_Co[h][i] = y_start;
				y_start += 200;
			}
			y_start = 0;
		}
	}

	//returns the corresponding image to the given Symbol
	Gosu::Image returnCorrespondingImage(int reference) {
		switch ((Symbols)reference) {
		case Symbols::APPLE: return this->apple;
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
		case Symbols::GRAPE: return this->grape;
			break;
		}
	}

	//draws the 5 wheels with each image at the given x/y coordinate
	void fillRollsVisual() {
		//reference 314 * column_index
		int x_co = 314;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 9; j++) {
				map<Symbols, Gosu::Image>::iterator it = this->translation.find(this->reference[i][j]);
				it->second.draw(x_co, this->reference_Co[i][j], 0.5, 0.9, 1);
			}
			x_co += 200;
		}
	}

	//returns the winning index, indicates when each wheel should stop
	int returnWinningIndex() {
		//decide on which Symbol to stop
		//srand(time(NULL));
		int w = 5400 + (rand() % (9000 - 5400 + 1));
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
