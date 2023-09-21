#pragma once

// File to contain all bullet implementation

// Game projectiles
class Bullet : public sf::Drawable { // Abstract base class
protected:
	sf::Shape* sprite; // Base sprite
	char flag; // Flag code that will be used for various purposes
	float xVelocity, yVelocity;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(*sprite, states);
	}
public:
	Bullet(float xVelocity = 0, float yVelocity = 0) {
		sprite = nullptr;
		this->xVelocity = xVelocity;
		this->yVelocity = yVelocity;
		flag = NEUTRAL;
	}
	~Bullet() {
		delete sprite;
	}
	// Called to move every frame
	virtual void processMovement() {
		sprite->move(xVelocity, yVelocity);
	}
	// Rotate bullet direction
	virtual void rotate(float angleDegrees) {
		float currentSpeed = sqrt(pow(xVelocity, 2) + pow(yVelocity, 2));
		float currentAngle = atan(yVelocity / xVelocity) * 180 / PI; 
		if (xVelocity < 0)
			currentAngle += 180;
		xVelocity = cos((currentAngle + angleDegrees) * PI / 180) * currentSpeed;
		yVelocity = sin((currentAngle + angleDegrees) * PI / 180) * currentSpeed;
		sprite->rotate(angleDegrees);
	}
	// Adds an offset to position instead of setting it
	virtual void adjustPosition(float x, float y) {
		sprite->move(x, y);
	}
	// Adds an offset to velocity
	virtual void adjustVelocity(float x, float y) {
		xVelocity += x;
		yVelocity += y;
	}
	virtual void setPosition(sf::Vector2f position) {
		sprite->setPosition(position);
	}
	// Flip the x coordinate (reflection along the y axis)
	virtual void flipX() {
		sprite->rotate(-sprite->getRotation() * 2);
		xVelocity *= -1;
	}
	// Flip the y coordinate (reflection along the x axis)
	virtual void flipY() {
		sprite->rotate(-sprite->getRotation() * 2);
		yVelocity *= -1;
	}
	// Align sprite rotation based on velocity. Optionally add an offset
	void alignAngle() {
		float angle;
		if (yVelocity != 0 || xVelocity != 0)
			angle = atan(yVelocity / xVelocity) * 180 / PI + 90; // C++ handles atan(y/0), but not atan(0/0)
		else
			angle = 0;
		if (xVelocity < 0)
			angle += 180;
		sprite->setRotation(angle);
	}
	// Align sprite rotation based on velocity. Optionally add an offset
	void alignAngle(float xOffset, float yOffset) {
		float angle;
		if (yVelocity + yOffset != 0 || xVelocity + xOffset != 0)
			angle = atan((yVelocity + yOffset) / (xVelocity + xOffset)) * 180 / PI + 90; // C++ handles atan(y/0), but not atan(0/0)
		else
			angle = 0;
		if (xVelocity + xOffset < 0)
			angle += 180;
		sprite->setRotation(angle);
	}
	void setVelocity(float x, float y) {
		xVelocity = x;
		yVelocity = y;
	}
	void scaleVelocity(float x, float y) {
		xVelocity *= x;
		yVelocity *= y;
	}
	sf::Vector2f getVelocity() {
		return sf::Vector2f(xVelocity, yVelocity);
	}
	// Set flags used for certain patterns
	void setFlag(char val) {
		flag = val;
	}
	
	char getFlag() {
		return flag;
	}
	sf::Vector2f getPosition() {
		return sprite->getPosition();
	}
	
	// Circular hitboxes will override this. 
	virtual bool checkPlayerCollision(sf::CircleShape& hitbox) = 0;
};
#pragma region Bullet types
// Basic bullet with circular hitbox (sprite does not have to be circular). Can be a bullet type by itself.
class CircleBullet : virtual public Bullet {
protected:
	int hitBoxRadius; // Used for collision detection;
public:
	CircleBullet(sf::Vector2f position = SCREENPOS, float xVelocity = 0, float yVelocity = 0, sf::Color color = WHITE, int radius = 0) : Bullet(xVelocity, yVelocity) {
		sprite = new SfCircleAtHome(WHITE, radius, position, true, color, STANDARDCIRCLEOUTLINE);
		hitBoxRadius = radius;
	}
	bool checkPlayerCollision(sf::CircleShape& playerHitbox) {
		sf::Vector2f hitBoxPos = playerHitbox.getPosition();
		sf::Vector2f bulletPos = sprite->getPosition();
		return sqrt(pow(hitBoxPos.x - bulletPos.x, 2) + pow(hitBoxPos.y - bulletPos.y, 2)) <= playerHitbox.getRadius() + hitBoxRadius;
	}

};

