#pragma once
#include "Constants.h"
// Class to store bullet pattern templates. The base class is for random bullets and children have specific patterns.
class Pattern : public sf::Drawable {
protected:
	sf::FloatRect screenBounds; // Determines the bounds where the bullets can exist
	vector<Bullet*> bullets;
	// Timing
	int frameCounter; // Used as a timer and determines where to spawn bullets and when to move them
	bool active;

	// Bullet properties
	int streamCount;
	float baseSpeed;
	sf::Vector2f sourcePos;
	// Amount of shots per second, up to the framerate. FPS / shotFrequency = frames per shot
	// Shoots only once if set to zero. Max is 60 (shoots every frame)
	float shotFrequency;
	bool shootOnlyOnce;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		if (active)
			for (Bullet* bullet : bullets)
				target.draw(*bullet, states);
	}
public:
	Pattern(int streamCount = 0, float shotFrequency = 0, float baseSpeed = 0, sf::Vector2f sourcePos = SCREENPOS) {
		frameCounter = 0;
		active = true;
		shootOnlyOnce = false;
		screenBounds = SCREENBOUNDS;
		expandBounds(0.1);
		this->streamCount = streamCount;
		this->shotFrequency = shotFrequency;
		this->baseSpeed = baseSpeed;
		this->sourcePos = sourcePos;
		if (shotFrequency > FPS)
			shotFrequency = FPS;
	}
	~Pattern() {
		for (Bullet* bullet : bullets)
			delete bullet;
	}
	virtual void processMovement() {
		if (!active)
			return;
		for (Bullet* bullet : bullets)
			bullet->processMovement();
	}
	// Increment frame counter
	void incrementFrame() {
		if (active)
			frameCounter++;
	}
	// Base class does nothing
	virtual void spawnBullets() {
		return;
	}
	// Delete all bullets. Typically paired with resetPattern, but not always
	virtual void deleteAllBullets() {
		for (Bullet* bullet : bullets)
			delete bullet;
		bullets.clear();
	}
	// Delete out of bound bullets. Some patterns will need to have larger bounds
	virtual void deleteOutOfBoundsBullets() {
		for (int i = 0; i < bullets.size(); i++)
			if (!screenBounds.contains(bullets[i]->getPosition())) {
				Bullet* temp = bullets[i];
				bullets.erase(bullets.begin() + i);
				delete temp;
				i--; // Reiterate current index since vector has shrunk;
			}
	}

	// Reset frame counter
	virtual void resetPattern() {
		frameCounter = 0;
		for (Bullet* bullet : bullets)
			bullet->resetBullet();
	}
	vector<Bullet*>& getBullets() {
		return bullets;
	}
	bool getActive() {
		return active;
	}
	void setActive(bool val) {
		active = val;
	}
	void setScreenBounds(sf::FloatRect& screenBounds) {
		this->screenBounds = screenBounds;
	}
	// Adds width/height * increaseFactor to each side. Entering 1 will triple width and height
	void expandBounds(float increaseFactor) {
		screenBounds.left -= SCREENWIDTH * increaseFactor;
		screenBounds.top -= SCREENHEIGHT * increaseFactor;
		screenBounds.width += SCREENWIDTH * 2 * increaseFactor;
		screenBounds.height += SCREENHEIGHT * 2 * increaseFactor;
	}
	// Compares shotFrequency with frameCounter and is used by spawnBullets to determine when to shoot.
	virtual bool isGoodToshoot() {
		int framesPerShot;
		if (shotFrequency == 0) {
			if (!shootOnlyOnce) {
				shootOnlyOnce = true;
				return true;
			}
			return false;
		}
		else
			return frameCounter % int(FPS / shotFrequency) == 0;
	}
	// All addBullet functions use a source position and polar coordinates
	void addCircleBullet(sf::Vector2f position, float speed = 0, float angleDegrees = 0, sf::Color color = DEFAULTCIRCLEBULLETCOLOR, int radius = STANDARDCIRCLEBULLETRADIUS) {
		bullets.push_back(new CircleBullet(position, speed, angleDegrees, color, radius));
	}
	void addRiceBullet(sf::Vector2f position, float speed = 0, float angleDegrees = 0, sf::Color color = DEFAULTRICEBULLETCOLOR, int radius = STANDARDRICEBULLETRADIUS) {
		bullets.push_back(new RiceBullet(position, speed, angleDegrees, color, radius));
	}
	void addDotBullet(sf::Vector2f position, float speed = 0, float angleDegrees = 0, sf::Color color = DEFAULTDOTBULLETCOLOR, int radius = STANDARDDOTBULLETRADIUS) {
		bullets.push_back(new DotBullet(position, speed, angleDegrees, color, radius));
	}
	void addTalismanBullet(sf::Vector2f position, float speed = 0, float angleDegrees = 0, sf::Color color = DEFAULTTALISMANBULLETCOLOR, int radius = STANDARDTALISMANBULLETRADIUS) {
		bullets.push_back(new TalismanBullet(position, speed, angleDegrees, color, radius));
	}
	void addBubbleBullet(sf::Vector2f position, float speed = 0, float angleDegrees = 0, sf::Color color = DEFAULTBUBBLEBULLETCOLOR, int radius = STANDARDBUBBLEBULLETRADIUS) {
		bullets.push_back(new BubbleBullet(position, speed, angleDegrees, color, radius));
	}
	void addLaser(sf::Vector2f position, float angleDegrees = 0, float maxWidth = 0, float growthSpeed = 1, float activationDelay = 0, float activeDuration = 0, sf::Color color = DEFAULTLASERCOLOR) {
		bullets.push_back(new Laser(position, angleDegrees, maxWidth, growthSpeed, activationDelay, activeDuration, color));
	}
	// Spawners are always inserted at the beginning of the array
	void addSpawner(sf::Vector2f position, float speed = 0, float angleDegrees = 0, bool visible = false, sf::Color color = DEFAULTSPAWNERCOLOR, int radius = STANDARDSPAWNERRADIUS) {
		bullets.insert(bullets.begin(), new Spawner(position, speed, angleDegrees, visible, color, radius));
	}
};

