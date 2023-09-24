#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
namespace Constants {
	// Todo: convex shapes, laser expiration, review comments
	// more patterns, more bullets
	// mercury poison, resurrection butterfly, virtue of wind god
	// Size and dimensions
	const int WINDOWWIDTH = 1600, WINDOWHEIGHT = 900;
	const int SCREENWIDTH = 720, SCREENHEIGHT = 840, SCREENLEFT = 50, SCREENTOP = 40;
	const sf::FloatRect SCREENBOUNDS(SCREENLEFT, SCREENTOP, SCREENWIDTH, SCREENHEIGHT);
	const int BORDERMARGIN = WINDOWWIDTH; // The margin where game objects exist but are not seen. Past this is out of bounds
	
	// Sprite variables
	const int STANDARDCIRCLEOUTLINE = 2;
	const float SMALLBULLETOUTLINE = 0.5;
	const int PLAYERHITBOXRADIUS = 4;
	const int STANDARDCIRCLEBULLETRADIUS = 6;
	const int STANDARDRICEBULLETRADIUS = 4;
	const int STANDARDDOTBULLETRADIUS = 4;
	const int STANDARDTALISMANBULLETRADIUS = 4;
	const int STANDARDBUBBLEBULLETRADIUS = 15;
	const int STARTINGLASEROUTLINE = 1;
	const int FINALLASEROUTLINE = 2;
	
	// Object positions
	const sf::Vector2f SCREENPOS(SCREENLEFT, SCREENTOP);
	const sf::Vector2f FPSTEXTPOS(SCREENLEFT + SCREENWIDTH - 50, SCREENTOP + SCREENHEIGHT - 50 );

	// Mechanical variables
	const float PLAYERSTANDARDSPEED = 6, FOCUSSPEEDMODIFIER = 0.5f;
	const float FPS = 60;

	// Bullet flags
	const char NEUTRAL = 0, BOUNCED = 1, REVERSEROTATION = 1;


	// Set color constants for easy use and passing to functions
	const sf::Color WHITE(255, 255, 255);
	const sf::Color TRANSPARENTWHITE(255, 255, 255, 200);
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
	const sf::Color SEETHROUGH(255, 255, 255, 100);
	const sf::Color MAGENTA(255, 0, 255);
	// Default colors
	const sf::Color DEFAULTCIRCLEBULLETCOLOR = RED;
	const sf::Color DEFAULTRICEBULLETCOLOR = MAGENTA;
	const sf::Color DEFAULTDOTBULLETCOLOR = SEETHROUGH;
	const sf::Color DEFAULTTALISMANBULLETCOLOR = MAGENTA;
	const sf::Color DEFAULTBUBBLEBULLETCOLOR = RED;
	const sf::Color DEFAULTLASERCOLOR = BLUE;


	const float PI = 3.14159;
	const sf::Color PLAYERHITBOXCOLOR = RED;
	// Print stuff for debug
	template <typename T>
	void print(T var) {
		cout << "Value is: " << var << endl;
	}
}