// Bullets that require multiple shapes to draw
class ComplexBullet : virtual public Bullet {
protected:
	vector<sf::Shape*> extraSprites;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(*sprite, states);
		for(sf::Shape* extraSprite : extraSprites)
			target.draw(*extraSprite, states);
	}
public:
	ComplexBullet() : Bullet(){}
	~ComplexBullet() {
		for (sf::Shape* extraSprite : extraSprites)
			delete extraSprite;
	}
	// Called to move every frame
	virtual void processMovement() {
		sprite->move(xVelocity, yVelocity);
		for (sf::Shape* extraSprite : extraSprites)
			extraSprite->move(xVelocity, yVelocity);
	}
	// Rotate bullet direction
	virtual void rotate(float angleDegrees) {
		float currentSpeed = sqrt(pow(xVelocity, 2) + pow(yVelocity, 2));
		float currentAngle = sprite->getRotation() - 90; // Default orientation is facing down. Setting current angle to have 0 at facing right
		xVelocity = cos((currentAngle + angleDegrees) / 180 * PI) * currentSpeed;
		yVelocity = sin((currentAngle + angleDegrees) / 180 * PI) * currentSpeed;
		sprite->rotate(angleDegrees);
		for (sf::Shape* extraSprite : extraSprites)
			extraSprite->rotate(angleDegrees);
	}
	virtual void setPosition(sf::Vector2f position) {
		sprite->setPosition(position);
		for (sf::Shape* extraSprite : extraSprites)
			extraSprite->setPosition(position);

	}
	// Flip the x coordinate (reflection along the y axis)
	virtual void flipX() {
		sprite->rotate(-sprite->getRotation() * 2);
		for (sf::Shape* extraSprite : extraSprites)
			extraSprite->rotate(-extraSprite->getRotation() * 2);
		xVelocity *= -1;
	}
	// Flip the y coordinate (reflection along the x axis)
	virtual void flipY() {
		sprite->rotate(-sprite->getRotation() * 2);
		for (sf::Shape* extraSprite : extraSprites)
			extraSprite->rotate(-extraSprite->getRotation() * 2);
		yVelocity *= -1;
	}
};

// Laser with instantaneous travel time. Rectangular hitbox;
class Laser : public ComplexBullet {
protected:
	float growthSpeed;
	float maxWidth;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(*sprite, states);
		for (sf::Shape* extraSprite : extraSprites)
			target.draw(*extraSprite, states);
	}
public:
	Laser(sf::Vector2f position = SCREENPOS, float angleDegrees = 0, float maxWidth = 0, sf::Color color = WHITE, float growthSpeed = 1)
		: Bullet(), ComplexBullet() {
		this->growthSpeed = growthSpeed;
		this->maxWidth = maxWidth;
		SfRectangleAtHome* rect = new SfRectangleAtHome(WHITE, { WINDOWWIDTH, 3 }, position, false, color, 1);
		rect->alignX();
		sprite = rect;
		sprite->rotate(angleDegrees);
		extraSprites.push_back(new SfCircleAtHome(WHITE, maxWidth, position, true, color, STANDARDCIRCLEOUTLINE));
	}
	void rotate(float angleDegrees) {
		sprite->rotate(angleDegrees);
	}
	// Process the active status and growth of laser
	virtual void processMovement() {
		sprite->move(xVelocity, yVelocity);
		for (sf::Shape* extraSprite : extraSprites)
			extraSprite->move(xVelocity, yVelocity);
	}
	// Adds an offset to position instead of setting it
	virtual void adjustPosition(float x, float y) {
		sprite->move(x, y);
	}
	bool checkPlayerCollision(sf::CircleShape& hitbox) { // Try equation with slope
		return sprite->getGlobalBounds().contains(hitbox.getPosition());
	}
};

