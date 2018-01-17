#pragma  once

#include <SFML/Graphics.hpp>
#include "Rectangle.hpp"

class Button {
private:
    Rectangle background;
    sf::Vector2f backgroundSize;
    sf::Vector2f position;
    sf::Text displayText;
public:
    Button(sf::Vector2f backgroundSize, sf::Vector2f position):
    backgroundSize(backgroundSize), position(position){
        background.setSize(backgroundSize);
        background.setPosition(position);
    };

    void setText(std::string text){
        sf::Font font;
        font.loadFromFile("arial.ttf");
        displayText.setFont(font);

        displayText.setString(text);
        sf::FloatRect textRectangle= displayText.getLocalBounds();
        displayText.setOrigin(textRectangle.width/2, textRectangle.height/2);
        displayText.setColor(sf::Color::Red);
        displayText.setCharacterSize(30);
        displayText.setStyle(sf::Text::Bold);
    }

    void draw(sf::RenderWindow &window){
        background.draw(window);
        window.draw(displayText);
    }
};

