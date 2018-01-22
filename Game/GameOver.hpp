#pragma once

#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "Button.hpp"
#include <SFML/Audio.hpp>

class GameOver : public State {
	Statemachine& statemachine;

    Label menuLabel;
	Button gameOverButton;
	Button mainMenuButton;
    Button exitButton;

	sf::Music gameOver;

	EventConnection<> gameOverButtonPressedConn;
	EventConnection<> gameOverButtonReleasedConn;
	EventConnection<> mainMenuButtonPressedConn;
	EventConnection<> mainMenuButtonReleasedConn;
    EventConnection<> exitButtonPressedConn;
    EventConnection<> exitButtonReleasedConn;
	EventConnection<> mouseEnterConn;
	EventConnection<> mouseLeaveConn;

public:
	GameOver(Statemachine& statemachine) :
		State("game-over"),
		statemachine(statemachine),
		mainMenuButton(),
        menuLabel(Button::getDefaultFont())
	{
		menuLabel.setPosition({ 640, 360 });
		menuLabel.setCharSize(32);
        menuLabel.setColor(sf::Color::White);
		menuLabel.setText("Game over");
        menuLabel.setStyle(sf::Text::Bold);

		mainMenuButton.setSize({ 300, 100 });
		mainMenuButton.setPosition({ 640, 490 });
		mainMenuButton.setCharSize(32);
		mainMenuButton.setBackgroundColor({ 0, 153, 51 });
		mainMenuButton.setText("Main Menu");

        exitButton.setSize({300,100});
        exitButton.setPosition({640,620});
        exitButton.setCharSize(32);
        exitButton.setBackgroundColor({0,153,51});
        exitButton.setText("Exit game");

        statemachine.addState(*this);
	}

	void entry() override {
        std::cout << "Game-over state" << std::endl;
		mainMenuButtonPressedConn = mainMenuButton.buttonPressed.connect([this](){
			mainMenuButton.setBackgroundColor({0,163,61});
		});
        exitButtonPressedConn = exitButton.buttonPressed.connect([this](){
            exitButton.setBackgroundColor({0,163,61});
        });

        mainMenuButtonReleasedConn = mainMenuButton.buttonReleased.connect([this]() {
			statemachine.doTransition("main-menu");
		});
        exitButtonReleasedConn = exitButton.buttonReleased.connect([this]() {
            statemachine.window.close();
        });

		gameOver.openFromFile("Gameover.wav");
		gameOver.play();

        mouseEnterConn = mainMenuButton.mouseEnter.connect([this]() {
            mainMenuButton.setBackgroundColor({ 0, 123, 21 });
        });
        mouseEnterConn = exitButton.mouseEnter.connect([this]() {
            exitButton.setBackgroundColor({ 0, 123, 21 });
        });

        mouseLeaveConn = mainMenuButton.mouseLeave.connect([this]() {
            mainMenuButton.setBackgroundColor({ 0, 153, 51 });
        });
        mouseLeaveConn = exitButton.mouseLeave.connect([this]() {
            exitButton.setBackgroundColor({ 0, 153, 51 });
        });
	}

	void exit() override {
        mainMenuButtonPressedConn.disconnect();
        mainMenuButtonReleasedConn.disconnect();
        exitButtonPressedConn.disconnect();
        exitButtonReleasedConn.disconnect();
	}

	void update(const float elapsedTime) override {
		menuLabel.draw(statemachine.window);
        mainMenuButton.draw(statemachine.window);
        exitButton.draw(statemachine.window);
	}
};