class RiceBullet : public CircleBullet {
public:
	RiceBullet(sf::Vector2f position = SCREENPOS, float xVelocity = 0, float yVelocity = 0, sf::Color color = WHITE, int radius = 0) 
		: CircleBullet(position, xVelocity, yVelocity, color, radius), Bullet(xVelocity, yVelocity) {
		sprite->setOutlineThickness(SMALLCIRCLEOUTLINE);
		alignAngle();
		sprite->scale(1, 2);
	}
};
class DotBullet : public CircleBullet {
public:
	DotBullet(sf::Vector2f position = SCREENPOS, float xVelocity = 0, float yVelocity = 0, sf::Color color = WHITE, int radius = 0) 
		: CircleBullet(position, xVelocity, yVelocity, color, radius), Bullet(xVelocity, yVelocity) {
		sprite->setOutlineThickness(SMALLCIRCLEOUTLINE);
	}
};
class TalismanBullet : public CircleBullet {
public:
	TalismanBullet(sf::Vector2f position = SCREENPOS, float xVelocity = 0, float yVelocity = 0, sf::Color color = WHITE, int radius = 0) 
		: CircleBullet(position, xVelocity, yVelocity, color, radius), Bullet(xVelocity, yVelocity) {
		delete sprite;
		sprite = new SfRectangleAtHome(TRANSPARENTWHITE, { 3.f * radius, 4.f * radius }, position, true, color, STANDARDCIRCLEOUTLINE);
		alignAngle();
	}
};
class BubbleBullet : public ComplexBullet, public CircleBullet {
public:
	BubbleBullet(sf::Vector2f position = SCREENPOS, float xVelocity = 0, float yVelocity = 0, sf::Color color = WHITE, int radius = 0)
		: CircleBullet(position, xVelocity, yVelocity, color, radius), Bullet(xVelocity, yVelocity) {
		// Four concentric circles with colors: transparent, color darkened and partially transparent, original color, white
		sprite->setFillColor(TRANSPARENT);
		sprite->setOutlineThickness(radius * 0.6);
		sf::Color copyColor = color;
		color.a = 200;
		color.r *= 0.9;
		color.g *= 0.9;
		color.b *= 0.9;
		sprite->setOutlineColor(color);
		extraSprites.push_back(new SfCircleAtHome(TRANSPARENT, radius * 1.3, position, true, copyColor, radius * 0.6));
		extraSprites.push_back(new SfCircleAtHome(TRANSPARENT, radius * 1.9, position, true, TRANSPARENTWHITE, radius * 0.75));
	}
};
 

#pragma endregion

// Class to store bullet pattern templates. The base class is for random bullets and children have specific patterns.
class Pattern : public sf::Drawable {
protected:
	sf::FloatRect screenBounds; // Determines the bounds where the bullets can exist
	vector<Bullet*> bullets;
	// Timing
	float duration; // How long the pattern is active in seconds. Does not deactivate if zero
	int frameCounter; // Used as a timer
	bool active;

	// Bullet properties
	int streamCount;
	float baseSpeed;
	sf::Vector2f sourcePos;
	// Amount of shots per second, up to the framerate. FPS / shotFrequency = frames per shot
	// Shoots only once if set to zero
	float shotFrequency;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		if (active)
			for (Bullet* bullet : bullets)
				target.draw(*bullet, states);
	}
