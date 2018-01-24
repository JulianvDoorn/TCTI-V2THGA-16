#pragma once

#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "Button.hpp"
#include <SFML/Audio.hpp>

class GameOver : public State {
	Statemachine& statemachine;

    Label menuLabel;
	Button mainMenuButton;
    Button restartGameButton;
    Button exitButton;

	sf::Music gameOver;

	EventConnection<> mainMenuButtonPressedConn;
    EventConnection<> mainMenuButtonReleasedConn;
    EventConnection<> restartGameButtonPressedConn;
    EventConnection<> restartGameButtonReleasedConn;
    EventConnection<> exitButtonPressedConn;
    EventConnection<> exitButtonReleasedConn;
	EventConnection<> mouseEnterConn;
	EventConnection<> mouseLeaveConn;

public:
	GameOver(Statemachine& statemachine) :
		statemachine(statemachine),
		mainMenuButton(),
        menuLabel()
	{
		menuLabel.setPosition({ 540, 185 });

		menuLabel.setCharSize(32);
        menuLabel.setColor(sf::Color::White);
		menuLabel.setText("Game over");
        menuLabel.setStyle(sf::Text::Bold);

		mainMenuButton.setSize({ 300, 100 });
		mainMenuButton.setPosition({ 840, 360 });
		mainMenuButton.setCharSize(32);
		mainMenuButton.setBackgroundColor({ 0, 153, 51 });
		mainMenuButton.setText("Main Menu");

        restartGameButton.setSize({ 300, 100 });
        restartGameButton.setPosition({ 640, 490 });
        restartGameButton.setCharSize(32);
        restartGameButton.setBackgroundColor({ 0, 153, 51 });
        restartGameButton.setText("Restart Game");

        exitButton.setSize({300,100});
        exitButton.setPosition({640,620});
        exitButton.setCharSize(32);
        exitButton.setBackgroundColor({0,153,51});
        exitButton.setText("Exit game");
	}

	void entry() override {
		mainMenuButtonPressedConn = mainMenuButton.buttonPressed.connect([this](){
			mainMenuButton.setBackgroundColor({0,163,61});
		});
        restartGameButtonPressedConn =restartGameButton.buttonPressed.connect([this](){
            restartGameButton.setBackgroundColor({0,163,61});
        });
        exitButtonPressedConn = exitButton.buttonPressed.connect([this](){
            exitButton.setBackgroundColor({0,163,61});
        });

        mainMenuButtonReleasedConn = mainMenuButton.buttonReleased.connect([this]() {
			//statemachine.resetState("main-menu");
            statemachine.doTransition("main-menu");
		});
        restartGameButtonReleasedConn = restartGameButton.buttonReleased.connect([this]() {
            statemachine.resetState("running");
            statemachine.doTransition("running");
        });
        exitButtonReleasedConn = exitButton.buttonReleased.connect([this]() {
            statemachine.window.close();
        }); 

        mouseEnterConn = mainMenuButton.mouseEnter.connect([this]() {
            mainMenuButton.setBackgroundColor({ 0, 123, 21 });
        });
        mouseEnterConn = restartGameButton.mouseEnter.connect([this](){
            restartGameButton.setBackgroundColor({0,123,21});
        });
        mouseEnterConn = exitButton.mouseEnter.connect([this]() {
            exitButton.setBackgroundColor({ 0, 123, 21 });
        });

        mouseLeaveConn = mainMenuButton.mouseLeave.connect([this]() {
            mainMenuButton.setBackgroundColor({ 0, 153, 51 });
        });
        mouseLeaveConn=  restartGameButton.mouseLeave.connect([this](){
            restartGameButton.setBackgroundColor({0,153,21});
        });
        mouseLeaveConn = exitButton.mouseLeave.connect([this]() {
            exitButton.setBackgroundColor({ 0, 153, 51 });
        });

		AssetManager::instance()->getSound("game-over").play();
	}

	void exit() override {
        mainMenuButtonPressedConn.disconnect();
        mainMenuButtonReleasedConn.disconnect();
        restartGameButtonPressedConn.disconnect();
        restartGameButtonReleasedConn.disconnect();
        exitButtonPressedConn.disconnect();
        exitButtonReleasedConn.disconnect();
		mouseEnterConn.disconnect();
		mouseLeaveConn.disconnect();
	}

	void update(const float elapsedTime) override {
		menuLabel.draw(statemachine.window);
        mainMenuButton.draw(statemachine.window);
        restartGameButton.draw(statemachine.window);
        exitButton.draw(statemachine.window);
	}
};