// Class for patterns that groups bullets into "waves" to manipulate
class WavePattern : public Pattern{
protected:
	// Wave counters organize the bullets and allow manipulation of individual waves
	vector<int> waveBulletCount; // Stores the number of remaining bullets per wave
	vector<int> waveFrameCount; // Stores the time each wave has been active
	int currentBulletCount; // Keeps track of the bullet count in each layer for the vectors. Used for patterns with no clearly defined wave sizes
public:
	WavePattern(float streamCount, float shotFrequency, float baseSpeed, sf::Vector2f sourcePos) : Pattern(streamCount, shotFrequency, baseSpeed, sourcePos) {
		currentBulletCount = 0;
	}
	// Add wave based on current bullets. Optionally use an argument instead of currentBulletCount
	void addWave(int bulletCount = 0) {
		if (bulletCount == 0) {
			waveBulletCount.push_back(currentBulletCount);
			currentBulletCount = 0;
		}
		else {
			waveBulletCount.push_back(bulletCount);
		}
		waveFrameCount.push_back(0);
		checkValidWaves();
	}
	// Check if wave vectors are consistent with actual bullet vectors 
	void checkValidWaves() {
		int counter = 0;
		for (int& count : waveBulletCount)
			counter += count;
		if (counter != bullets.size()) {
			print("Wave mismatch");
			print(bullets.size());
			print(counter);
		}
	}
	// Returns the start indices of the bullet vector for a specific wave 
	int getStartIndex(int waveindex) {
		int num = 0; // Number of bullets before current wave
		for (int i = 0; i < waveindex; i++)
			num += waveBulletCount[i];
		return num;
	}
	int getEndIndex(int waveindex) { 
		return getStartIndex(waveindex) + waveBulletCount[waveindex] - 1;
	}
	// Increment wave frame timers. Needed for time tracking
	void incrementWaveFrames() {
		for (int& counter : waveFrameCount)
			counter++;
	}
	void incrementCurrentBulletCount() {
		currentBulletCount++;
	}
	// Assuming all bullets are counted in the wave vectors, updates vectors along with OOB checks
	virtual void deleteOutOfBoundsBullets() {
		// Delete out of bound bullets while keeping sync with wave counters
		for (int i = 0; i < bullets.size(); i++)
			if (!screenBounds.contains(bullets[i]->getPosition())) {
				Bullet* temp = bullets[i];
				bullets.erase(bullets.begin() + i);
				delete temp;

				// Decrement waveBulletCount at the correct index
				int cumulativeCount = 0;
				for (int j = 0; j < waveBulletCount.size(); j++)
					if (i < waveBulletCount[j] + cumulativeCount) {
						waveBulletCount[j]--;
						if (waveBulletCount[j] <= 0) { // Erase empty waves
							waveBulletCount.erase(waveBulletCount.begin() + j);
							waveFrameCount.erase(waveFrameCount.begin() + j);
						}
						break;
					}
					else
						cumulativeCount += waveBulletCount[j];
				i--; // Reiterate current index since vector has shrunk;
			}

	}
	// Delete all bullets and clear vectors
	virtual void deleteAllBullets() {
		Pattern::deleteAllBullets();
		waveBulletCount.clear();
		waveFrameCount.clear();
		currentBulletCount = 0;
	}
};

