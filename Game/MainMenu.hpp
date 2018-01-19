#pragma once

#include "GameState.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "Button.hpp"

class MainMenu : public GameState {
	Statemachine& statemachine;

	Button startButton;

	EventConnection<> startButtonPressedConn;
	EventConnection<> startButtonReleasedConn;
	EventConnection<> mouseEnterConn;
	EventConnection<> mouseLeaveConn;

public:
	MainMenu(Statemachine& statemachine) :
		GameState("main-menu"),
		statemachine(statemachine),
		startButton(statemachine.mouse)
	{
		startButton.setSize({ 300, 100 });
		startButton.setPosition({ 640, 360 });
		startButton.setCharSize(32);
		startButton.setBackgroundColor({ 0, 153, 51 });
		startButton.setText("Start game");

		statemachine.addState(*this);
	}

	void entry() override {
		startButtonPressedConn = startButton.buttonPressed.connect([this]() {
			startButton.setBackgroundColor({ 0, 163, 61 });
		});

		startButtonReleasedConn = startButton.buttonReleased.connect([this]() {
			statemachine.doTransition("running");
		});

		mouseEnterConn = startButton.mouseEnter.connect([this]() {
			startButton.setBackgroundColor({ 0, 123, 21 });
		});

		mouseLeaveConn = startButton.mouseLeave.connect([this]() {
			startButton.setBackgroundColor({ 0, 153, 51 });
		});
	}

	void exit() override {
		startButtonPressedConn.disconnect();
		startButtonReleasedConn.disconnect();
		mouseEnterConn.disconnect();
		mouseLeaveConn.disconnect();
	}

	void update(const float elapsedTime) override {
		startButton.draw(statemachine.window);
	}
};