public:
	Pattern(float duration = 0, int streamCount = 0, float shotFrequency = 0, float baseSpeed = 0, sf::Vector2f sourcePos = SCREENPOS) {
		frameCounter = 0;
		active = true;
		screenBounds = SCREENBOUNDS;
		expandBounds(0.1);
		this->duration = duration;
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
		for (Bullet* bullet : bullets)
			bullet->processMovement();
	}
	// Increment frame counter
	void incrementFrame() {
		if(active)
			frameCounter++;
	}
	vector<Bullet*>& getBullets() {
		return bullets;
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
	// Not used by base class
	virtual void spawnBullets() {
		return;
	}
	// Compares shotFrequency with frameCounter and is used by spawnBullets to determine when to shoot.
	bool isGoodToshoot() {
		int framesPerShot;
		if (shotFrequency == 0) {
			framesPerShot = 1;
			active = false;
		}
		else
			framesPerShot = FPS / shotFrequency;
		return frameCounter % framesPerShot == 0;
	}
	// Reset frame counter
	void resetCounter() {
		frameCounter = 0;
	}
	// Delete all bullets. Typically paired with resetCounter
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

#pragma region Add bullets
	// All addBullet functions return a pointer to the bullet create
// Add circle bullet with velocity in rectangular coordinates
	void addCircleBullet(sf::Vector2f position, sf::Vector2f velocity, sf::Color color = DEFAULTCIRCLEBULLETCOLOR, int radius = STANDARDCIRCLEBULLETRADIUS) {
		bullets.push_back(new CircleBullet(position, velocity.x, velocity.y, color, radius));
	}
	// Add circle bullet with velocity in polar coordinates
	void addCircleBulletR(sf::Vector2f position, float speed = 0, float angleDegrees = 0, sf::Color color = DEFAULTCIRCLEBULLETCOLOR, int radius = STANDARDCIRCLEBULLETRADIUS) {
		bullets.push_back(new CircleBullet(position, speed * cos(angleDegrees * PI / 180), speed * -sin(angleDegrees * PI / 180), color, radius));
	}
	// Add rice bullet with velocity in polar coordinates
	void addRiceBulletR(sf::Vector2f position, float speed = 0, float angleDegrees = 0, sf::Color color = DEFAULTRICEBULLETCOLOR, int radius = STANDARDRICEBULLETRADIUS) {
		bullets.push_back(new RiceBullet(position, speed * cos(angleDegrees * PI / 180), speed * -sin(angleDegrees * PI / 180), color, radius));
	}
	// Add rice bullet with velocity in polar coordinates
	void addDotBulletR(sf::Vector2f position, float speed = 0, float angleDegrees = 0, sf::Color color = DEFAULTDOTBULLETCOLOR, int radius = STANDARDDOTBULLETRADIUS) {
		bullets.push_back(new DotBullet(position, speed * cos(angleDegrees * PI / 180), speed * -sin(angleDegrees * PI / 180), color, radius));
	}
	// Add talisman bullet with velocity in polar coordinates
	void addTalismanBulletR(sf::Vector2f position, float speed = 0, float angleDegrees = 0, sf::Color color = DEFAULTTALISMANBULLETCOLOR, int radius = STANDARDTALISMANBULLETRADIUS) {
		bullets.push_back(new TalismanBullet(position, speed * cos(angleDegrees * PI / 180), speed * -sin(angleDegrees * PI / 180), color, radius));
	}
	// Add bubble bullet with velocity in polar coordinates
	void addBubbleBulletR(sf::Vector2f position, float speed = 0, float angleDegrees = 0, sf::Color color = DEFAULTBUBBLEBULLETCOLOR, int radius = STANDARDBUBBLEBULLETRADIUS) {
		bullets.push_back(new BubbleBullet(position, speed * cos(angleDegrees * PI / 180), speed * -sin(angleDegrees * PI / 180), color, radius));
	}
	// Add laser with start position and angle
	void addLaser(sf::Vector2f position, float angleDegrees = 0, float maxWidth = 0, sf::Color color = BLUE, float growthSpeed = 1) {
		bullets.push_back(new Laser(position, angleDegrees, maxWidth, color, growthSpeed));
	}

#pragma endregion

};

// Direct stream with accelerating angle velocity
class Bowap : public Pattern {
	float offset;
	float angleVelocity;
	float angleAcceleration;
public:
	Bowap(float offset, float angleVelocity, float angleAcceleration, int streamCount, sf::Vector2f sourcePos, float shotFrequency, float baseSpeed, float duration = 0)
		: Pattern(duration, streamCount, shotFrequency, baseSpeed, sourcePos) {
		this->offset = offset;
		this->angleVelocity = angleVelocity;
		this->angleAcceleration = angleAcceleration;
	}
	void spawnBullets() {
		if (!active)
			return;
		if (isGoodToshoot()) {
			int num = offset + angleVelocity * frameCounter / FPS + angleAcceleration * pow(frameCounter / FPS, 2);
			for (int i = 0; i < streamCount; i++)
				addRiceBulletR(sourcePos, baseSpeed, num + i * 360 / streamCount);
		}
	}

};