// Direct stream with accelerating angle velocity
class Bowap : public Pattern {
public:
	Bowap(float BOWAP_ANGLEOFFSET, float BOWAP_ANGLEVELOCITY, float BOWAP_ANGLEACCELERATION, int streamCount, sf::Vector2f sourcePos, float shotFrequency, float baseSpeed)
		: Pattern(streamCount, shotFrequency, baseSpeed, sourcePos) {}
	void spawnBullets() {
		if (!active)
			return;
		if (isGoodToshoot()) {
			// Rotation is constant. Bullet count, frequency, and speed can be set.
			int num = BOWAP_ANGLEOFFSET + BOWAP_ANGLEVELOCITY * frameCounter / FPS + BOWAP_ANGLEACCELERATION * pow(frameCounter / FPS, 2);
			for (int i = 0; i < streamCount; i++)
				addRiceBullet(sourcePos, baseSpeed, num + i * 360 / streamCount);
		}
	}
};

// Ring of bullets, bounces off top left right walls once.
class QedRipples : public Pattern { // Todo: speed up phases
	sf::FloatRect bounceBounds;
public:
	QedRipples(int streamCount, sf::Vector2f sourcePos, float shotFrequency, float baseSpeed, sf::FloatRect bounceBounds = SCREENBOUNDS)
		:Pattern(streamCount, shotFrequency, baseSpeed, sourcePos) {
		this->bounceBounds = bounceBounds;
	}
	void processMovement() {
		for (Bullet* bullet : bullets) {
			bullet->processMovement();
			// Check for bounces
			sf::Vector2f pos = bullet->getPosition();
			if (!bounceBounds.contains(pos)) {
				if (pos.y > bounceBounds.top + bounceBounds.height)
					continue;
				if (bullet->getFlag() != BOUNCED && (pos.x < bounceBounds.left || pos.x > bounceBounds.left + bounceBounds.width)) 
					bullet->flipX();
				else if (pos.y < bounceBounds.top) 
					bullet->flipY();
				bullet->setFlag(BOUNCED);
			}
		}
			}
	void spawnBullets() {
		if (!active)
			return;
		if (isGoodToshoot()) {
			// Random angle and position
			int shotAngle = rand() % 360;
			sf::Vector2f shotSource;
			if (frameCounter != 0)
				shotSource = { sourcePos.x + rand() % QED_VARIANCEX - QED_VARIANCEX / 2, sourcePos.y + rand() % QED_VARIANCEY - QED_VARIANCEY / 2 };
			else // Fixed position for first shot
				shotSource = sourcePos;
			for (int i = 0; i < streamCount; i++)
				addRiceBullet(shotSource, baseSpeed, shotAngle + i * 360 / streamCount, BLUE);
		}
	}
};
// Ring of bullets that along an imaginary circle that moves down and expands
class FlyingSaucer : public WavePattern {
	bool alternate; // Alternate rotation
	vector<sf::Vector2f> shotSources;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (Bullet* bullet : bullets)
			target.draw(*bullet, states);
	}
