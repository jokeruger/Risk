//
//  main.cpp
//  Risk
//
//  Created by Joe Kruger on 3/9/15.
//  Copyright (c) 2015 Joe Kruger. All rights reserved.
//

#include <iostream>
using namespace std;
#include "CImg.h"
using namespace cimg_library;
#include "Country.h"
#include "Player.h"
#include <iomanip>

/*
 to do:
 error checking, prevent closing window
 
 stats, graphs, attack history
 TIME LAPSE screenshots of board each turn
 
 history, to undo
 
 auto attacks and defends with most available; add key to change
 
 overlay instructions with brief highlights instead of console
*/

#include <random>

CImg<unsigned char> image("Risk4.bmp");
CImgDisplay main_disp(image,"Click a point");
int menuX = 20;
int menuY = 400;
int numberOfCountries = 42;
Country countries[42] = {
	//North America
	Country(57, 99, "Akaska", 221, 192, 87),
	Country(132, 103, "Northwest Territory", 221, 192, 86),
	Country(315, 65, "Greenland", 221, 192, 85),
	Country(134, 146, "Alberta", 221, 192, 84),
	Country(184, 150, "Ontario", 221, 192, 83),
	Country(247, 150, "Quebec", 221, 192, 82),
	Country(136, 196, "Western US", 221, 192, 81),
	Country(192, 207, "Eastern US", 221, 192, 80),
	Country(142, 240, "Central America", 221, 192, 79),
	//South America
	Country(173, 310, "Venezuela", 214, 72, 87),
	Country(192, 377, "Peru", 214, 72, 86),
	Country(244, 351, "Brazil", 214, 72, 85),
	Country(197, 436, "Argentina", 214, 72, 84),
	//Europe
	Country(328, 130, "Iceland", 147, 164, 172),
	Country(377, 124, "Scandanavia", 147, 164, 173),
	Country(325, 183, "Great Britian", 147, 164, 174),
	Country(376, 179, "Northern Europe", 147, 164, 175),
	Country(442, 163, "Ukraine", 147, 164, 176),
	Country(335, 232, "Western Europe", 147, 164, 177),
	Country(404, 213, "Southern Europe", 147, 164, 178),
	//Africa
	Country(339, 324, "North Africa", 154, 122, 96),
	Country(407, 285, "Egypt", 154, 122, 95),
	Country(397, 386, "Congo", 154, 122, 94),
	Country(429, 340, "East Africa", 154, 122, 93),
	Country(394, 463, "South Africa", 154, 122, 92),
	Country(472, 456, "Madagascar", 154, 122, 91),
	//Asia
	Country(531, 135, "Ural", 168, 200, 92),
	Country(587, 124, "Siberia", 168, 200, 91),
	Country(660, 103, "Yakutsk", 168, 200, 90),
	Country(730, 100, "Kamchatka", 168, 200, 89),
	Country(631, 151, "Irkutsk", 168, 200, 88),
	Country(656, 191, "Mongolia", 168, 200, 87),
	Country(520, 196, "Afghanistan", 168, 200, 86),
	Country(602, 233, "China", 168, 200, 85),
	Country(458, 258, "Middle East", 168, 200, 84),
	Country(551, 288, "India", 168, 200, 83),
	Country(606, 293, "Siam", 168, 200, 82),
	Country(717, 210, "Japan", 168, 200, 81),
	//Australia
	Country(652, 343, "Indonesia", 163, 71, 153),
	Country(718, 358, "New Guinea", 163, 71, 152),
	Country(628, 442, "Western Australia", 163, 71, 151),
	Country(700, 442, "Eastern Australia", 163, 71, 150)};
int radius = 8;
int numberOfPlayers = 0;	//number of players currently in the game
bool chosen;
int attacking = 3;
int defending = 2;
int phase = 0;
int turn = 0;
int id1 = -1;

unsigned char black[]	= {0,	0,	0};
unsigned char white[]	= {255,	255,255};
unsigned char red[]		= {255,	0,	0};
unsigned char green[]	= {0,	255,0};
unsigned char yellow[]	= {255,	255,0};
unsigned char teal[]	= {0,	255,255};
unsigned char pink[]	= {255,	0,	255};
unsigned char blue[]	= {100,	100,255};
unsigned char colors[7][3] = {{255,	255,255},//white
	{255,	0,		0},		//red
	{0,		255,	0},		//greeen
	{255,	255,	0},		//yellow
	{0,		255,	255},	//teal
	{255,	0,		255},	//pink
	{100,	100,	255}};	//blue

Player players[7] = {Player(), Player(), Player(), Player(), Player(), Player(), Player()};


