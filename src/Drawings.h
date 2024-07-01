#pragma once
#include "Constants.h"
#include "Mechanisms.h"

#pragma region Sf Sprites At Home
using namespace std;
using namespace Constants;
// Class for easy sf::Text generation. Replaces the setText function
class SfTextAtHome : public sf::Text {
public:
	SfTextAtHome() {}
	SfTextAtHome(sf::Font& font, sf::Color color, string message, unsigned int textSize, sf::Vector2f position, bool bold = true, bool underlined = false, bool centered = false, bool outline = false) {
		setFont(font);
		setFillColor(color);
		setString(message);
		setCharacterSize(textSize);
		if (centered)
			alignCenter();
		setPosition(position);
		if (bold)
			setStyle(sf::Text::Bold);
		if (underlined)
			setStyle(sf::Text::Underlined);
		if (outline)
		{
			setOutlineColor(BLACK);
			setOutlineThickness(1);
		}
	}
	void alignCenter() {
		const sf::FloatRect box = getLocalBounds();
		setOrigin(box.width / 2.0f, box.height / 2.0f);
	}
	bool contains(float x, float y) {
		return getGlobalBounds().contains(x, y);
	}
};

// Class for easy sf::RectangleShape generation.
class SfRectangleAtHome : public sf::RectangleShape {
public:
	SfRectangleAtHome() {}
	SfRectangleAtHome(sf::Color color, sf::Vector2f size, sf::Vector2f position, bool centered = false, sf::Color outlineColor = sf::Color(), float outlineThickness = 0) {
		setSize(size);
		setFillColor(color);
		if (centered)
			alignCenter();
		setPosition(position);
		setOutlineColor(outlineColor);
		setOutlineThickness(outlineThickness);
	}
	void alignCenter() {
		const sf::FloatRect box = getLocalBounds();
		setOrigin(box.width / 2.0f, box.height / 2.0f);
	}
	void alignX() {
		setOrigin(getSize().x / 2, getOrigin().y);
	}
	void alignY() {
		setOrigin(getOrigin().x, getSize().y / 2);
	}
	bool contains(float x, float y) {
		return getGlobalBounds().contains(x, y);
	}
};

// Class for easy sf::Circle generation
class SfCircleAtHome : public sf::CircleShape {
public:
	SfCircleAtHome() {}
	SfCircleAtHome(sf::Color color, float radius, sf::Vector2f position, bool centered = false, sf::Color outlineColor = sf::Color(), float outlineThickness = 0) {
		setRadius(radius);
		setFillColor(color);
		if (centered)
			alignCenter();
		setPosition(position);
		setOutlineColor(outlineColor);
		setOutlineThickness(outlineThickness);
	}
	void alignCenter() {
		setOrigin(getRadius(), getRadius());
	}
	bool contains(float x, float y) {
		return getGlobalBounds().contains(x, y);
	}
};
#pragma endregion

// Class for a text that can be navigated and clicked
class ClickableMenu : public sf::Drawable {
	vector<SfTextAtHome> texts;
	sf::CircleShape cursor; // Takes in a shape as the cursor. Must preset attributes.
	int cursorPos;

	// Draw all menu components
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (int i = 0; i < texts.size(); i++)
			target.draw(texts[i], states);
		target.draw(cursor, states);
	}
