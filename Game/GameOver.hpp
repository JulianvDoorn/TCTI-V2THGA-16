#pragma once

#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "Button.hpp"
#include <SFML/Audio.hpp>

class GameOver : public State {
	Statemachine& statemachine;

	Button gameOverButton;

	sf::Music gameOver;


	EventConnection<> gameOverButtonPressedConn;
	EventConnection<> gameOverButtonReleasedConn;
	EventConnection<> mouseEnterConn;
	EventConnection<> mouseLeaveConn;

public:
	GameOver(Statemachine& statemachine) :
		State("game-over"),
		statemachine(statemachine),
		gameOverButton()
	{
		gameOverButton.setSize({ 300, 100 });
		gameOverButton.setPosition({ 640, 360 });
		gameOverButton.setCharSize(32);
		gameOverButton.setBackgroundColor({ 0, 153, 51 });
		gameOverButton.setText("Game over");

		statemachine.addState(*this);
	}

	void entry() override {
		gameOver.openFromFile("GameOver.wav");
		gameOver.play();
		

	}

	void exit() override {

	}

	void update(const float elapsedTime) override {
		gameOverButton.draw(statemachine.window);
	}
};