#pragma once

#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "Button.hpp"

class MainMenu : public State {
	Statemachine& statemachine;

    Label menuLabel;
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
	MainMenu(Statemachine& statemachine) :
		statemachine(statemachine),
		startButton(),
        settingsButton(),
        menuLabel(),
        exitButton()
	{
        menuLabel.setPosition({ 550, 200 });
        menuLabel.setCharSize(45);
        menuLabel.setColor(sf::Color::White);
        menuLabel.setText("Fimmy");
        menuLabel.setStyle(sf::Text::Bold);

		startButton.setSize({ 300, 100 });
		startButton.setPosition({ 640, 360 });
		startButton.setCharSize(32);
		startButton.setBackgroundColor({ 0, 153, 51 });
		startButton.setText("Start game");

        settingsButton.setSize({300,100});
        settingsButton.setPosition({640,490});
        settingsButton.setCharSize(32);
        settingsButton.setBackgroundColor({0,153,51});
        settingsButton.setText("Settings");

		exitButton.setSize({300,100});
		exitButton.setPosition({640,620});
		exitButton.setCharSize(32);
		exitButton.setBackgroundColor({0,153,51});
		exitButton.setText("Exit game");
	}

	void entry() override {
		startButtonPressedConn = startButton.buttonPressed.connect([this]() {
			startButton.setBackgroundColor({ 0, 163, 61 });
		});
        settingsButtonPressedConn = settingsButton.buttonPressed.connect([this](){
            settingsButton.setBackgroundColor({0,163,61});
        });
        exitButtonPressedConn = exitButton.buttonPressed.connect([this](){
            exitButton.setBackgroundColor({0,163,61});
        });

        startButtonReleasedConn = startButton.buttonReleased.connect([this]() {
            statemachine.resetState("running");
            statemachine.doTransition("running");
		});
        settingsButtonReleasedConn = settingsButton.buttonReleased.connect([this](){
            std::cout << "Settings Button Released Connection" << std::endl;
			statemachine.doTransition("settings-menu");

        });
        exitButtonReleasedConn = exitButton.buttonReleased.connect([this]() {
            statemachine.window.close();
        });

		mouseEnterConn = startButton.mouseEnter.connect([this]() {
			startButton.setBackgroundColor({ 0, 123, 21 });
		});

		mouseLeaveConn = startButton.mouseLeave.connect([this]() {
			startButton.setBackgroundColor({ 0, 153, 51 });
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
		startButtonPressedConn.disconnect();
		startButtonReleasedConn.disconnect();
        settingsButtonPressedConn.disconnect();
        settingsButtonReleasedConn.disconnect();
        exitButtonPressedConn.disconnect();
        exitButtonReleasedConn.disconnect();
		mouseEnterConn.disconnect();
		mouseLeaveConn.disconnect();
	}

	void update(const float elapsedTime) override {
        menuLabel.draw(statemachine.window);
		startButton.draw(statemachine.window);
        settingsButton.draw(statemachine.window);
		exitButton.draw(statemachine.window);
	}
};