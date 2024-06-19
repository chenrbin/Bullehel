#pragma once

#include "Constants.h"
using namespace Constants;

// Modified sf::Clock for ease of use and pausing
class sfClockAtHome : public sf::Clock {
	sf::Time storage; // For pausing and resuming the clock;
	bool paused;
public:
	sfClockAtHome() {
		paused = false;
	}

	// Restarts both clock and storage
	sf::Time restart() {
		storage = sf::Time();
		paused = false;
		return sf::Clock::restart();
	}
	// Saves current time in storage
	void pause() {
		if (paused)
			return;
		storage += sf::Clock::getElapsedTime();
		sf::Clock::restart();
		paused = true;
	}
	// Resets current clock only
	void resume() {
		if (!paused)
			return;
		sf::Clock::restart();
		paused = false;
	}
	// Returns paused status
	bool getPaused() {
		return paused;
	}
	// Returns time values that include stored time from pausing
	sf::Time getElapsedTime() {
		return sf::Clock::getElapsedTime() + storage;
	}
	float getTimeSeconds() {
		return sf::Clock::getElapsedTime().asSeconds() + storage.asSeconds();
	}
	sf::Int32 getTimeMilliseconds() {
		return sf::Clock::getElapsedTime().asMilliseconds() + storage.asMilliseconds();
	}

};

// Class to handle auto repeat / DAS
class KeyTimer {
	sf::Clock startTimer, holdTimer;
	float startDelay, holdDelay; // In milliseconds
	bool startOn, holdOn;
public:
	KeyTimer(float startDelay = 0, float holdDelay = 0) {
		this->startDelay = startDelay;
		this->holdDelay = holdDelay;
		startOn = false;
		holdOn = false;
	}
	// Handles action while key is pressed. Returns true if an action should be performed
	bool press() {
		if (holdOn) { // If startOn and holdOn are true, check for auto repeat
			if (holdTimer.getElapsedTime().asMilliseconds() >= holdDelay) {
				holdTimer.restart();
				return true;
			}
			return false;
		}
		// If holdOn isn't true, check startOn for startTimer
		if (!startOn) {
			startOn = true;
			startTimer.restart();
			return true;
		}
		else {
			if (startTimer.getElapsedTime().asMilliseconds() >= startDelay) {
				holdOn = true;
				holdTimer.restart();
			}
			return false;
		}
	}
	// Key is released. Reset boolean values
	void release() {
		holdOn = false;
		startOn = false;
	}
	void setStartDelay(float val) {
		startDelay = val;
	}
	void setHoldDelay(float val) {
		holdDelay = val;
	}
};

// Controls for each player
class KeySet {
	sf::Keyboard::Key left;
	sf::Keyboard::Key right;
	sf::Keyboard::Key up;
	sf::Keyboard::Key down;
	sf::Keyboard::Key shoot;
	sf::Keyboard::Key bomb;
	sf::Keyboard::Key focus;
	vector<sf::Keyboard::Key*> setPointers;
public:
	KeySet(sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key up,
		sf::Keyboard::Key down, sf::Keyboard::Key shoot, sf::Keyboard::Key bomb, sf::Keyboard::Key focus) {
		this->left = left;
		this->right = right;
		this->up = up;
		this->down = down;
		this->shoot = shoot;
		this->bomb = bomb;
		this->focus = focus;

		setPointers = { &up, &left, &down, &right, &shoot, &bomb, &focus };
	}
	void setLeft(sf::Keyboard::Key key) {
		left = key;
	}
	void setRight(sf::Keyboard::Key key) {
		right = key;
	}
	void setUp(sf::Keyboard::Key key) {
		up = key;
	}
	void setDown(sf::Keyboard::Key key) {
		down = key;
	}
	void setShoot(sf::Keyboard::Key key) {
		shoot = key;
	}
	void setBomb(sf::Keyboard::Key key) {
		bomb = key;
	}
	void setFocus(sf::Keyboard::Key key) {
		focus = key;
	}
	sf::Keyboard::Key getLeft() {
		return left;
	}
	sf::Keyboard::Key getRight() {
		return right;
	}
	sf::Keyboard::Key getUp() {
		return up;
	}
	sf::Keyboard::Key getDown() {
		return down;
	}
	sf::Keyboard::Key getShoot() {
		return shoot;
	}
	sf::Keyboard::Key getBomb() {
		return bomb;
	}
	sf::Keyboard::Key getFocus() {
		return focus;
	}
	// Return a series of pointers to the keybinds
	vector<sf::Keyboard::Key*> getSet() {
		return { &up, &left, &down, &right, &shoot, &bomb, &focus };
	}
};

// Class for sound clips coming from a "soundboard" file
class SoundEffect : public sf::Sound {
	float startTime; // In seconds
	float duration; // In seconds
public:
	SoundEffect(sf::SoundBuffer& buffer, float startTime, float duration) {
		setBuffer(buffer);
		this->startTime = startTime;
		this->duration = duration;
	}
	void play() {
		sf::Sound::play();
		setPlayingOffset(sf::seconds(startTime));
	}
	// Check when the sound has met the duration and stop it.
	bool checkSound() {
		if (getPlayingOffset().asSeconds() > startTime + duration) {
			stop();
			return true;
		}
		return false;
	}
};

// Class for managing sound effects from a single buffer and keeping checks across classes
class SoundManager {
	vector<SoundEffect> soundEffects;
	sf::SoundBuffer buffer;
	map<float, int> timestamps; // For easier play function calls
public:
	SoundManager(string fileName) {
		if (!buffer.loadFromFile(fileName))
			cout << "Missing audio file";
	}
	void addEffect(float startTime) {
		timestamps.emplace(startTime, soundEffects.size());
		soundEffects.push_back(SoundEffect(buffer, startTime, 0));
	}
	void checkTimers() {
		for (SoundEffect& fx : soundEffects)
			fx.checkSound();
	}
	SoundEffect& operator[](int index) {
		return soundEffects[index];
	}
	void play(float time) {
		soundEffects[timestamps[time]].play();
	}
	void setVolume(float volume) {
		for (SoundEffect& fx : soundEffects)
			fx.setVolume(volume);
	}
	void pauseAll() {
		for (SoundEffect& fx : soundEffects)
			fx.pause();
	}
};

// Custom exception for config file reading error
class ConfigError : public exception {
public:
	const char* what() {
		return "Config file is not formatted correctly or does not exist";
	}
};
