#pragma once

#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "Button.hpp"

class SettingsMenu : public State {
    Statemachine& statemachine;

    Label menuLabel;
    Button exitButton;

    EventConnection<> exitButtonPressedConn;
    EventConnection<> exitButtonReleasedConn;

    EventConnection<> mouseEnterConn;
    EventConnection<> mouseLeaveConn;

public:
    SettingsMenu(Statemachine& statemachine) :
            statemachine(statemachine),
            menuLabel(),
            exitButton()
    {
        menuLabel.setPosition({ 500, 200 });
        menuLabel.setCharSize(45);
        menuLabel.setColor(sf::Color::White);
        menuLabel.setText("Settings Menu");
        menuLabel.setStyle(sf::Text::Bold);


        exitButton.setSize({350,100});
        exitButton.setPosition({640,620});
        exitButton.setCharSize(32);
        exitButton.setBackgroundColor({0,153,51});
        exitButton.setText("Return to main menu");
    }

    void entry() override {
        exitButtonPressedConn = exitButton.buttonPressed.connect([this](){
            exitButton.setBackgroundColor({0,163,61});
        });

        exitButtonReleasedConn = exitButton.buttonReleased.connect([this]() {
            statemachine.doTransition("main-menu");
        });

        mouseEnterConn = exitButton.mouseEnter.connect([this]() {
            exitButton.setBackgroundColor({ 0, 123, 21 });
        });
        mouseLeaveConn = exitButton.mouseLeave.connect([this]() {
            exitButton.setBackgroundColor({ 0, 153, 51 });
        });
    }

    void exit() override {
        exitButtonPressedConn.disconnect();
        exitButtonReleasedConn.disconnect();
        mouseEnterConn.disconnect();
        mouseLeaveConn.disconnect();
    }

    void update(const float elapsedTime) override {
        menuLabel.draw(statemachine.window);
        exitButton.draw(statemachine.window);
    }
};