public:
	FlyingSaucer(int streamCount, sf::Vector2f sourcePos, float shotFrequency, float baseSpeed)
		:WavePattern(streamCount, shotFrequency, baseSpeed, sourcePos) {
		alternate = false;
		shotSources.push_back({ sourcePos.x - 180, sourcePos.y + 50 });
		shotSources.push_back({ sourcePos.x + 180, sourcePos.y + 50 });
		shotSources.push_back({ sourcePos.x + 100, sourcePos.y - 150 });
		shotSources.push_back({ sourcePos.x - 100, sourcePos.y - 150 });
		expandBounds(1);
	}
	void processMovement() {
		using namespace UFO;
		if (!active)
			return;
		incrementWaveFrames();
		// Process movement and ring expansion through rotation speed
		for (int wave = 0; wave < waveBulletCount.size(); wave++) {
			// Calculate circle radius based on desire behavior. See pattern constants in Constants.h
			float targetRadius = 0;
			int frameCount = waveFrameCount[wave];
			// Determine speed of ring expansion
			if (frameCount < PHASE1CHECKPOINT) {
				targetRadius = frameCount / FPS * STARTVEL + pow(frameCount / FPS, 2) * PHASE1ACCEL;
			}
			else if (frameCount < PHASE2CHECKPOINT) {
				targetRadius = (frameCount - PHASE1CHECKPOINT) / FPS * PHASE2VELOCITY + PHASE1ADDEDRADIUS;
			}
			else {
				targetRadius = (frameCount - PHASE2CHECKPOINT) / FPS * PHASE3VELOCITY + PHASE1ADDEDRADIUS + PHASE2ADDEDRADIUS;
			}
			// Rotate each wave
			for (int j = getStartIndex(wave); j <= getEndIndex(wave); j++) {
				Bullet* bullet = bullets[j];
				bullet->processMovement();
				if (bullet->getFlag() == NEUTRAL)
					bullet->rotateArc(targetRadius, baseSpeed);
				else
					bullet->rotateArc(targetRadius, -baseSpeed);
				// Move bullets down
				if (frameCount < PHASE2CHECKPOINT)
					bullet->adjustPosition(0, 1);
				else // Speed up descent after phase 2
					bullet->adjustPosition(0, 1.1);
			}
		}

	}
	void spawnBullets() {
		using namespace UFO;
		if (!active)
			return;
		if (isGoodToshoot()) {
			int shotAngle = rand() % 360;
			int randomColor = rand() % BULLETCOLORS.size();
			int sourceCount = 0;
			for (sf::Vector2f pos : shotSources) {
				int index = bullets.size(); // Save index to set flags
				for (int i = 0; i < streamCount; i++) {
					addTalismanBullet(pos, baseSpeed, shotAngle + i * 360 / streamCount, BULLETCOLORS[randomColor]);
				}
				bool alternateCondition = pos.x > sourcePos.x ^ pos.y > sourcePos.y;
				if (alternate) // Alternate decides a specific shotSource's rotation
					alternateCondition = !alternateCondition; // alternateCondition reverses half of the sources' rotation
				if (alternateCondition) // Index goes from size before adding batch to after adding. Effectively accesses the new batch
					for (; index < bullets.size(); index++) 
						bullets[index]->setFlag(REVERSEROTATION);
				sourceCount++;
				if (sourceCount == shotSources.size() / 2) // Reroll rng
					shotAngle = rand() % 360;
			}
			// Add to the wave counters
			addWave(shotSources.size() * streamCount);
			alternate = !alternate;
		}
	}
};

// Simple but fast bullet rings 
class GengetsuTime : public Pattern {
public:
	GengetsuTime(int streamCount, sf::Vector2f sourcePos, float shotFrequency, float baseSpeed)
		:Pattern(streamCount, shotFrequency, baseSpeed, sourcePos) {
	}
	void spawnBullets() {
		if (!active)
			return;
		if (isGoodToshoot()) {
			// Random angle and position
			int shotAngle = rand() % 360;
			bool useDotBullets = rand() % 2;
			sf::Vector2f shotSource = { sourcePos.x + rand() % 200 - 100, sourcePos.y + rand() % 100 - 50 };
			for (int i = 0; i < streamCount; i++) {
				if (useDotBullets)
					addDotBullet(shotSource, baseSpeed, shotAngle + i * 360 / streamCount);
				else
					addCircleBullet(shotSource, baseSpeed, shotAngle + i * 360 / streamCount, BLUE);
			}
		}
	}
};

