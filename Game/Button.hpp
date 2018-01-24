#pragma  once

#include <SFML/Graphics.hpp>
#include "Rectangle.hpp"
#include "Label.hpp"
#include "EventSource.hpp"
#include "EventConnection.hpp"
#include "Mouse.hpp"

class Button {
private:
    Rectangle background;
    sf::Vector2f backgroundSize;
    sf::Vector2f position;
    Label textLabel;

	bool mouseInside = false;
	bool isPressed = false;
/**
 * @brief Connect all used mouse events to the eventmanager..
 */
	void bindEvents() {
		game.mouse.mouseLeftButtonDown.connect([this](const sf::Vector2i mousePos) {
			if (background.contains(static_cast<sf::Vector2f>(mousePos))) {
				buttonPressed.fire();
				isPressed = true;
			}
		});

		game.mouse.mouseLeftButtonUp.connect([this](const sf::Vector2i mousePos) {
			if (background.contains(static_cast<sf::Vector2f>(mousePos)) && isPressed) {
				buttonReleased.fire();
				isPressed = false;
			}
		});

		game.mouse.mouseMoved.connect([this](const sf::Vector2i mousePos) {
			if (background.contains(static_cast<sf::Vector2f>(mousePos))) {
				if (mouseInside == false) {
					mouseInside = true;
					mouseEnter.fire();
				}
			}
			else {
				if (mouseInside == true) {
					mouseInside = false;
					mouseLeave.fire();
				}
			}
		});
	}

public:
	EventSource<> buttonPressed;
	EventSource<> buttonReleased;
	EventSource<> mouseEnter;
	EventSource<> mouseLeave;
./**
 *  @brief Empty constructor for button, create a empty button without text.
 */
	Button() : textLabel() {
		bindEvents();
	}

	/**
	 * @brief Constructor for a button with were most options can be set.
	 * @param backgroundSize The size of the background rectangle the button is based on.
	 * @param position The position of the rectangle the button is based on.
	 * @param size The size of the text on the button.
	 * @param text The string on the button. This string will be placed in the center of the button.
	 * @param font The font of the text on the button.
	 */
    Button(sf::Vector2f backgroundSize, sf::Vector2f position, int size, std::string text, sf::Font& font):
		backgroundSize(backgroundSize),
		position(position),
		textLabel(text, font, position, size)
	{
        background.setSize(backgroundSize);
        background.setPosition(position);
		background.setFillColor(sf::Color(0, 153, 51));
        sf::FloatRect textRectangle = textLabel.getBounds();
        textLabel.setOrigin(textRectangle.width / 2, textRectangle.height / 2);

		bindEvents();
    };

	/**
	 * @brief Set the size of the background rectangle a button is based on.
	 * @param size Vector with the size of the background.
	 */
	void setSize(sf::Vector2f size) {
		background.setSize(size);
	}
/**
 * @brief Set the character size for the text that is displayed on the button.
 * @param size The character size.
 */
	void setCharSize(int32_t size) {
		textLabel.setCharSize(size);
	}
	/**
	 * @brief Set the font for the text on the button.
	 * @param font The font for the text on the button.
	 */
	void setFont(sf::Font& font) {
		textLabel.setFont(font);
	}

	/**
	 * @brief Set the text that will be displayed on the button. The text is automaticly placed in the center of the button.
	 * @param text A string with the text that must be displayed on the button.
	 */
    void setText(const std::string& text){
        textLabel.setText(text);
        sf::FloatRect textRectangle= textLabel.getBounds();
		textLabel.setOrigin(textRectangle.width / 2, textRectangle.height / 2);
    }
	/**
	 * @brief The color of the button.
	 * @param color The color for the button.
	 */
	void setBackgroundColor(sf::Color color) {
		background.setFillColor(color);
	}

	/**
	 * @brief Set the color for the text on the button.
	 * @param color The for the text.
	 */
	void setTextColor(sf::Color color) {
		textLabel.setColor(color);
	}

	/**
	 * @brief The postion of the button.
	 * @param position The position fo the button.
	 */
	void setPosition(sf::Vector2f position) {
		background.setPosition(position);
		textLabel.setPosition(position);
	}

	/**
	 * @brief The outer size of the button.
	 * @return Vector with the size of the button.
	 */
	sf::Vector2f getSize() const {
		return background.getSize();
	}

	/**
	 * @brief The postion of the button.
	 * @return A vector with the position of the button.
	 */
	sf::Vector2f getPosition() const {
		return background.getPosition();
	}

	/**
	 * @brief Draw the button on a display.
	 * @param window The display the button will be displayed on.
	 */
    void draw(sf::RenderWindow& window) {
        background.draw(window);
        textLabel.draw(window);
    }
};

