#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
namespace Constants {
	// Todo: make game board
	

	// Size and dimensions
	const int WINDOWWIDTH = 1600, WINDOWHEIGHT = 900;
	const int SCREENWIDTH = 720, SCREENHEIGHT = 840, SCREENLEFT = 50, SCREENTOP = 40;


	// Mechanical variables
	const float PLAYERSTANDARDSPEED = 12, FOCUSSPEEDMODIFIER = 0.5f;
	const float PLAYERHITBOXRADIUS = 5;


	// Set color constants for easy use and passing to functions
	const sf::Color WHITE(255, 255, 255);
	const sf::Color BLACK(0, 0, 0);
	const sf::Color GRAY(128, 128, 128);
	const sf::Color RED(255, 0, 0);
	const sf::Color ORANGE(255, 165, 0);
	const sf::Color YELLOW(255, 255, 0);
	const sf::Color GREEN(0, 255, 0);
	const sf::Color BLUE(0, 0, 255);
	const sf::Color CYAN(0, 255, 255);
	const sf::Color VIOLET(148, 0, 211);
	const sf::Color TRANSPARENT(0, 0, 0, 0);
	const sf::Color PLAYERHITBOXCOLOR = RED;

	// Print stuff for debug
	template <typename T>
	void print(T var) {
		cout << "Value is: " << var << endl;
	}
}