int getCountryId(int x, int y, int n){
	//returns 100 for white (continue button)
	
	int xOriginal = x;
	int yOriginal = y;
//	cout << x << "," << y << " " << n <<endl;
	
	//search around
	bool vertical = (n%2==1);
	bool positive = (((n%4)/2)==1);
	int	 distance = (n+3)/4;
//	cout << "vertical " << vertical << "  positive " << positive << "  distance " << distance << endl;
	if (n>0){
		if (vertical){
			if (positive)
				y = y + distance;
			else y = y - distance;
		}
		else {
			if (positive)
				x = x + distance;
			else x = x - distance;
		}
	}
//	cout << x << ", " << y << endl;
	if (x>0 && x<image.width() && y>0 && y<image.height()){
		int r = (int)image(x,y,0,0);
		int g = (int)image(x,y,0,1);
		int b = (int)image(x,y,0,2);
//		cout << r << ", " << g << ", " << b << endl;
		if (r==255 && g==255 && b==255) {
			return 100;
		}
		for (int i=0; i<numberOfCountries; i++){
			if (countries[i].getR()==r && countries[i].getG()==g && countries[i].getB()==b) {
				return i;
			}
		}
	}
	return getCountryId(xOriginal, yOriginal, n+1);
}

void drawThickCircle(int x, int y, int r, unsigned char color[]) {
	image.draw_circle(x, y, r, color, 1L,-1.0f);
	image.draw_circle(x+1, y, r, color, 1L,-1.0f);
	image.draw_circle(x, y+1, r, color, 1L,-1.0f);
	image.draw_circle(x-1, y, r, color, 1L,-1.0f);
	image.draw_circle(x, y-1, r, color, 1L,-1.0f);
}

void refreshMap(){
	image.load("Risk4.bmp");
//	image.draw_text(menuX, menuY, "Place Armies", white, 0, 1.0f, 26);
	
	image.draw_text(menuX, menuY, " Place Armies ", white);
	image.draw_text(menuX, menuY+15, " Exchange cards ", white);
	image.draw_text(menuX, menuY+30, " Attack ", white);
	image.draw_text(menuX, menuY+45, " Tactical ", white);
	
	switch (phase) {
		case 1:
			image.draw_text(menuX, menuY, " Place Armies ", 10, colors[turn]);
			break;
		case 2:
			image.draw_text(menuX, menuY+15, " Exchange cards ", 10, colors[turn]);
			break;
		case 3:
			image.draw_text(menuX, menuY+30, " Attack ", 10, colors[turn]);
			break;
		case 4:
			image.draw_text(menuX, menuY+45, " Tactical ", 10, colors[turn]);
			break;
	}
	
	if (phase==2 || phase==3 || phase==4) {
		for (int i=0; i<45; i++) {
			image.draw_circle(menuX+15+i, menuY+80, radius*1.2, white, 1);
		}
		image.draw_text(menuX+11, menuY+74, " Continue ", black);
	}
	
	int size;
	for (int i=0; i< numberOfCountries; i+=1) {
		size = radius-3 + countries[i].getArmies()*4;
		switch (countries[i].getOwner()) {
			case 0:
				image.draw_circle(countries[i].getX(), countries[i].getY(), radius, white, 1);
				drawThickCircle(countries[i].getX(), countries[i].getY(), size, white);
				break;
			case 1:
				image.draw_circle(countries[i].getX(), countries[i].getY(), radius, red, 1);
				drawThickCircle(countries[i].getX(), countries[i].getY(), size, red);
				break;
			case 2:
				image.draw_circle(countries[i].getX(), countries[i].getY(), radius, green, 1);
				drawThickCircle(countries[i].getX(), countries[i].getY(), size, green);
				break;
			case 3:
				image.draw_circle(countries[i].getX(), countries[i].getY(), radius, yellow, 1);
				drawThickCircle(countries[i].getX(), countries[i].getY(), size, yellow);
				break;
			case 4:
				image.draw_circle(countries[i].getX(), countries[i].getY(), radius, teal, 1);
				drawThickCircle(countries[i].getX(), countries[i].getY(), size, teal);
				break;
			case 5:
				image.draw_circle(countries[i].getX(), countries[i].getY(), radius, pink, 1);
				drawThickCircle(countries[i].getX(), countries[i].getY(), size, pink);
				break;
			case 6:
				image.draw_circle(countries[i].getX(), countries[i].getY(), radius, blue, 1);
				drawThickCircle(countries[i].getX(), countries[i].getY(), size, blue);
				break;
			default:
				image.draw_circle(countries[i].getX(), countries[i].getY(), radius, black, 1);
				break;
		}
		int armies = countries[i].getArmies();
		int offset;
		if (armies < 10)
			offset = 2;
		else
			offset = 5;
		image.draw_text(countries[i].getX()-offset, countries[i].getY()-6,"%u", black, 0, 1.0f, 13, (unsigned int)countries[i].getArmies());
	}
	main_disp.display(image);
}

