#pragma  once

#include <SFML/Graphics.hpp>

class Label {
private:
    sf::Font &font;
    sf::Text displayText;
public:
    Label(std::string &text, sf::Font &font,sf::Vector2f position, int size, sf::Color color = sf::Color::White, sf::Text::Style style = sf::Text::Bold):font(font){
        displayText.setPosition(position);
        displayText.setFont(font);
        displayText.setFillColor(color);
        displayText.setCharacterSize(size);
        displayText.setStyle(style);
        displayText.setPosition(position);
        displayText.setString(text);
    };
    void setText(std::string &text){
        displayText.setString(text);
    }

    void setColor(sf::Color color){
        displayText.setFillColor(color);
    }

    void setPosition(sf::Vector2f &position){
        displayText.setPosition(position);
    }

    void setCharSize(int size){
        displayText.setCharacterSize(size);
    }

    void setOrigin(sf::Vector2f position){
        displayText.setOrigin(position);
    }

    void setOrigin(float x, float y){
        displayText.setOrigin({x,y});
    }
    void draw(sf::RenderWindow &window){
        window.draw(displayText);
    }
    sf::FloatRect getBounds(){
        return displayText.getLocalBounds();
    }

};
