#pragma  once

#include <SFML/Graphics.hpp>
#include "Rectangle.hpp"

class Button {
private:
    Rectangle background;
    sf::Vector2f backgroundSize;
    sf::Vector2f position;
    sf::Text displayText;
	sf::Font& font;
public:
    Button(sf::Vector2f backgroundSize, sf::Vector2f position, sf::Font& font):
    backgroundSize(backgroundSize), position(position), font(font) {
        background.setSize(backgroundSize);
        background.setPosition(position);
		background.setFillColor(sf::Color(0, 153, 51));
		displayText.setPosition(position);
		displayText.setFont(font);
		displayText.setFillColor(sf::Color::White);
		displayText.setCharacterSize(30);
		displayText.setStyle(sf::Text::Bold);
		displayText.setPosition(position);
    };

    void setText(std::string text){
        displayText.setString(text);
        sf::FloatRect textRectangle= displayText.getLocalBounds();
		displayText.setOrigin(textRectangle.width / 2, textRectangle.height / 2);

    }

	void setBackgroundColor(sf::Color color) {
		background.setFillColor(color);
	}

	void setTextColor(sf::Color color) {
		displayText.setFillColor(color);
	}

	void setPosition(sf::Vector2f position) {
		background.setPosition(position);
		displayText.setPosition(position);
	}

	void setCharacterSize(int size) {
		displayText.setCharacterSize(size);
	}

    void draw(sf::RenderWindow &window){
        background.draw(window);
        window.draw(displayText);
    }
};