// Ring of bullets, bounces off top left right walls once.
class QedRipples : public Pattern{ // Todo: speed up phases
	sf::FloatRect bounceBounds;
public:
	QedRipples(int streamCount, sf::Vector2f sourcePos, float shotFrequency, float baseSpeed, sf::FloatRect bounceBounds = SCREENBOUNDS, float duration = 0)
		:Pattern(duration, streamCount, shotFrequency, baseSpeed, sourcePos) {
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
				if (bullet->getFlag() != BOUNCED && (pos.x < bounceBounds.left || pos.x > bounceBounds.left + bounceBounds.width)) {
					bullet->flipX();
					bullet->setFlag(BOUNCED);
				}
				else if (pos.y < bounceBounds.top) {
					bullet->flipY();
					bullet->setFlag(BOUNCED);
				}
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
				shotSource = { sourcePos.x + rand() % 400 - 200, sourcePos.y + rand() % 200 - 100 };
			else // Fixed position for first shot
				shotSource = sourcePos;
			for (int i = 0; i < streamCount; i++)
				addRiceBulletR(shotSource, baseSpeed, shotAngle + i * 360 / streamCount, BLUE);
		}
	}
};

// Ring of bullets that rotate along an imaginary circle that moves down and expands
class FlyingSaucer : public Pattern{
	float baseCircleRadius;
	bool alternate; // Alternate rotation
	vector<sf::Vector2f> shotSources;
	vector<int> waveBulletCount; // Stores the number of remaining bullets per wave
	vector<int> waveFrameCount; // Stores the time each wave has been active
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (Bullet* bullet : bullets)
			target.draw(*bullet, states);
	}
public:
	FlyingSaucer(int streamCount, sf::Vector2f sourcePos, float shotFrequency, float baseSpeed, float baseCircleRadius, float duration = 0)
		:Pattern(duration, streamCount, shotFrequency, baseSpeed, sourcePos) {
		this->baseCircleRadius = baseCircleRadius;
		alternate = false;
		shotSources.push_back({ sourcePos.x - 180, sourcePos.y + 50 });		
		shotSources.push_back({ sourcePos.x + 180, sourcePos.y + 50 });
		shotSources.push_back({ sourcePos.x + 100, sourcePos.y - 150 });
		shotSources.push_back({ sourcePos.x - 100, sourcePos.y - 150 });
		expandBounds(1);
	}
	void processMovement() {
		// Increment wave frame timers
		for (int& counter : waveFrameCount) 
			counter++;
		
		int index = 0; // For keeping track of each bullet in each wave
		// Process movement and ring expansion through decreased rotation speed
		for (int wave = 0; wave < waveBulletCount.size(); wave++) {
			// Bigger number means bigger circle
			const float PHASE2VELOCITY = 90;
			const float PHASE3VELOCITY = -20;

			const int PHASE1CHECKPOINT = 60, PHASE2CHECKPOINT = 360;
			
			// Calculate starting velocity and acceleration using target radius and acceleration after phase 1
			float targetPos = 180;
			float targetVel = 45;
			float targetTime = 1;
			float accel = (targetTime * targetVel - targetPos) / targetTime / targetTime;
			float startVel = targetVel - 2 * accel * targetTime;

			float targetRadius = 0;
			// Determine speed of ring expansion
			if (waveFrameCount[wave] < PHASE1CHECKPOINT) {
				targetRadius = baseCircleRadius + waveFrameCount[wave] / FPS * startVel + pow(waveFrameCount[wave] / FPS, 2) * accel;
			}
			else if (waveFrameCount[wave] < PHASE2CHECKPOINT) {
				targetRadius = baseCircleRadius + PHASE1CHECKPOINT / FPS * startVel + pow(PHASE1CHECKPOINT / FPS, 2) * accel + (waveFrameCount[wave] - PHASE1CHECKPOINT) / FPS * PHASE2VELOCITY;
			}
			else {
				targetRadius = baseCircleRadius + PHASE1CHECKPOINT / FPS * startVel + pow(PHASE1CHECKPOINT / FPS, 2) * accel + (PHASE2CHECKPOINT - PHASE1CHECKPOINT) / FPS * PHASE2VELOCITY + (waveFrameCount[wave] - PHASE2CHECKPOINT) / FPS * PHASE3VELOCITY;
			}
			for (int j = 0; j < waveBulletCount[wave]; j++) {
				Bullet* bullet = bullets[index];

				bullet->processMovement();
				if (bullet->getFlag() == NEUTRAL)
					bullet->rotate(baseSpeed * 360 / (2 * PI * (targetRadius)));
				else
					bullet->rotate(-baseSpeed * 360 / (2 * PI * (targetRadius)));
				index++;
				if (waveFrameCount[wave] < PHASE2CHECKPOINT) 
					bullet->adjustPosition(0, 1);
				else
					bullet->adjustPosition(0, 1.1);
					

			}
		}
			
	}
	void spawnBullets() {
		if (!active)
			return;
		int shotAngle = rand() % 360;
		shotAngle = 90;
		int sourceCount = 0;
		if (isGoodToshoot()) {
			for (sf::Vector2f pos : shotSources) {
				int index = bullets.size(); // Save index to set flags
				for (int i = 0; i < streamCount; i++) {
					addTalismanBulletR({ pos.x + cos((shotAngle + i * 360.f / streamCount + 90) * PI / 180) * baseCircleRadius, pos.y + sin((shotAngle + i * 360.f / streamCount - 90) * PI / 180) * baseCircleRadius }, baseSpeed, shotAngle + i * 360 / streamCount, RED);
				}
				bool alternateCondition;
				if (alternate)
					alternateCondition = pos.x > sourcePos.x ^ pos.y > sourcePos.y;
				else
					alternateCondition = !(pos.x > sourcePos.x ^ pos.y > sourcePos.y);
				if (alternateCondition) {
					for (; index < bullets.size(); index++) {
						bullets[index]->setFlag(REVERSEROTATION);
						bullets[index]->rotate(180);
					}
				}	
				
				sourceCount++;
				if (sourceCount == shotSources.size() / 2) // Reroll rng
					shotAngle = rand() % 360;
			}

			waveBulletCount.push_back(shotSources.size() * streamCount);
			waveFrameCount.push_back(0);
			alternate = !alternate;

		}
	}
	virtual void deleteAllBullets() {
		Pattern::deleteAllBullets();
		waveBulletCount.clear();
		waveFrameCount.clear();
	}
	void deleteOutOfBoundsBullets() {
		for (int i = 0; i < bullets.size(); i++)
			if (!screenBounds.contains(bullets[i]->getPosition())) {
				Bullet* temp = bullets[i];
				bullets.erase(bullets.begin() + i);
				delete temp;

				// Decrement waveBulletCount
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
};

class GengetsuTime : public Pattern {
public:
	GengetsuTime(int streamCount, sf::Vector2f sourcePos, float shotFrequency, float baseSpeed, float duration = 0)
		:Pattern(duration, streamCount, shotFrequency, baseSpeed, sourcePos) {
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
					addDotBulletR(shotSource, baseSpeed, shotAngle + i * 360 / streamCount);
				else
					addCircleBulletR(shotSource, baseSpeed, shotAngle + i * 360 / streamCount, BLUE);
			}
		}
	}
};

