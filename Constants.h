#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
namespace Constants {
	// Todo: convex shapes, arrowhead bullet, mercury poison rotation, initial deceleration, delay between waves
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
	const sf::Color YELLOW(220, 220, 0);
	const sf::Color GREEN(0, 255, 0);
	const sf::Color BLUE(0, 0, 255);
	const sf::Color CYAN(0, 200, 200);
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
	const float BOWAP_ANGLEOFFSET = -5, BOWAP_ANGLEVELOCITY = -150, BOWAP_ANGLEACCELERATION = 100; // Time is in seconds
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
	const float UFO_PHASE2VELOCITY = 90, UFO_PHASE3VELOCITY = -20; 	// Velocity of radius expansion, units per second
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
	const float MOF_RADIUS1 = 90; // Radius of the inner petals
	// Percentage of circle cut in pattern
	const float MOF_LAYER1CUT = 0.15f, MOF_LAYER4CUT = 0.75f; // First layer in flower 1 and 2
	const float MOF_LAYER2CUT = 0.5f, MOF_LAYER3CUT = 0.5f; // Both flowers use the same cut for layers 2 and 3

	const int MOF_ARCDRAWTIME1 = 0.8 * FPS; // Frames to draw the first circle
	const int MOF_LAUNCHDELAY = 90; // Number of frames after start of layer drawing to launch bullets
	const float MOF_LAUNCHACCEL = 0.03; // Acceleration at launch
	const vector<sf::Color> MOF_BULLETCOLORS = { RED, VIOLET, BLUE, GREEN, GREEN, CYAN };
	const float MOF_FASTSPEEDMULTIPLIER = 1.6; // Used for layer 3
	const float MOF_SLOWSPEEDMULTIPLIER = 0.85; // Used for layer 1, flower 2

	// Calculations
	// Base speed of spawners
	const float MOF_SPAWNERMOVESPEED = 2 * PI * MOF_RADIUS1 * (1 - MOF_LAYER1CUT) / MOF_ARCDRAWTIME1; 
	// Number of frames to advance from centerPos to spawner starting position
	const float MOF_FRAMEOFFSET = MOF_ARCDRAWTIME1 * (1 / (1 - MOF_LAYER1CUT) - 1) / 2;
	
	const float MOF_RADIUS2 = 2 * sin(PI / MOF_PETALCOUNT) * MOF_RADIUS1;
	const float MOF_RADIUS3 = MOF_RADIUS2 * sqrt(2); 

	// Can add constants to the end of checkpoints to make sure wave bullets are divisible by 5 * 4
	const int MOF_LAYER1CHECKPOINT = MOF_ARCDRAWTIME1 + 1; // Time point where layer 1 is complete
	const int MOF_LAYER2CHECKPOINT = MOF_LAYER1CHECKPOINT + PI * MOF_RADIUS2 / MOF_SPAWNERMOVESPEED; 
	const int MOF_LAYER3CHECKPOINT = MOF_LAYER2CHECKPOINT + PI * MOF_RADIUS3 / (MOF_FASTSPEEDMULTIPLIER * MOF_SPAWNERMOVESPEED) + 2;
	// Frame checkpoints for second flower. Does not include refresh delay
	const int MOF_LAYER4CHECKPOINT = MOF_LAYER3CHECKPOINT + MOF_LAYER1CHECKPOINT;
	const int MOF_LAYER5CHECKPOINT = MOF_LAYER3CHECKPOINT + MOF_LAYER2CHECKPOINT;
	const int MOF_LAYER6CHECKPOINT = MOF_LAYER3CHECKPOINT * 2;


	// Handles "frame skipping" to increase density
	// A cycle of DENOMINATOR frames, every cycle, each frame makes MINADVANCEMENTS advancements and REMAINDER frames advance one extra time
	// Scalar fraction should be in simplest form for smoothest transition. Common denominator is not checked.
	const int MOF_DSCALENUMER = 5, MOF_DSCALEDENOM = 4, MOF_DSCALENUMER2 = 5, MOF_DSCALEDENOM2 = 6, MOF_DSCALENUMER3 = 4, MOF_DSCALEDENOM3 = 3;
	const int MOF_EXPECTEDBULLETS1 = 60, MOF_EXPECTEDBULLETS2 = 40, MOF_EXPECTEDBULLETS3 = 40;

	// Handles angle variance
	const float MOF_VARIANCECONSTANT = -187; // Constant that is manually tested. Decrease for bigger spread.
	const vector<float> MOF_BULLETANGLEVARIANCE = { MOF_VARIANCECONSTANT, 2 * MOF_VARIANCECONSTANT, 3 * MOF_VARIANCECONSTANT, 4 * MOF_VARIANCECONSTANT };
	
	// Delay between waves, in frames
	const int HGP_WAVEDELAY = 120;
	// Number of rings in each wave
	const int HGP_WAVECOUNT = 16;

	// Bullet flags
	const char NEUTRAL = 0, BOUNCED = 1, REVERSEROTATION = 1, ACTIVESPAWNERHITBOX = 1;

	// Print stuff for debug
	template <typename T>
	void print(T var) {
		cout << "Value is: " << var << endl;
	}
}