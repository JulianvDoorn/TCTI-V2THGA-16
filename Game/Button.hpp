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
		displayText.setFont(font);
    };

    void setText(std::string text){
        displayText.setString(text);
        sf::FloatRect textRectangle= displayText.getLocalBounds();
        displayText.setOrigin(textRectangle.width / 2, textRectangle.height / 2);
        displayText.setFillColor(sf::Color::Red);
        displayText.setCharacterSize(30);
        displayText.setStyle(sf::Text::Bold);
    }

    void draw(sf::RenderWindow &window){
        background.draw(window);
        window.draw(displayText);
    }
};