void pickCountry (int player) {
	main_disp.wait();
	if (main_disp.button() && main_disp.mouse_y()>=0) {
		const int x = main_disp.mouse_x();
		const int y = main_disp.mouse_y();
		int id = getCountryId(x, y, 0);
		
		if (countries[id].getOwner()==0) {
			cout << countries[id].getName() << endl<<endl;
			countries[id].addArmies(1);
			countries[id].setOwner(player);
			refreshMap();
			chosen = true;
		}
	}
}

void addToCountry (int player) {
	main_disp.wait();
	if (main_disp.button() && main_disp.mouse_y()>=0) {
		const int x = main_disp.mouse_x();
		const int y = main_disp.mouse_y();
		int id = getCountryId(x, y, 0);
		
		if (countries[id].getOwner()==player) {
			countries[id].addArmies(1);
			refreshMap();
			chosen = true;
		}
		else cout << "Not your country!" << endl;
	}
}

void distributeArmies() {
	int max;
	switch (numberOfPlayers) {
		case 2:
			max = 40-numberOfCountries/2;
			for (int i=0; i < max; i++) {
				cout << endl << "Red, place an army (" << max-i << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(1);
				}
				cout << endl << "Green, place an army (" << max-i << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(2);
				}
			}
			break;
		case 3:
			max = 35-numberOfCountries/3;
			for (int i=0; i < max; i++) {
				cout << endl << "Red, place an army (" << max-i << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(1);
				}
				cout << endl << "Green, place an army (" << max-i << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(2);
				}
				cout << endl << "Yellow, place an army (" << max-i << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(3);
				}
			}
			break;
		case 4:
			max = 30-numberOfCountries/4;
			for (int i=0; i < max; i++) {
				cout << endl << "Red, place an army (" << max-i << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(1);
				}
				cout << endl << "Green, place an army (" << max-i << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(2);
				}
				cout << endl << "Yellow, place an army (" << max-i +1 << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(3);
				}
				cout << endl << "Teal, place an army (" << max-i +1 << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(4);
				}
			}
			cout << endl << "Yellow, place an army (" << "1" << " remaining)" << endl;
			chosen = false;
			while (!chosen) {
				addToCountry(3);
			}
			cout << endl << "Teal, place an army (" << "1" << " remaining)" << endl;
			chosen = false;
			while (!chosen) {
				addToCountry(4);
			}
			break;
		case 5:
			max = 25-numberOfCountries/5;
			for (int i=0; i < max; i++) {
				cout << endl << "Red, place an army (" << max-i << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(1);
				}
				cout << endl << "Green, place an army (" << max-i << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(2);
				}
				cout << endl << "Yellow, place an army (" << max-i << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(3);
				}
				cout << endl << "Teal, place an army (" << max-i +1 << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(4);
				}
				cout << endl << "Pink, place an army (" << max-i +1 << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(5);
				}
			}
			cout << endl << "Teal, place an army (" << "1" << " remaining)" << endl;
			chosen = false;
			while (!chosen) {
				addToCountry(4);
			}
			cout << endl << "Pink, place an army (" << "1" << " remaining)" << endl;
			chosen = false;
			while (!chosen) {
				addToCountry(5);
			}
		case 6:
			max = 20-numberOfCountries/6;
			for (int i=0; i < max; i++) {
				cout << endl << "Red, place an army (" << max-i << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(1);
				}
				cout << endl << "Green, place an army (" << max-i << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(2);
				}
				cout << endl << "Yellow, place an army (" << max-i << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(3);
				}
				cout << endl << "Teal, place an army (" << max-i << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(4);
				}
				cout << endl << "Pink, place an army (" << max-i +1 << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(5);
				}
				cout << endl << "Blue, place an army (" << max-i +1 << " remaining)" << endl;
				chosen = false;
				while (!chosen) {
					addToCountry(6);
				}
			}
			cout << endl << "Pink, place an army (" << "1" << " remaining)" << endl;
			chosen = false;
			while (!chosen) {
				addToCountry(5);
			}
			cout << endl << "Blue, place an army (" << "1" << " remaining)" << endl;
			chosen = false;
			while (!chosen) {
				addToCountry(6);
			}
	}
}

