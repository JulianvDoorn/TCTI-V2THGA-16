#pragma once

#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "Button.hpp"

class GamePauze : public State {
	Statemachine& statemachine;

	Button gameExitButton;
	Button gameResumeButton;

	EventConnection<> gameExitButtonPressedConn;
	EventConnection<> gameExitButtonReleasedConn;
	EventConnection<> gameResumeButtonPressedConn;
	EventConnection<> gameResumeButtonReleasedConn;

	EventConnection<> exitMouseEnterConn;
	EventConnection<> exitMouseLeaveConn;

	EventConnection<> resumeMouseEnterConn;
	EventConnection<> resumeMouseLeaveConn;
public:
	GamePauze(Statemachine& statemachine) :
		statemachine(statemachine),
		gameExitButton(),
		gameResumeButton()
	{
		gameExitButton.setSize({ 300, 100 });
		gameExitButton.setPosition({ 640, 435 });
		gameExitButton.setCharSize(32);
		gameExitButton.setBackgroundColor({ 0, 153, 51 });
		gameExitButton.setText("Exit game");

		gameResumeButton.setSize({ 300, 100 });
		gameResumeButton.setPosition({ 640, 285 });
		gameResumeButton.setCharSize(32);
		gameResumeButton.setBackgroundColor({ 0, 153, 51 });
		gameResumeButton.setText("Resume game");
	}

	void entry() override {
		gameExitButtonPressedConn = gameExitButton.buttonPressed.connect([this]() {
			gameExitButton.setBackgroundColor({ 0, 163, 61 });
		});

		gameResumeButtonPressedConn = gameResumeButton.buttonPressed.connect([this]() {
			gameResumeButton.setBackgroundColor({ 0, 163, 61 });
		});

		gameExitButtonReleasedConn = gameExitButton.buttonReleased.connect([this]() {
			statemachine.doTransition("game-over");
		});

		gameResumeButtonReleasedConn = gameResumeButton.buttonReleased.connect([this]() {
			statemachine.doTransition("running");
		});
		
		exitMouseEnterConn = gameExitButton.mouseEnter.connect([this]() {
			gameExitButton.setBackgroundColor({ 0, 123, 21 });
		});

		exitMouseLeaveConn = gameExitButton.mouseLeave.connect([this]() {
			gameExitButton.setBackgroundColor({ 0, 153, 51 });
		});

		resumeMouseEnterConn = gameResumeButton.mouseEnter.connect([this]() {
			gameResumeButton.setBackgroundColor({ 0, 123, 21 });
		});

		resumeMouseLeaveConn = gameResumeButton.mouseLeave.connect([this]() {
			gameResumeButton.setBackgroundColor({ 0, 153, 51 });
		});
	}

	void exit() override {
		gameExitButtonPressedConn.disconnect();
		gameExitButtonReleasedConn.disconnect();
		gameResumeButtonPressedConn.disconnect();
		gameResumeButtonReleasedConn.disconnect();
		exitMouseEnterConn.disconnect();
		exitMouseLeaveConn.disconnect();
	}

	void update(const float elapsedTime) override {
		gameExitButton.draw(statemachine.window);
		gameResumeButton.draw(statemachine.window);
	}
};