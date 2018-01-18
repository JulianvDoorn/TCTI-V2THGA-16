#pragma  once

#include <SFML/Graphics.hpp>
#include "Rectangle.hpp"
#include "Label.hpp"
#include "EventSource.hpp"
#include "EventConnection.hpp"

class Button {
private:
    Rectangle background;
    sf::Vector2f backgroundSize;
    sf::Vector2f position;
    Label textLabel;
public:
	EventSource<> buttonPressed;
	EventSource<> buttonReleased;

    Button(sf::Vector2f backgroundSize, sf::Vector2f position, int size, std::string text, sf::Font &font):
    backgroundSize(backgroundSize), position(position),textLabel(text,font,position,size) {
        background.setSize(backgroundSize);
        background.setPosition(position);
		background.setFillColor(sf::Color(0, 153, 51));
        sf::FloatRect textRectangle= textLabel.getBounds();
        textLabel.setOrigin(textRectangle.width / 2, textRectangle.height / 2);
    };

    void setText(std::string text){
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

	void setCharacterSize(int size) {
		textLabel.setCharSize(size);
	}

    void draw(sf::RenderWindow &window){
        background.draw(window);
        textLabel.draw(window);
    }
};

