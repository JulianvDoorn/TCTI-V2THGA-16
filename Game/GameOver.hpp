#pragma once

#include "GameState.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "Button.hpp"

class GameOver : public GameState {
	Statemachine& statemachine;

	Button gameOverButton;

	EventConnection<> gameOverButtonPressedConn;
	EventConnection<> gameOverButtonReleasedConn;
	EventConnection<> mouseEnterConn;
	EventConnection<> mouseLeaveConn;

public:
	GameOver(Statemachine& statemachine) :
		GameState("game-over"),
		statemachine(statemachine),
		gameOverButton(statemachine.mouse)
	{
		gameOverButton.setSize({ 300, 100 });
		gameOverButton.setPosition({ 640, 360 });
		gameOverButton.setCharSize(32);
		gameOverButton.setBackgroundColor({ 0, 153, 51 });
		gameOverButton.setText("Game over");

		statemachine.addState(*this);
	}

	void entry() override {

	}

	void exit() override {

	}

	void update(const float elapsedTime) override {
		gameOverButton.draw(statemachine.window);
	}
};