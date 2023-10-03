#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
namespace Constants {
	// Todo: convex shapes, laser expiration, review comments, MoF customizable variables
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
	const int STANDARDSPAWNERRADIUS = 4;
	const int STARTINGLASEROUTLINE = 1;
	const int FINALLASEROUTLINE = 2; // Not used currently
	
	// Object positions
	const sf::Vector2f SCREENPOS(SCREENLEFT, SCREENTOP);
	const sf::Vector2f FPSTEXTPOS(SCREENLEFT + SCREENWIDTH - 50, SCREENTOP + SCREENHEIGHT - 50 );

	// Mechanical variables
	const float PLAYERSTANDARDSPEED = 6, FOCUSSPEEDMODIFIER = 0.5f;
	const float FPS = 60;

	// Pattern constants
	
	// Flying Saucer
	// A bunch of variables that go into calculating rate of change in target radius. 
	// Calculated by using calculus on a quadratic equation, s(x) = Ax^2 + Bx
	// x = frameCounter (time), A = acceleration, B = velocity. Intercept is zero since circle grows from radius 0
	// v(x) = 2Ax + B; Radius growth velocity
	// a(x) = 2A; Radius growth acceleration
	// At x = t = TARGETTIME, s(t) = TARGETPOS = At^2 + Bt, v(s) = TARGETVEL = 2At + B
	// B = TARGETVEL - 2At, TARGETPOS = At^2 + (TARGETVEL - 2At) * t = At^2 + TARGETVEL * t - 2At^2 = TARGETVEL * t - At^2 = TARGETPOS 
	// => A = TARGETVEL * t - TARGETPOS / t^2 = PHASE1ACCEL = (Actual acceleration) / 2
	// TARGETVEL = 2 * PHASE1ACCEL * t + B => B = TARGETVEL - 2 * PHASE1ACCEL * t
	// Phase 1: Accelerating growth. Phase 2: Constant growth. Phase 3: Constant shrink 
	const float UFO_PHASE1CHECKPOINT = 60, UFO_PHASE2CHECKPOINT = 360; // Time point where phase ends (not duration. Phase 2 duration = checkpoint 2 - checkpoint 1)
	const float UFO_PHASE2VELOCITY = 90, UFO_PHASE3VELOCITY = -20; 	// Velocity of radius expansion
	const float UFO_TARGETPOS = 180, UFO_TARGETVEL = 45, UFO_TARGETTIME = UFO_PHASE1CHECKPOINT / FPS; // Target position, velocity at end of phase 1
	// PHASE1ACCEL is half the actual acceleration, 2A. Not an accurate name but best describes its purpose.
	const float UFO_PHASE1ACCEL = (UFO_TARGETTIME * UFO_TARGETVEL - UFO_TARGETPOS) / UFO_TARGETTIME / UFO_TARGETTIME;
	const float UFO_STARTVEL = UFO_TARGETVEL - 2 * UFO_PHASE1ACCEL * UFO_TARGETTIME;
	// Radius added by each phase, summed up for easier reading and calculation
	const float UFO_PHASE1ADDEDRADIUS = UFO_PHASE1CHECKPOINT / FPS * UFO_STARTVEL + pow(UFO_PHASE1CHECKPOINT / FPS, 2) * UFO_PHASE1ACCEL;
	const float UFO_PHASE2ADDEDRADIUS = (UFO_PHASE2CHECKPOINT - UFO_PHASE1CHECKPOINT) / FPS * UFO_PHASE2VELOCITY;


	// Bullet flags
	const char NEUTRAL = 0, BOUNCED = 1, REVERSEROTATION = 1, ACTIVESPAWNERHITBOX = 1;

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
	const sf::Color DEFAULTSPAWNERCOLOR = SEETHROUGH;


	const float PI = 3.14159;
	const sf::Color PLAYERHITBOXCOLOR = RED;
	// Print stuff for debug
	template <typename T>
	void print(T var) {
		cout << "Value is: " << var << endl;
	}
}