#pragma once

#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "Button.hpp"

class SettingsMenu : public State {
    Statemachine& statemachine;

    Button startButton;
    Button exitButton;
    Button settingsButton;

    EventConnection<> startButtonPressedConn;
    EventConnection<> startButtonReleasedConn;
    EventConnection<> exitButtonPressedConn;
    EventConnection<> exitButtonReleasedConn;
    EventConnection<> settingsButtonPressedConn;
    EventConnection<> settingsButtonReleasedConn;
    EventConnection<> mouseEnterConn;
    EventConnection<> mouseLeaveConn;

public:
    SettingsMenu(Statemachine& statemachine) :
            statemachine(statemachine),
            startButton(),
            settingsButton(),
            exitButton()
    {
        startButton.setSize({ 300, 100 });
        startButton.setPosition({ 640, 360 });
        startButton.setCharSize(32);
        startButton.setBackgroundColor({ 0, 0,0 });
        startButton.setText("Settings Menu ");

        settingsButton.setSize({300,100});
        settingsButton.setPosition({640,490});
        settingsButton.setCharSize(32);
        settingsButton.setBackgroundColor({0,153,51});
        settingsButton.setText("Settings");

        exitButton.setSize({350,100});
        exitButton.setPosition({640,620});
        exitButton.setCharSize(32);
        exitButton.setBackgroundColor({0,153,51});
        exitButton.setText("Return to main menu");
    }

    void entry() override {
        std::cout << "Settings-menu state" << std::endl;
        settingsButtonPressedConn = settingsButton.buttonPressed.connect([this](){
            settingsButton.setBackgroundColor({0,163,61});
        });
        exitButtonPressedConn = exitButton.buttonPressed.connect([this](){
            exitButton.setBackgroundColor({0,163,61});
        });

        exitButtonReleasedConn = exitButton.buttonReleased.connect([this]() {
            statemachine.doTransition("main-menu");
        });
        settingsButtonReleasedConn = settingsButton.buttonReleased.connect([this](){
            statemachine.doTransition("settings-menu");
        });

        mouseEnterConn = settingsButton.mouseEnter.connect([this]() {
            settingsButton.setBackgroundColor({ 0, 123, 21 });
        });

        mouseLeaveConn = settingsButton.mouseLeave.connect([this]() {
            settingsButton.setBackgroundColor({ 0, 153, 51 });
        });
        mouseEnterConn = exitButton.mouseEnter.connect([this]() {
            exitButton.setBackgroundColor({ 0, 123, 21 });
        });

        mouseLeaveConn = exitButton.mouseLeave.connect([this]() {
            exitButton.setBackgroundColor({ 0, 153, 51 });
        });
    }

    void exit() override {
        settingsButtonPressedConn.disconnect();
        settingsButtonReleasedConn.disconnect();
        exitButtonPressedConn.disconnect();
        exitButtonReleasedConn.disconnect();
        mouseEnterConn.disconnect();
        mouseLeaveConn.disconnect();
    }

    void update(const float elapsedTime) override {
        startButton.draw(statemachine.window);
        settingsButton.draw(statemachine.window);
        exitButton.draw(statemachine.window);
    }
};