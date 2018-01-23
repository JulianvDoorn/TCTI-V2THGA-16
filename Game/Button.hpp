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

	void bindEvents() {
		game.mouse.mouseLeftButtonDown.connect([this](const sf::Vector2i mousePos) {
			if (background.contains(static_cast<sf::Vector2f>(mousePos))) {
				buttonPressed.fire();
			}
		});

		game.mouse.mouseLeftButtonUp.connect([this](const sf::Vector2i mousePos) {
			if (background.contains(static_cast<sf::Vector2f>(mousePos))) {
				buttonReleased.fire();
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

	Button() : textLabel() {
		bindEvents();
	}

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

	void setSize(sf::Vector2f size) {
		background.setSize(size);
	}

	void setCharSize(int32_t size) {
		textLabel.setCharSize(size);
	}

	void setFont(sf::Font& font) {
		textLabel.setFont(font);
	}

    void setText(const std::string& text){
        textLabel.setText(text);
        sf::FloatRect textRectangle= textLabel.getBounds();
		textLabel.setOrigin(textRectangle.width / 2, textRectangle.height / 2);
    }

	void setBackgroundColor(sf::Color color) {
		background.setFillColor(color);
	}

	void setTextColor(sf::Color color) {
		textLabel.setColor(color);
	}

	void setPosition(sf::Vector2f position) {
		background.setPosition(position);
		textLabel.setPosition(position);
	}

	sf::Vector2f getSize() const {
		return background.getSize();
	}

	sf::Vector2f getPosition() const {
		return background.getSize();
	}

    void draw(sf::RenderWindow& window) {
        background.draw(window);
        textLabel.draw(window);
    }
};

