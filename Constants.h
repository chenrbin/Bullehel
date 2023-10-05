#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
namespace Constants {
	// Todo: convex shapes, arrowhead bullet, sprite layering, figure out variance constant, add new petals
	// more patterns, more bullets
	// mercury poison, resurrection butterfly, seamless ceiling
	// Original ideas: coding, chemistry, dna spirals
	// Size and dimensions
	const int WINDOWWIDTH = 1600, WINDOWHEIGHT = 900;
	const int SCREENWIDTH = 720, SCREENHEIGHT = 840, SCREENLEFT = 50, SCREENTOP = 40;
	const sf::FloatRect SCREENBOUNDS(SCREENLEFT, SCREENTOP, SCREENWIDTH, SCREENHEIGHT);
	const int BORDERMARGIN = WINDOWWIDTH; // The margin where game objects exist but are not seen. Past this is out of bounds
	
	// Sprite variables
	const float STANDARDCIRCLEOUTLINE = 2;
	const float SMALLBULLETOUTLINE = 0.5;
	const float PLAYERHITBOXRADIUS = 4;
	const float STANDARDCIRCLEBULLETRADIUS = 6;
	const float STANDARDRICEBULLETRADIUS = 4;
	const float STANDARDDOTBULLETRADIUS = 4;
	const float STANDARDTALISMANBULLETRADIUS = 4;
	const float STANDARDBUBBLEBULLETRADIUS = 15;
	const float STANDARDSPAWNERRADIUS = 4;
	const float STARTINGLASEROUTLINE = 1;
	const float FINALLASEROUTLINE = 2; // Not used currently
	// Object positions
	const sf::Vector2f SCREENPOS(SCREENLEFT, SCREENTOP);
	const sf::Vector2f FPSTEXTPOS(SCREENLEFT + SCREENWIDTH - 50, SCREENTOP + SCREENHEIGHT - 50 );

	// Mechanical variables
	const float PLAYERSTANDARDSPEED = 6, FOCUSSPEEDMODIFIER = 0.5f;
	const float FPS = 60;
	const float PI = 3.14159f;


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


	const sf::Color PLAYERHITBOXCOLOR = RED;

	// Pattern constants
	// BOWAP
	const float BOWAP_ANGLEOFFSET = -5, BOWAP_ANGLEVELOCITY = -150, BOWAP_ANGLEACCELERATION = 100;
	// QED 
	const int QED_VARIANCEX = 400, QED_VARIANCEY = 200; // Length range where bullets can spawn

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
	const vector<sf::Color> UFO_BULLETCOLORS = { BLUE, CYAN, MAGENTA, YELLOW, RED };

	// WindGod
	// Desired behavior
	const int MOF_PETALCOUNT = 5;
	const float MOF_INNERRADIUS = 80; // Radius of the inner petals
	const float MOF_CIRCLEPORTIONCUT = 0.15f; // Percentage of circle cut from spawning pattern
	const float MOF_ARCDRAWTIME = 0.8f; // Seconds to draw the cut circle
	// Tangential speed of spawner, units per frame. Speed = Arc length / Arc draw time (add density scalar later)
	const float MOF_SPAWNERMOVESPEED = 2 * PI * MOF_INNERRADIUS * (1 - MOF_CIRCLEPORTIONCUT) / (MOF_ARCDRAWTIME * FPS);
	// Number of frames to advance from centerPos to spawner starting position
	const float MOF_FRAMEOFFSET = MOF_ARCDRAWTIME * (1 / (1 - MOF_CIRCLEPORTIONCUT) - 1) / 2 * FPS;
	// Handles "frame skipping" to increase density
	// A cycle of DENOMINATOR frames, every cycle, each frame makes MINADVANCEMENTS advancements and REMAINDER frames advance one extra time
	// Scalar fraction should be in simplest form for smoothest transition. Common denominator is not checked.
	const int MOF_DENSITYSCALENUMERATOR = 5, MOF_DENSITYSCALEDENOMINATOR = 4;
	const int MOF_MINADVANCEMENTS = MOF_DENSITYSCALENUMERATOR / MOF_DENSITYSCALEDENOMINATOR;
	const int MOF_ADVANCEMENTREMAINDER = MOF_DENSITYSCALENUMERATOR % MOF_DENSITYSCALEDENOMINATOR;

	// Handles angle variance
	const float MOF_VARIANCE = -2.4 / MOF_ARCDRAWTIME; // 3.5 inversely relative to draw time was tested to be the desired spread. Slightly decrease for bigger spread.
	const vector<float> MOF_BULLETANGLEVARIANCE = { MOF_VARIANCE, 2 * MOF_VARIANCE, 3 * MOF_VARIANCE, 4 * MOF_VARIANCE };
	// Bullet flags
	const char NEUTRAL = 0, BOUNCED = 1, REVERSEROTATION = 1, ACTIVESPAWNERHITBOX = 1;

	// Print stuff for debug
	template <typename T>
	void print(T var) {
		cout << "Value is: " << var << endl;
	}
}