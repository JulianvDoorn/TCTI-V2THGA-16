#pragma once

#include "Rectangle.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
class Bodypart : public Rectangle {
private:
    bool exsists = true;
    bool mustDraw = true;
    bool rollable = false;
    int rollDirection = 0;
    int animationCyle;
    bool annimations = false;
    int animationTimeInMiliseconds = 50;
    int visibledirection = 0;
    sf::Texture standingTexture;
    sf::Texture runLeftTexture;
    sf::Texture runRightTexture;
    std::array<sf::Texture,3> runAnnimation;
    sf::Texture currentTexture;
    sf::Texture rollRightTexture;
    sf::Texture rollLeftTexture;
    sf::Texture empty;
public:
    Bodypart(){
        empty.create(20,40);
    };
    void draw(sf::RenderTarget & window){
        if (mustDraw && exsists) {
            if (rollable) {
                if (rollDirection > 0) {
                    setTexture(rollRightTexture);
                } else if (rollDirection < 0) {
                    setTexture(rollLeftTexture);
                }
            }
            else{
                setTexture(currentTexture);
            }
            Drawable::draw(window);
        }
    }
    void enableAnnimations(){
        annimations = true;
    }
    void disableAnnimations(){
        annimations = false;
    }
    void enableDraw(){
        mustDraw = true;
    }
    void disalbeDraw(){
        mustDraw = false;
    }

    void remove(){
        exsists = false;
    }
    void show(){
        exsists = true;
    }

    void setStandingTexture(const sf::Texture& texture){
        standingTexture = texture;
    }
    void useStandingTexture(){
        currentTexture = standingTexture;
    }
    void setRunLeftTexture(const sf::Texture& texture){
        runLeftTexture = texture;
    }
    void setRunLeftTexture(){
        runLeftTexture = empty;
    }
    void useRunLeftTexture(){
        currentTexture = runLeftTexture;
    }
    void setRunRightTexture(const sf::Texture& texture){
        runRightTexture = texture;
    }
    void setRunRightTexture(){
        runRightTexture = empty;
    }
    void useRunRightTexture(){
            currentTexture = runRightTexture;
    }
    void setRollable(){
        rollable = true;
    }
    void roll(int direction){
        rollDirection =direction;
        setSize({20,20});
        sf::Vector2f offset ={40,0};
        setPosition(getPosition()+ offset);
    }
    void unRoll(){
        setSize({20,40});
        sf::Vector2f offset ={40,0};
        setPosition(getPosition()- offset);
    }

    void setRollLeftTexture(const sf::Texture & texture){
        rollLeftTexture = texture;
    }
    void setRollRightTexture(const sf::Texture & texture){
        rollRightTexture = texture;
    }

    void setRunAnnimationTextures(const std::array<sf::Texture,3>&Textures){
        runAnnimation = Textures;
        enableAnnimations();
    }
    void setVisibleDirections(int direction){
        visibledirection = direction;
    }
    void updateAnimation(sf::Clock & animationClock, int direction){
        if (annimations && visibledirection == direction ) {
            if (animationClock.getElapsedTime().asMilliseconds() > animationTimeInMiliseconds) {
                switch (animationCyle) {
                    case 1:
                        currentTexture = runAnnimation[0];
                        break;
                    case 2:
                        currentTexture = runAnnimation[1];
                        break;
                    case 3:
                        currentTexture = runAnnimation[2];
                        animationCyle = 0;
                        break;
                    default:
                        animationCyle = 0;
                        break;
                }
                animationClock.restart();
                animationCyle++;
            }
        }
    }
};