public:
	ClickableMenu() {
		cursorPos = 0;
	}
	ClickableMenu(sf::Font& font, sf::Color color, vector<string>& menuText, int textSize, sf::Vector2f startPos, int spacing, sf::CircleShape cursor) {
		for (int i = 0; i < menuText.size(); i++)
			texts.push_back(SfTextAtHome(font, color, menuText[i], textSize, { startPos.x, startPos.y + spacing * i }));
		this->cursor = cursor;
		cursor.setPosition(startPos.x - 5, startPos.y);
		cursorPos = 0;
		updateCursor();
	}
	void moveUp() {
		cursorPos--;
		if (cursorPos < 0)
			cursorPos = texts.size() - 1;
		updateCursor();
	}
	void moveDown() {
		cursorPos++;
		if (cursorPos >= texts.size())
			cursorPos = 0;
		updateCursor();
	}
	// Update cursor position to the left of the text it is pointing to
	void updateCursor() {
		cursor.setPosition(texts[cursorPos].getPosition().x - 5, texts[cursorPos].getPosition().y + 5);
	}
	// Update cursor based on mouse position. Return true if a text is selected
	bool onMouseClick(int x, int y) {
		for (int i = 0; i < texts.size(); i++) {
			if (texts[i].contains(x, y)) {
				cursorPos = i;
				updateCursor();
				return true;
			}
		}
		return false;
	}
	// Slightly different from mouse click. Needed for sound effect conditionals
	bool onMouseMove(int x, int y) {
		for (int i = 0; i < texts.size(); i++) {
			if (texts[i].contains(x, y)) {
				if (cursorPos != i) {
					cursorPos = i;
					updateCursor();
					return true;
				}
				else
					return false;
			}
		}
		return false;
	}
	int getCursorPos() {
		return cursorPos;
	}
	void resetCursorPos() {
		cursorPos = 0;
		updateCursor();
	}
};

// Animations are meant to be each created once and restarted when played
class Animation {
protected:
	sfClockAtHome startTime;
	float duration;

public:
	Animation() {
		duration = 0;
	}
	// Draws the animation to the window
	virtual void drawAnimation(sf::RenderWindow& window) = 0;
	virtual ~Animation() {};
	// Turns on animation
	virtual void restart() = 0;
	// Return true if animation has expired
	virtual bool isOver() {
		return startTime.getTimeSeconds() > duration;
	}
};

// Text that fades after a set duration
class FadeText : public Animation {
	float fadeDuration; // In seconds
	SfTextAtHome text; // Text to display. 
	sf::Color textColor;
public:
	FadeText(SfTextAtHome text, float duration, float fadeDuration) {
		this->text = text;
		this->duration = duration;
		this->fadeDuration = fadeDuration;
		startTime.restart();
		textColor = text.getFillColor();
		textColor.a = 0; // This disables the animation when constructed
		this->text.setFillColor(textColor);
	}
	// Draws the animation to the window
	void drawAnimation(sf::RenderWindow& window) {
		float elapsedTime = startTime.getElapsedTime().asSeconds();
		if (elapsedTime > duration + fadeDuration)
			return; // Returns nothing if animation is past duration
		if (elapsedTime > duration && textColor.a > 0) { // Begin fading
			textColor.a = ((1 - (elapsedTime - duration) / fadeDuration) * 255);
			text.setFillColor(textColor);
		}
		window.draw(text);
	}
	// Turns on animation
	void restart() {
		startTime.restart();
		textColor.a = 255;
		text.setFillColor(textColor);
	}
	// Change the text to display
	void setString(string str) {
		text.setString(str);
	}
};

// Organize drawable rectangle elements
class BorderRects {
	SfRectangleAtHome background;
	SfRectangleAtHome border1;
	SfRectangleAtHome border2;
public:
	BorderRects() {
		background = SfRectangleAtHome(GRAY, { SCREENWIDTH, SCREENHEIGHT }, { SCREENLEFT, SCREENTOP });
		border1 = SfRectangleAtHome(TRANSPARENT, { SCREENWIDTH, SCREENHEIGHT }, { SCREENLEFT, SCREENTOP }, false, WHITE, 1);
		border2 = SfRectangleAtHome(TRANSPARENT, { SCREENWIDTH + 2, SCREENHEIGHT + 2 }, { SCREENLEFT - 1, SCREENTOP - 1 }, false, BLACK, BORDERMARGIN);
	}
	SfRectangleAtHome& getBackground() {
		return background;
	}
	SfRectangleAtHome& getBorder1() {
		return border1;
	}
	SfRectangleAtHome& getBorder2() {
		return border2;
	}
};