// 3-layer floral pattern spawns
class WindGod : public WavePattern{
	int varianceCounter; // Groups bullets into groups of 4. Used with angle variance vector
	int spawnPoint; // Frame point where inner spawners became active
	int cycleCounter;
	int shotAngle; // Refreshes for every new "flower"
	float adjustedSpawnerSpeed; // Spawner speed that accounts for skipped frames
	float currentCircleRadius; // Spawner pathing.
	int scaleNumer, scaleDenom; // Adjusts bullet density
	float bulletDensity; // Adjustment to ensure proper bullet spread
	int currentColorIndex; // Index 
	int refreshFrames;
	int phase; // Current stage of the 8-phase cycle

public:
	WindGod(sf::Vector2f sourcePos, float refreshDelay, float baseSpeed, float duration = 0)
		:WavePattern(0, 1 / (MOF::LAYER3CHECKPOINT / FPS + refreshDelay) / 2, baseSpeed, sourcePos) { 
		// StreamCount is unused because petal count is constant. 
		// Shot frequency determines frequency of spawner refresh. 
		spawnPoint = 0, varianceCounter = 0, cycleCounter = 0, shotAngle = 0, currentBulletCount = 0, phase = 0;
		currentCircleRadius = 0; bulletDensity = 0, scaleDenom = 0, scaleNumer = 0, adjustedSpawnerSpeed = 0;
		expandBounds(0.1); // Spawner may slightly clip the top, so expand bounds
		currentColorIndex = 0;
		refreshFrames = refreshDelay * FPS;
	}
	void spawnBullets() {
		using namespace MOF;
		if (!active)
			return;
		// There will be five spawners persistent as the first five items in the bullet vector
		if (isGoodToshoot()) { // Reset to layer 1
			spawnPoint = frameCounter;
			currentColorIndex = 0;
			calculatePhase();
			adjustSpawners();

			// Erase existing spawners and set shot angle
			if (frameCounter != 0) { // Delete existing spawners
				for (int i = 0; i < PETALCOUNT; i++)
					bullets.erase(bullets.begin());
				shotAngle = rand() % 360;
			}
			else {
				shotAngle = 0; // Preset angle for first shot
			}
			
			// Create new spawners
			for (int i = 0; i < PETALCOUNT; i++)
				addSpawner(sourcePos, SPAWNERMOVESPEED, 360 / PETALCOUNT * i + 180 + shotAngle);
			if (frameCounter == 0) // Accounting for spawners in the waves. They won't be deleted by OOB checks but will instantly be replaced, so there is no need to update these.
				addWave(5); // Spawners will always be wave 0;

			// Skip frames to set up starting position for spawners
			for (int i = 0; i < PETALCOUNT; i++) {
				for (int j = 0; j < FRAMEOFFSET; j++)
				{
					bullets[i]->rotateArc(currentCircleRadius, SPAWNERMOVESPEED);
					bullets[i]->processMovement();
				}
				// Once spawners are in position, adjust spawner speed
				bullets[i]->setSpeed(adjustedSpawnerSpeed);
			}
		}
	}
	void processMovement() {
		using namespace MOF;
		if (!active)
			return;
		incrementWaveFrames();
		calculatePhase();
		// Launch the talisman bullets if they're ready
		for (int i = 1; i < waveBulletCount.size(); i++)
		{
			if (waveFrameCount[i] >= LAUNCHDELAY && waveFrameCount[i] <= LAUNCHDELAY + baseSpeed / LAUNCHACCEL)
			{
				for (int j = getStartIndex(i); j <= getEndIndex(i); j++)
					bullets[j]->adjustSpeed(LAUNCHACCEL);
			}
		}
		// Update positions for non-spawners
		for (int i = PETALCOUNT; i < bullets.size(); i++)
			bullets[i]->processMovement();

		// Process spawner behavior
		if (phase == 4 || phase == 8) {
			// Do nothing during refresh delay
			return;
		}
		// Start layer 2
		if (frameCounter == spawnPoint + LAYER1CHECKPOINT) 
		{
			addWave();
			currentColorIndex = 1;
			adjustSpawners();
			// Adjust spawner velocity and position
			for (int i = 0; i < PETALCOUNT; i++) {
				// Angle towards starting point of layer 2. 
				float angle = 360 / PETALCOUNT * i + 180 + shotAngle + 90 / PETALCOUNT;
				bullets[i]->setVelocityR(adjustedSpawnerSpeed, angle + 180 / PETALCOUNT);
				bullets[i]->setPosition(sourcePos.x + 2 * RADIUS1 * cos(angle / 180 * PI), sourcePos.y + 2 * RADIUS1 * sin(angle / 180 * PI));
			}
		}
		// Start layer 3
		else if (frameCounter == spawnPoint + LAYER2CHECKPOINT) 
		{
			addWave();
			currentColorIndex = 2;
			adjustSpawners();
			// Adjust spawner velocity and position
			for (int i = 0; i < PETALCOUNT; i++) {
				// Angle towards starting point of layer 3
				float dist = 2.365 * RADIUS2; // No easy way of measuring this. Eyeballing from reference
				float angle = 360 / PETALCOUNT * i + 180 + shotAngle - 90 / PETALCOUNT;
				bullets[i]->setVelocityR(adjustedSpawnerSpeed, angle + 180 / PETALCOUNT);
				bullets[i]->setPosition(sourcePos.x + dist * cos(angle / 180 * PI), sourcePos.y + dist * sin(angle / 180 * PI));
			}
		}
		// Stop spawners after each flower
		else if (frameCounter == spawnPoint + LAYER3CHECKPOINT || frameCounter == spawnPoint + LAYER6CHECKPOINT + refreshFrames) {
			addWave();
			for (int i = 0; i < PETALCOUNT; i++)
				bullets[i]->setVelocity(0, 0);
		}
		
		// Start flower 2 
		// Start layer 1
		else if (frameCounter == spawnPoint + LAYER3CHECKPOINT + refreshFrames) {
			currentColorIndex = 3;
			adjustSpawners();
			shotAngle = rand() % 360; // Reroll shot angle

			// Adjust spawner velocity and position
			for (int i = 0; i < PETALCOUNT; i++) {
				bullets[i]->setVelocityR(adjustedSpawnerSpeed, 360 / PETALCOUNT * i + shotAngle);
				bullets[i]->setPosition(sourcePos.x, sourcePos.y);
			}
		}
		// Start layer 2
		else if (frameCounter == spawnPoint + LAYER4CHECKPOINT + refreshFrames) {
			addWave();
			currentColorIndex = 4;
			adjustSpawners();
			// Adjust spawner velocity and position
			for (int i = 0; i < PETALCOUNT; i++) {
				// Angle towards starting point of layer 2. 
				float angle = 360 / PETALCOUNT * i + shotAngle - 18;
				bullets[i]->setVelocityR(adjustedSpawnerSpeed, angle - 180 / PETALCOUNT);
				bullets[i]->setPosition(sourcePos.x + 2 * RADIUS1 * cos(angle / 180 * PI), sourcePos.y + 2 * RADIUS1 * sin(angle / 180 * PI));
			}
		}
		// Start layer 3
		else if (frameCounter == spawnPoint + LAYER5CHECKPOINT + refreshFrames) {
			addWave();
			currentColorIndex = 5;
			adjustSpawners();
			// Adjust spawner velocity and position
			for (int i = 0; i < PETALCOUNT; i++) {
				// Angle towards starting point of layer 3
				float dist = 2.365 * RADIUS2; // No easy way of measuring this. Eyeballing from reference
				float angle = 360 / PETALCOUNT * i + shotAngle + 90 / PETALCOUNT;
				bullets[i]->setVelocityR(adjustedSpawnerSpeed, angle - 180 / PETALCOUNT);
				bullets[i]->setPosition(sourcePos.x + dist * cos(angle / 180 * PI), sourcePos.y + dist * sin(angle / 180 * PI));
			}
		}
		else // Spawn bullets
		{
			int i = 0;
			// Adjusts bullet density by manipulating the number of iterations per frame
			if (cycleCounter % scaleDenom < scaleNumer % scaleDenom)
				i--; // Extend loop
			cycleCounter = (cycleCounter >= scaleDenom - 1) ? 0 : cycleCounter + 1;
			for (; i < scaleNumer / scaleDenom; i++) {
				for (int j = 0; j < PETALCOUNT; j++) {
					Bullet* bullet = bullets[j];
					// 90 aims bullets to petal centers as spawners are tangential. Also add variance to group by quads.
					if (phase < 4) {
						addTalismanBullet(bullet->getPosition(), 0, bullet->getRotation() + 90 + 15 + BULLETANGLEVARIANCE[varianceCounter] / bulletDensity, BULLETCOLORS[currentColorIndex]);
						bullet->rotateArc(currentCircleRadius, adjustedSpawnerSpeed);
					}
					else {
						addTalismanBullet(bullet->getPosition(), 0, bullet->getRotation() - 90 - 10 - BULLETANGLEVARIANCE[varianceCounter] / bulletDensity, BULLETCOLORS[currentColorIndex]);
						bullet->rotateArc(currentCircleRadius, -adjustedSpawnerSpeed);
					}
					bullet->processMovement();
					incrementCurrentBulletCount();
				}
				varianceCounter = varianceCounter >= BULLETANGLEVARIANCE.size() - 1 ? 0 : varianceCounter + 1;
			}
		}
		
	}
	void resetPattern() {
		Pattern::resetPattern();
		varianceCounter = 0;
		cycleCounter = 0;
	}
	// Calculate current pattern phase for cleaner code
	void calculatePhase() {
		using namespace MOF;
		// Using += for phases before a resting phase
		if (frameCounter < spawnPoint + LAYER1CHECKPOINT)
			phase = 1; // First flower, first layer
		else if (frameCounter < spawnPoint + LAYER2CHECKPOINT)
			phase = 2; // First flower, second layer
		else if (frameCounter <= spawnPoint + LAYER3CHECKPOINT)
			phase = 3; // First flower, third layer
		else if (frameCounter < spawnPoint + LAYER3CHECKPOINT + refreshFrames)
			phase = 4; // Resting phase before second flower
		else if (frameCounter < spawnPoint + LAYER4CHECKPOINT + refreshFrames)
			phase = 5; // Second flower, first layer
		else if (frameCounter < spawnPoint + LAYER5CHECKPOINT + refreshFrames)
			phase = 6; // Second flower, second layer
		else if (frameCounter <= spawnPoint + LAYER6CHECKPOINT + refreshFrames)
			phase = 7; // Second flower, third layer
		else if (frameCounter < spawnPoint + LAYER6CHECKPOINT + refreshFrames * 2)
			phase = 8; // Resting phase before pattern resets
	}
	// Grouping code for setting variables based on current layer
	void adjustSpawners() {
		using namespace MOF;
		if (phase == 1) { // Layer 1
			currentCircleRadius = RADIUS1;
			bulletDensity = EXPECTEDBULLETS1 / (1 - LAYER1CUT);
			scaleNumer = DSCALENUMER;
			scaleDenom = DSCALEDENOM;
			// Add offset to spawner speed to make bullets line up
			adjustedSpawnerSpeed = SPAWNERMOVESPEED * scaleDenom / scaleNumer - 0.1;
		}
		else if (phase == 2 || phase == 6) { // Layer 2
			currentCircleRadius = RADIUS2;
			bulletDensity = EXPECTEDBULLETS2 * 2;
			adjustedSpawnerSpeed = SPAWNERMOVESPEED * scaleDenom / scaleNumer + 0.2;
		}
		else if (phase == 3 || phase == 7) { // Layer 3
			bulletDensity = EXPECTEDBULLETS3 * 2;
			currentCircleRadius = RADIUS3;
			scaleNumer = DSCALENUMER3;
			scaleDenom = DSCALEDENOM3;
			adjustedSpawnerSpeed = FASTSPEEDMULTIPLIER * SPAWNERMOVESPEED * scaleDenom / scaleNumer - 0.25;
		}
		if (phase == 5) { // Layer 1, flower 2
			currentCircleRadius = RADIUS1;
			bulletDensity = EXPECTEDBULLETS1 * 1.2;
			scaleNumer = DSCALENUMER;
			scaleDenom = DSCALEDENOM;
			// Add offset to spawner speed to make bullets line up
			adjustedSpawnerSpeed = SLOWSPEEDMULTIPLIER * SPAWNERMOVESPEED * scaleDenom / scaleNumer - 0.1;
		}
	}
	
};