void distributeArmiesRandomly() {
	int max = 50-numberOfPlayers*5;
	int cIndex20[840] {0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
		0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
		22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,
	};
	random_device rd;
	mt19937 g(rd());
	shuffle(std::begin(cIndex20),std::end(cIndex20), g);
	for (int i=0; i<840; i++) {
		int thisPlayer =countries[cIndex20[i]].getOwner();
		if (players[thisPlayer].getArmies()<max) {
			countries[cIndex20[i]].addArmies(1);
			players[thisPlayer].addArmy();
			refreshMap();
		}
	}
}

void startGame() {
	cout << "Number of players? (2-6) \n";
	while (!main_disp.is_closed()) {
		main_disp.wait();
		if (main_disp.is_key2()) {
			numberOfPlayers = 2;
			break;
		}
		if (main_disp.is_key3()) {
			numberOfPlayers = 3;
			break;
		}
		if (main_disp.is_key4()) {
			numberOfPlayers = 4;
			break;
		}
		if (main_disp.is_key5()) {
			numberOfPlayers = 5;
			break;
		}
		if (main_disp.is_key6()) {
			numberOfPlayers = 6;
			break;
		}
	}
	cout << numberOfPlayers << " player game \n\n";
	refreshMap();
	
	for (int i=1; i<=numberOfPlayers; i++) {
		players[i].setAlive(true);
	}
	
	cout << "[R]andom countries, or [C]hoose? \n";
	while (!main_disp.is_closed()) {
		main_disp.wait();
		
		//Random
		if (main_disp.is_keyR()) {
			cout << "Randomly distributing countries...\n\n";
			int cIndex[42] {0, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
				22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41};
			random_device rd;
			mt19937 g(rd());
			shuffle(std::begin(cIndex),std::end(cIndex),g);
			
			for (int i=0; i<42; i++) {
				countries[cIndex[i]].setOwner(i%numberOfPlayers+1);
				countries[cIndex[i]].addArmies(1);
				players[i%numberOfPlayers+1].addArmy();
				players[i%numberOfPlayers+1].addCountry();
				refreshMap();
			}
			break;
		}
		
		//Choose
		if (main_disp.is_keyC()) {
			for (int i = 0; i<numberOfCountries; i++) {
				switch (i%numberOfPlayers) {
					case 0:
						cout << "Red, choose a country" << endl;
						chosen = false;
						while (!chosen) {
							pickCountry(1);
						}
						players[1].addArmy();
						players[1].addCountry();
						break;
					case 1:
						cout << "Green, choose a country" << endl;
						chosen = false;
						while (!chosen) {
							pickCountry(2);
						}
						players[2].addArmy();
						players[2].addCountry();
						break;
					case 2:
						cout << "Yellow, choose a country" << endl;
						chosen = false;
						while (!chosen) {
							pickCountry(3);
						}
						players[3].addArmy();
						players[3].addCountry();
						break;
					case 3:
						cout << "Teal, choose a country" << endl;
						chosen = false;
						while (!chosen) {
							pickCountry(4);
						}
						players[4].addArmy();
						players[4].addCountry();
						break;
					case 4:
						cout << "Pink, choose a country" << endl;
						chosen = false;
						while (!chosen) {
							pickCountry(5);
						}
						players[5].addArmy();
						players[5].addCountry();
						break;
					case 5:
						cout << "Blue, choose a country" << endl;
						chosen = false;
						while (!chosen) {
							pickCountry(6);
							players[6].addCountry();
						}
						players[6].addArmy();
						break;
				}
			}
			break;
		}
	}
	
	phase = 1;
	refreshMap();
	cout << "[R]andom armies or [C]hoose distribution? \n";
	while (!main_disp.is_closed()) {
		main_disp.wait();
		
		//Random
		if (main_disp.is_keyR()) {
			cout << "Radomizing...\n\n";
			distributeArmiesRandomly();
			break;
		}
		
		//Choose
		if (main_disp.is_keyC()) {
			distributeArmies();
			break;
		}
	}
	turn = 1;
	refreshMap();
	cout << " - Player 1's turn - " << endl;
}

tuple<int, int, int> rollDice(int dice){
	random_device rd;
	int die1, die2, die3;
	switch (dice) {
		case 1:
			die1 = rd()%6+1;
			return make_tuple(die1, 0, 0);
		case 2:
			die1 = rd()%6+1;
			die2 = rd()%6+1;
			return make_tuple(die1, die2, 0);
		default:
			die1 = rd()%6+1;
			die2 = rd()%6+1;
			die3 = rd()%6+1;
			return make_tuple(die1, die2, die3);
	}
	return make_tuple(0, 0, 0);
}