// Manager for all bullet objects. Will be called by main, GameScreen, and others.
class BulletManager : public sf::Drawable {
	vector<Pattern*> activePatterns;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (Pattern* pattern :activePatterns)
			target.draw(*pattern, states);
	}
public:
	BulletManager() {}
	~BulletManager() {
		for (Pattern* pattern : activePatterns)
			delete pattern;
	}
	// Checks that will occur every frame
	void doStuff() {
		deleteOutOfBoundsBullets();
		spawnPatterns();
		processMovement();
	}
	// Adds a pattern to the manager. Patterns can either spawn bullets from an algorithm or function call.
	void addPattern(Pattern* pattern) {
		activePatterns.push_back(pattern);
	}
	void spawnPatterns() {
		for (Pattern* pattern : activePatterns)
			pattern->spawnBullets();
	}
	// Deactive all patterns and reset their counters
	void deactivateAllPatterns() {
		for (int i = 0; i < activePatterns.size(); i++) {
			activePatterns[i]->setActive(false);
			activePatterns[i]->resetCounter();
			if (i != 0) // Do not delete test bullets
				activePatterns[i]->deleteAllBullets();
		}
	}
	
	// Call this function every frame to move bullets
	void processMovement() {
		for (Pattern* pattern : activePatterns) {
			pattern->incrementFrame();
			pattern->processMovement();
		}
	}
	void rotateAllBullets(float angleDegrees) {
		for (Pattern* pattern : activePatterns)
			for (Bullet* bullet : pattern->getBullets()) 
				bullet->rotate(angleDegrees);
	}
	// Delete any bullets that are out of bounds
	void deleteOutOfBoundsBullets() {
		for (Pattern* pattern : activePatterns) {
			pattern->deleteOutOfBoundsBullets();
		}
	}
	// Check if player hitbox has collided with any bullets
	bool checkPlayerCollision(sf::CircleShape& hitbox) {
		for (Pattern* pattern : activePatterns)
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



