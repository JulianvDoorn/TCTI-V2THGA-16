#pragma once

#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "Button.hpp"

class GameOver : public State {
	Statemachine& statemachine;

	Button startButton;

	EventConnection<> startButtonPressedConn;
	EventConnection<> startButtonReleasedConn;
	EventConnection<> mouseEnterConn;
	EventConnection<> mouseLeaveConn;

public:
	GameOver(Statemachine& statemachine) :
		State("game-over"),
		statemachine(statemachine),
		startButton()
	{
		startButton.setSize({ 300, 100 });
		startButton.setPosition({ 640, 360 });
		startButton.setCharSize(32);
		startButton.setBackgroundColor({ 0, 153, 51 });
		startButton.setText("Game over");

		statemachine.addState(*this);
	}

	void entry() override {

	}

	void exit() override {

	}

	void update(const float elapsedTime) override {
		startButton.draw(statemachine.window);
	}
};