int checkDice(){
	int sum;
	int rollResults[13] = { };
	
	for (int i = 0; i<100000; i++){
		tuple<int, int, int> rolls = rollDice(2);
		sum = get<0>(rolls) + get<1>(rolls);
		rollResults[sum]=rollResults[sum]+1;
	}
	
	int spot = 0;
	for (int i : rollResults) {
		cout << spot << ": " << i << "\n";
		spot++;
	}
	return 0;
}

tuple<char, int> decideRolls(int diceA, int diceD){
	tuple<int, int, int> rollsA = rollDice(diceA);	//player a dice (attacker)
	tuple<int, int, int> rollsD = rollDice(diceD);	//player d dice
	cout << "A: " << get<0>(rollsA) << get<1>(rollsA) << get<2>(rollsA);
	cout << "  D: " << get<0>(rollsD) << get<1>(rollsD) << get<2>(rollsD) << endl;
	int highA=0, lowA=0, highD=0, lowD=0;						//player a/d high/low rolls
	int highAindex = -1, highDindex = -1;			//keeps track of which dice have already been decided on
	
	//player a highest roll
	if (get<0>(rollsA) >= get<1>(rollsA) && get<0>(rollsA) >= get<2>(rollsA)){
		highA = get<0>(rollsA);
		highAindex=0;
	}
	if (get<1>(rollsA) >= get<0>(rollsA) && get<1>(rollsA) >= get<2>(rollsA)){
		highA = get<1>(rollsA);
		highAindex=1;
	}
	if (get<2>(rollsA) >= get<0>(rollsA) && get<2>(rollsA) >= get<1>(rollsA)){
		highA = get<2>(rollsA);
		highAindex=2;
	}
	
	//player d highest roll
	if (get<0>(rollsD) >= get<1>(rollsD) && get<0>(rollsD) >= get<2>(rollsD)){
		highD = get<0>(rollsD);
		highDindex = 0;
	}
	if (get<1>(rollsD) >= get<0>(rollsD) && get<1>(rollsD) >= get<2>(rollsD)){
		highD = get<1>(rollsD);
		highDindex = 1;
	}
	if (get<2>(rollsD) >= get<0>(rollsD) && get<2>(rollsD) >= get<1>(rollsD)){
		highD = get<2>(rollsD);
		highDindex = 2;
	}
	
	if (diceA>=2 && diceD>=2){		//both players need to have rolled more than two dice
		//player a lower roll
		if (highAindex==0){
			if (get<1>(rollsA) > get<2>(rollsA)){
				lowA = get<1>(rollsA);
			}
			else lowA = get<2>(rollsA);
		}
		if (highAindex==1){
			if (get<0>(rollsA) > get<2>(rollsA)){
				lowA = get<0>(rollsA);
			}
			else lowA = get<2>(rollsA);
		}
		if (highAindex==2){
			if (get<1>(rollsA) > get<0>(rollsA)){
				lowA = get<1>(rollsA);
			}
			else lowA = get<0>(rollsA);
		}
		
		//player b lower roll
		if (highDindex==0){
			if (get<1>(rollsD) > get<2>(rollsD)){
				lowD = get<1>(rollsD);
			}
			else lowD = get<2>(rollsD);
		}
		if (highDindex==1){
			if (get<0>(rollsD) > get<2>(rollsD)){
				lowD = get<0>(rollsD);
			}
			else lowD = get<2>(rollsD);
		}
		if (highDindex==2){
			if (get<1>(rollsD) > get<0>(rollsD)){
				lowD = get<1>(rollsD);
			}
			else lowD = get<0>(rollsD);
		}
		
		cout << "A: " << highA << " " << lowA;
		cout << "  D: " << highD << " " << lowD << endl;
		
		//final results for 2 rolls
		if (highA > highD && lowA > lowD) {
			return make_tuple('D', 2);				//b loses 2
		}
		else if (highD >= highA && lowD >= lowA) {
			return make_tuple('A', 2);				//a loses 2
		}
		
		else return make_tuple('X', 1);				//each lose 1
	}
	
	cout << "A: " << left << setw(3) << highA;
	cout << "  D: " << highD << endl;

	//final results for 1 roll
	if (highA>highD) {
		return make_tuple('D', 1);		//d loses 1
	}
	else return make_tuple('A', 1);		//a loses 1
}

int testRollingJudgement(){
	for (int i=0; i<10; i++){
		tuple<char, int> result = decideRolls(3, 2);
		if (get<0>(result)=='X') {
			cout << "Both players lose 1 army" << endl;
		}
		else cout << "Player " << get<0>(result) << " loses " << get<1>(result) << endl;
	}
	return 0;
}