// Rings of curving orange and cyan circles
class MercuryPoison : public WavePattern {
	bool alternate;
	sf::Vector2f shotSource; // Shot source will change after 16 shots
	int shotCounter;
	int waveEnd; // Used to time delay between waves
public:
	MercuryPoison(int streamCount, sf::Vector2f sourcePos, float shotFrequency, float baseSpeed)
		:WavePattern(streamCount, shotFrequency, baseSpeed, sourcePos) {
		alternate = true;
		shotCounter = 0;
		shotSource = { sourcePos.x + rand() % 200 - 100, sourcePos.y + rand() % 100 - 50 };
		waveEnd = -HGP::WAVEDELAY;
		expandBounds(0.2);
	}
	void spawnBullets() {
		using namespace HGP;
		if (!active)
			return;
		if (frameCounter < waveEnd + WAVEDELAY) // Delay between waves
			return;
		if (isGoodToshoot()) {
			// Random angle and position
			int shotAngle = rand() % 360;
			for (int i = 0; i < streamCount; i++) {
				if (alternate) {
					addCircleBullet(shotSource, baseSpeed, shotAngle + i * 360.f / streamCount, ORANGE, BULLETSIZE);
					addCircleBullet(shotSource, baseSpeed * SPEEDMULTIPLIER, shotAngle + (i + 0.5) * 360.f / streamCount, ORANGE, BULLETSIZE);
				}
				else {
					addCircleBullet(shotSource, baseSpeed, shotAngle + i * 360.f / streamCount, CYAN, BULLETSIZE);
					addCircleBullet(shotSource, baseSpeed * SPEEDMULTIPLIER, shotAngle + (i + 0.5) * 360.f / streamCount, CYAN, BULLETSIZE);
				}
			}
			addWave(streamCount * 2);
			// Set the flags for rotation movement
			if (alternate)
				for (int i = bullets.size() - streamCount * 2; i < bullets.size(); i++)
					bullets[i]->setFlag(REVERSEROTATION);
			alternate = !alternate;
			if (++shotCounter >= WAVECOUNT) { // Reroll shot source
				shotCounter = 0;
				shotSource = { sourcePos.x + rand() % 200 - 100, sourcePos.y + rand() % 100 - 50 };
				waveEnd = frameCounter;
			}
			
		}
	}
	void processMovement() {
		using namespace HGP;
		if (!active)
			return;
		incrementWaveFrames();
		// Process movement and ring expansion through rotation speed
		for (int wave = 0; wave < waveBulletCount.size(); wave++) {
			int frameCount = waveFrameCount[wave];
			// Rotate each wave
			for (int j = getStartIndex(wave); j <= getEndIndex(wave); j++) {
				Bullet* bullet = bullets[j];
				bullet->processMovement();
				// Rotates the bullet only for a specific period in time
				if (frameCount > ROTATIONSTART && frameCount <= ROTATIONEND)
				{
					if (bullet->getFlag() == REVERSEROTATION)
						bullet->rotateBullet(ROTATIONANGLE);
					else
						bullet->rotateBullet(-ROTATIONANGLE);
				}	
			}
		}
	}
	void resetPattern() {
		Pattern::resetPattern();
		waveEnd = -HGP::WAVEDELAY;
		shotCounter = 0;
		alternate = true;
		shotSource = { sourcePos.x + rand() % 200 - 100, sourcePos.y + rand() % 100 - 50 };
	}
};

