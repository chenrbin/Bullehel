#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;
namespace Constants {
	// Todo: convex shapes, arrowhead bullet
	// resurrection butterfly, seamless ceiling
	// Original ideas: coding, chemistry, dna spirals

	// File paths
	const string PLAYERTEXTUREFILEPATH = "assets/reimoo.png";
	const string FONTFILEPATH = "assets/font.ttf";

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
	const float STANDARDARROWHEADBULLETRADIUS = 3;
	const float STANDARDSPAWNERRADIUS = 4;
	const float STARTINGLASEROUTLINE = 1;
	const float FINALLASEROUTLINE = 2; // Not used currently
	const float MINHITBOXSIZE = 3;

	// Object positions
	const sf::Vector2f SCREENPOS(SCREENLEFT, SCREENTOP);
	const sf::Vector2f FPSTEXTPOS(SCREENLEFT + SCREENWIDTH - 50, SCREENTOP + SCREENHEIGHT - 50);

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
	const sf::Color DEFAULTARROWHEADBULLETCOLOR = RED;

	const sf::Color DEFAULTSPAWNERCOLOR = SEETHROUGH;

	const sf::Color PLAYERHITBOXCOLOR = RED;

	// Simple pattern constants
	// BOWAP
	const float BOWAP_ANGLEOFFSET = -5, BOWAP_ANGLEVELOCITY = -150, BOWAP_ANGLEACCELERATION = 100; // Time is in seconds
	// QED 
	const int QED_VARIANCEX = 400, QED_VARIANCEY = 200; // Length range where bullets can spawn

	// Bullet flags
	const char NEUTRAL = 0, BOUNCED = 1, REVERSEROTATION = 1, ACTIVESPAWNERHITBOX = 1;

	// Print stuff for debug
	template <typename T>
	void print(T var) {
		cout << "Value is: " << var << endl;
	}
}
// Constant values for specific patterns with a lot of constands
// Flying Saucer
namespace UFO {
	using namespace Constants;
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
	const float PHASE1CHECKPOINT = 60, PHASE2CHECKPOINT = 360; // Time point where phase ends (not duration. Phase 2 duration = checkpoint 2 - checkpoint 1)
	const float PHASE2VELOCITY = 90, PHASE3VELOCITY = -20; 	// Velocity of radius expansion, units per second
	const float TARGETPOS = 180, TARGETVEL = 45, TARGETTIME = PHASE1CHECKPOINT / FPS; // Target position, velocity at end of phase 1
	// PHASE1ACCEL is half the actual acceleration, 2A. Not an accurate name but best describes its purpose.
	const float PHASE1ACCEL = (TARGETTIME * TARGETVEL - TARGETPOS) / TARGETTIME / TARGETTIME;
	const float STARTVEL = TARGETVEL - 2 * PHASE1ACCEL * TARGETTIME;
	// Radius added by each phase, summed up for easier reading and calculation
	const float PHASE1ADDEDRADIUS = PHASE1CHECKPOINT / FPS * STARTVEL + pow(PHASE1CHECKPOINT / FPS, 2) * PHASE1ACCEL;
	const float PHASE2ADDEDRADIUS = (PHASE2CHECKPOINT - PHASE1CHECKPOINT) / FPS * PHASE2VELOCITY;
	const vector<sf::Color> BULLETCOLORS = { BLUE, CYAN, MAGENTA, YELLOW, RED };
}

// Wind God 
namespace MOF {
	using namespace Constants;
	// WindGod
	// Desired behavior
	const int PETALCOUNT = 5;
	const float RADIUS1 = 90; // Radius of the inner petals
	// Percentage of circle cut in pattern
	const float LAYER1CUT = 0.15f, LAYER4CUT = 0.75f; // First layer in flower 1 and 2
	const float LAYER2CUT = 0.5f, LAYER3CUT = 0.5f; // Both flowers use the same cut for layers 2 and 3