void placeArmies(int player){
	int armies = 0;
	
//	cout << players[player].getCountries() << endl;
	
	if (players[player].getCountries()/3<11){
		armies+=3;
	}
	else {
		armies += players[player].getCountries()/3;
	}
	
	for (int i=0; i<armies;i++) {
		image.draw_text(menuX + 80 + i*10, menuY, " * ", 10, colors[turn]);
	}
	
	main_disp.display(image);
	
//	cout << armies << endl;
	
	while (armies>0) {
		main_disp.wait();
		if (main_disp.button() && main_disp.mouse_y()>=0) {
			const int x = main_disp.mouse_x();
			const int y = main_disp.mouse_y();
			int id = getCountryId(x, y, 0);
			
			if (countries[id].getOwner()==player) {
//				cout << countries[id].getName() << endl<<endl;
				countries[id].addArmies(1);
				refreshMap();
				armies--;
				for (int i=0; i<armies;i++) {
					image.draw_text(menuX + 80 + i*10, menuY, " * ", 10, colors[turn]);
				}
				main_disp.display(image);
			}
		}
	}
	
	phase++;
	refreshMap();
}

bool bTouching(string c1, string c2) {
	//North America
	if (c1=="Alaska") {
		if (c2=="Kamchatka" || c2=="Northwest Territory" || c2=="Alberta") {
			return true;
		}
	}
	if (c1=="Northwest Territory") {
		if (c2=="Akaska" || c2=="Alberta" || c2=="Ontario" || c2=="Greenland") {
			return true;
		}
	}
	if (c1=="Greenland") {
		if (c2=="Northwest Territory" || c2=="Ontario" || c2=="Quebec" || c2=="Iceland") {
			return true;
		}
	}
	if (c1=="Alberta") {
		if (c2=="Akaska" || c2=="Northwest Territory" || c2=="Ontario" || c2=="Western US") {
			return true;
		}
	}
	if (c1=="Ontario") {
		if (c2=="Northwest Territory" || c2=="Alberta" || c2=="Quebec" || c2=="Greenland" || c2=="Eastern US" || c2=="Western US") {
			return true;
		}
	}
	if (c1=="Quebec") {
		if (c2=="Ontario" || c2=="Greenland" || c2=="Eastern US") {
			return true;
		}
	}
	if (c1=="Western US") {
		if (c2=="Alberta" || c2=="Ontario" || c2=="Eastern US" || c2=="Central America") {
			return true;
		}
	}
	if (c1=="Eastern US") {
		if (c2=="Central America" || c2=="Western US" || c2=="Ontario" || c2=="Quebec") {
			return true;
		}
	}
	if (c1=="Central America") {
		if (c2=="Western US" || c2=="Eastern US" || c2=="Venezuela") {
			return true;
		}
	}
	//South America
	if (c1=="Venezuela") {
		if (c2=="Central America" || c2=="Brazil" || c2=="Peru") {
			return true;
		}
	}
	if (c1=="Brazil") {
		if (c2=="Venezuela" || c2=="Peru" || c2=="Argentina" || c2=="North Africa") {
			return true;
		}
	}
	if (c1=="Peru") {
		if (c2=="Venezuela" || c2=="Brazil" || c2=="Argentina") {
			return true;
		}
	}
	if (c1=="Argentina") {
		if (c2=="Peru" || c2=="Brazil") {
			return true;
		}
	}
	//Africa
	if (c1=="North Africa") {
		if (c2=="Brazil" || c2=="Congo" || c2=="East Africa" || c2=="Egypt" || c2=="Southern Europe" || c2=="Western Europe") {
			return true;
		}
	}
	if (c1=="Egypt") {
		if (c2=="North Africa" || c2=="Southern Europe" || c2=="Middle East" || c2=="East Africa") {
			return true;
		}
	}
	if (c1=="East Africa") {
		if (c2=="North Africa" || c2=="Egypt" || c2=="Middle East" || c2=="South Africa" || c2=="Congo" || c2=="Madagascar") {
			return true;
		}
	}
	if (c1=="Congo") {
		if (c2=="North Africa" || c2=="East Africa" || c2=="South Africa") {
			return true;
		}
	}
	if (c1=="South Africa") {
		if (c2=="Congo" || c2=="East Africa" || c2=="Madagascar") {
			return true;
		}
	}
	if (c1=="Madagascar") {
		if (c2=="South Africa" || c2=="East Africa") {
			return true;
		}
	}
	//Europe
	if (c1=="Iceland") {
		if (c2=="Greenland" || c2=="Scandanavia" || c2=="Great Britian") {
			return true;
		}
	}
	if (c1=="Scandanavia") {
		if (c2=="Iceland" || c2=="Ukraine" || c2=="Northern Europe" || c2=="Great Britian") {
			return true;
		}
	}
	if (c1=="Ukraine") {
		if (c2=="Scandanavia" || c2=="Northern Europe" || c2=="Southern Europe" || c2=="Middle East" || c2=="Afghanistan" || c2=="Ural") {
			return true;
		}
	}
	if (c1=="Great Britian") {
		if (c2=="Iceland" || c2=="Scandanavia" || c2=="Northern Europe" || c2=="Western Europe") {
			return true;
		}
	}
	if (c1=="Northern Europe") {
		if (c2=="Great Britian" || c2=="Scandanavia" || c2=="Ukraine" || c2=="Southern Europe" || c2=="Western Europe") {
			return true;
		}
	}
	if (c1=="Western Europe") {
		if (c2=="Great Britian" || c2=="Northern Europe" || c2=="Southern Europe" || c2=="North Africa") {
			return true;
		}
	}
	if (c1=="Southern Europe") {
		if (c2=="Western Europe" || c2=="Northern Europe" || c2=="Ukraine" || c2=="Middle East" || c2=="Egypt" || c2=="North Africa") {
			return true;
		}
	}
	//Asia
	if (c1=="Ural") {
		if (c2=="Ukraine" || c2=="Siberia" || c2=="China" || c2=="Afghanistan") {
			return true;
		}
	}
	if (c1=="Siberia") {
		if (c2=="Ural" || c2=="Yakutsk" || c2=="Irkutsk" || c2=="Mongolia" || c2=="China") {
			return true;
		}
	}
	if (c1=="Yakutsk") {
		if (c2=="Siberia" || c2=="Kamchatka" || c2=="Irkutsk") {
			return true;
		}
	}
	if (c1=="Kamchatka") {
		if (c2=="Yakutsk" || c2=="Irkutsk" || c2=="Mongolia" || c2=="Japan" || c2=="Akaska") {
			return true;
		}
	}
	if (c1=="Irkutsk") {
		if (c2=="Siberia" || c2=="Yakutsk" || c2=="Kamchatka" || c2=="Mongolia") {
			return true;
		}
	}
	if (c1=="Mongolia") {
		if (c2=="Siberia" || c2=="Irkutsk" || c2=="Kamchatka" || c2=="Japan") {
			return true;
		}
	}
	if (c1=="Japan") {
		if (c2=="Mongolia" || c2=="Kamchatka") {
			return true;
		}
	}
	if (c1=="Afghanistan") {
		if (c2=="Ukraine" || c2=="Ural" || c2=="China" || c2=="India" || c2=="Middle East") {
			return true;
		}
	}
	if (c1=="China") {
		if (c2=="Afghanistan" || c2=="Ural" || c2=="Siberia" || c2=="Mongolia" || c2=="Siam" || c2=="India") {
			return true;
		}
	}
	if (c1=="Middle East") {
		if (c2=="Southern Europe" || c2=="Ukraine" || c2=="Afghanistan" || c2=="India" || c2=="Egypt" || c2=="East Africa") {
			return true;
		}
	}
	if (c1=="India") {
		if (c2=="Middle East" || c2=="Afghanistan" || c2=="China" || c2=="Siam") {
			return true;
		}
	}
	if (c1=="Siam") {
		if (c2=="India" || c2=="China" || c2=="Indonesia") {
			return true;
		}
	}
	//Australia
	if (c1=="Indonesia") {
		if (c2=="Siam" || c2=="New Guinea" || c2=="Western Australia") {
			return true;
		}
	}
	if (c1=="New Guinea") {
		if (c2=="Indonesia" || c2=="Western Australia" || c2=="Eastern Australia") {
			return true;
		}
	}
	if (c1=="Eastern Australia") {
		if (c2=="Western Australia" || c2=="New Guinea") {
			return true;
		}
	}
	if (c1=="Western Australia") {
		if (c2=="Indonesia" || c2=="New Guinea" || c2=="Eastern Australia") {
			return true;
		}
	}
	
	return false;
}