// Manager for all patterns. Will be called by main, GameScreen, and others.
class PatternManager : public sf::Drawable {
	vector<Pattern*> activePatterns;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (Pattern* pattern : activePatterns)
			target.draw(*pattern, states);
	}
public:
	PatternManager() {}
	~PatternManager() {
		for (Pattern* pattern : activePatterns)
			delete pattern;
	}
	// Adds a pattern to the manager. Patterns can either spawn bullets from an algorithm or function call.
	void addPattern(Pattern* pattern) {
		activePatterns.push_back(pattern);
	}
	// Call every frame. Delete, spawn, and move bullets
	void doStuff() {
		for (Pattern* pattern : activePatterns) {
			pattern->deleteOutOfBoundsBullets();
			pattern->spawnBullets();
			pattern->incrementFrame();
			pattern->processMovement();
		}
	}
	// Deactive all patterns and reset their counters
	void deactivateAllPatterns() {
		for (int i = 0; i < activePatterns.size(); i++) {
			activePatterns[i]->setActive(false);
			activePatterns[i]->resetPattern();
			if (i != 0) // Do not delete test bullets
				activePatterns[i]->deleteAllBullets();
		}
	}
	void rotateAllBullets(float angleDegrees) {
		for (Pattern* pattern : activePatterns)
			for (Bullet* bullet : pattern->getBullets())
				bullet->rotateBullet(angleDegrees);
	}
	// Check if player hitbox has collided with any bullets
	bool checkPlayerCollision(sf::CircleShape& hitbox) {
		for (Pattern* pattern : activePatterns)
			if (pattern->getActive())
				for (Bullet* bullet : pattern->getBullets())
					if (bullet->checkPlayerCollision(hitbox))
						return true;
		return false;
	}
	int getPatternCount() {
		return activePatterns.size();
	}
	Pattern* operator[](int index) {
		return activePatterns[index];
	}
};