	const int ARCDRAWTIME1 = 0.8 * FPS; // Frames to draw the first circle
	const int LAUNCHDELAY = 90; // Number of frames after start of layer drawing to launch bullets
	const float LAUNCHACCEL = 0.03; // Acceleration at launch
	const vector<sf::Color> BULLETCOLORS = { RED, VIOLET, BLUE, GREEN, GREEN, CYAN };
	const float FASTSPEEDMULTIPLIER = 1.6; // Used for layer 3
	const float SLOWSPEEDMULTIPLIER = 0.85; // Used for layer 1, flower 2

	// Calculations
	// Base speed of spawners
	const float SPAWNERMOVESPEED = 2 * PI * RADIUS1 * (1 - LAYER1CUT) / ARCDRAWTIME1;
	// Number of frames to advance from centerPos to spawner starting position
	const float FRAMEOFFSET = ARCDRAWTIME1 * (1 / (1 - LAYER1CUT) - 1) / 2;

	const float RADIUS2 = 2 * sin(PI / PETALCOUNT) * RADIUS1;
	const float RADIUS3 = RADIUS2 * sqrt(2);

	// Can add constants to the end of checkpoints to make sure wave bullets are divisible by 5 * 4
	const int LAYER1CHECKPOINT = ARCDRAWTIME1 + 1; // Time point where layer 1 is complete
	const int LAYER2CHECKPOINT = LAYER1CHECKPOINT + PI * RADIUS2 / SPAWNERMOVESPEED;
	const int LAYER3CHECKPOINT = LAYER2CHECKPOINT + PI * RADIUS3 / (FASTSPEEDMULTIPLIER * SPAWNERMOVESPEED) + 2;
	// Frame checkpoints for second flower. Does not include refresh delay
	const int LAYER4CHECKPOINT = LAYER3CHECKPOINT + LAYER1CHECKPOINT;
	const int LAYER5CHECKPOINT = LAYER3CHECKPOINT + LAYER2CHECKPOINT;
	const int LAYER6CHECKPOINT = LAYER3CHECKPOINT * 2;


	// Handles "frame skipping" to increase density
	// A cycle of DENOMINATOR frames, every cycle, each frame makes MINADVANCEMENTS advancements and REMAINDER frames advance one extra time
	// Scalar fraction should be in simplest form for smoothest transition. Common denominator is not checked.
	const int DSCALENUMER = 5, DSCALEDENOM = 4, DSCALENUMER2 = 5, DSCALEDENOM2 = 6, DSCALENUMER3 = 4, DSCALEDENOM3 = 3;
	const int EXPECTEDBULLETS1 = 60, EXPECTEDBULLETS2 = 40, EXPECTEDBULLETS3 = 40;

	// Handles angle variance
	const float VARIANCECONSTANT = -187; // Constant that is manually tested. Decrease for bigger spread.
	const vector<float> BULLETANGLEVARIANCE = { VARIANCECONSTANT, 2 * VARIANCECONSTANT, 3 * VARIANCECONSTANT, 4 * VARIANCECONSTANT };
}

// Mercury Poison
namespace HGP {
	const int BULLETSIZE = 8;
	// Delay between waves, in frames
	const int WAVEDELAY = 120;
	// Number of rings in each wave
	const int WAVECOUNT = 16;
	const float SPEEDMULTIPLIER = 1.17;

	// Frame point for rotating bullets
	const int ROTATIONSTART = 45, ROTATIONEND = 180;
	const float ROTATIONANGLE = 0.375f;
}

// Seamless Ceiling
namespace SCOKJ {
	const int CEILINGCOUNT = 20, CEILINGDROPDELAY = 60;
	const float CEILINGBULLETINITIALSPEED = 3, CEILINGBULLETSPACING = 0.35;
	const int CEILINGVARIANCEX = 80, CEILINGVARIANCEY = 100;
	const char ISCEILING = 1;
	const int STREAMVARIANCEXY = 50;
	const int PHASE1END = 900, PHASE2END = 1800, PHASE3END = 2700; 
}