int game()
{
//	testRollingJudgement();

	startGame();

	while (!main_disp.is_closed()) {
		
		//place armies
		if (phase==1){
			placeArmies(turn);
		}
		
		main_disp.wait();
		if (main_disp.button() && main_disp.mouse_y()>=0) {
			const int x = main_disp.mouse_x();
			const int y = main_disp.mouse_y();
			id1 = getCountryId(x, y, 0);
			
			//continue button
			if (id1==100) {
				//advance to next phase/turn
				if (phase==4) {
					//this code is also in phase 4's section
					phase = 1;
					do {
						turn = turn + 1;
						if (turn==7) turn = 1;
					} while (!players[turn].isAlive());
					cout << " - Player " << turn << "'s turn - " << endl;
				}
				else phase = phase + 1;
				refreshMap();
				
//				phase=4;
//				refreshMap();
//				continue;
			}
			
			//attack phase
			//still needs non-max attack logic
			if (phase==3) {
				//not your country
				if (countries[id1].getOwner() != turn) {
					continue;
				}
				
				cout << countries[id1].getName() << " (" << countries[id1].getArmies()<< ") attacking: ";
				cout.flush();
				while (!main_disp.is_closed()) {
					main_disp.wait();
					if (main_disp.button() && main_disp.mouse_y()>=0) {
						const int x = main_disp.mouse_x();
						const int y = main_disp.mouse_y();
						int id2 = getCountryId(x, y, 0);
						
						//legal attack
						if (countries[id1].getOwner() != countries[id2].getOwner()
							&& countries[id1].getArmies() > 1
							&& bTouching(countries[id1].getName(), countries[id2].getName())) {
							
							cout << countries[id2].getName() << " (" << countries[id2].getArmies() << ")" << endl;
							attacking = countries[id1].getArmies()-1;
							defending = countries[id2].getArmies();
							
							//not-max attackers decision will go here
							
							tuple<char, int> result = decideRolls(attacking, defending);
							if (get<0>(result)=='X') {
								cout << "Both players lose 1 army" << endl << endl;
								if (countries[id2].getArmies()==1) {
									countries[id2].setOwner(countries[id1].getOwner());
									countries[id2].setArmies(attacking);
									countries[id1].setArmies(countries[id1].getArmies()-attacking);
								}
								else {
									countries[id1].subArmies(1);
									countries[id2].subArmies(1);
								}
							}
							else {
								int deaths = get<1>(result);
								cout << "Player " << get<0>(result) << " loses " << deaths << endl << endl;
								if (get<0>(result)=='D'){
									if (countries[id2].getArmies()==deaths) {
										countries[id2].setOwner(countries[id1].getOwner());
										countries[id2].setArmies(attacking);
										countries[id1].setArmies(countries[id1].getArmies()-attacking);
									}
									else countries[id2].subArmies(deaths);
								}
								else {
									countries[id1].subArmies(deaths);
								}
							}
							
							break;
						}
						
						cout << countries[id2].getName() << endl << " Can't attack";
						if (id1==id2)
							cout << " itself!" << endl;
						else if (countries[id1].getOwner() == countries[id2].getOwner())
							cout << " your own country!" << endl;
						else if (!bTouching(countries[id1].getName(), countries[id2].getName()))
							cout << " from that far away!" << endl;
						else if (countries[id1].getArmies() == 1)
							cout << ", not enough armies!" << endl;
						cout << endl;
						break;
						
					}
				}
				refreshMap();
			}
			
			
			//tactical
			//needs chain of borders
			if (phase==4) {
				if (countries[id1].getOwner() != turn) {
					continue;
				}
				
				cout << "Move armies from " << countries[id1].getName() << " to: " << endl;
				while (!main_disp.is_closed()) {
					main_disp.wait();
					if (main_disp.button() && main_disp.mouse_y()>=0) {
						const int x = main_disp.mouse_x();
						const int y = main_disp.mouse_y();
						int id2 = getCountryId(x, y, 0);
						
						if (id2==100) {
							break;
						}
						//same country
						if (id2 == id1){
							continue;
						}
						
						//legal move
						//still needs chain of borders
						else if (countries[id1].getOwner() == countries[id2].getOwner() && countries[id1].getArmies() > 1) {
							countries[id1].subArmies(1);
							countries[id2].addArmies(1);
							refreshMap();
						}
					}
				}
				phase = 1;
				do {
					turn = turn + 1;
					if (turn==7) turn = 1;
				} while (!players[turn].isAlive());
				cout << " - Player " << turn << "'s turn - " << endl;
				refreshMap();
			}

			
		}
	}

	
	cout << "--- out of game range ---" << endl;
	
	while (!main_disp.is_closed()) {
		main_disp.wait();
		if (main_disp.button() && main_disp.mouse_y()>=0) {
			const int x = main_disp.mouse_x();
			const int y = main_disp.mouse_y();
			int id = getCountryId(x, y, 0);
			
			cout << countries[id].getName() << endl;
			countries[id].addArmies(1);

			refreshMap();
		}
	}
	return 0;
}

int main(int argc, const char * argv[]) {
	game();
    return 0;
}