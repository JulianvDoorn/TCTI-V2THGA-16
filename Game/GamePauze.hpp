#pragma once

#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "Button.hpp"

class GamePauze : public State {
	Statemachine& statemachine;

	Button gameResumeButton;
	Button gameRestartButton;
	Button gameExitButton;

	EventConnection<> gameResumeButtonPressedConn;
	EventConnection<> gameResumeButtonReleasedConn;
	EventConnection<> gameRestartButtonPressedConn;
	EventConnection<> gameRestartButtonReleasedConn;
	EventConnection<> gameExitButtonReleasedConn;
	EventConnection<> gameExitButtonPressedConn;

	EventConnection<> exitMouseEnterConn;
	EventConnection<> exitMouseLeaveConn;

	EventConnection<> resumeMouseEnterConn;
	EventConnection<> resumeMouseLeaveConn;

	EventConnection<> restartMouseEnterConn;
	EventConnection<> restartMouseLeaveConn;

	EventConnection<sf::Keyboard::Key> keyReleasedConnection;

public:
	GamePauze(Statemachine& statemachine) :
		statemachine(statemachine),
		gameResumeButton(),
		gameRestartButton(),
		gameExitButton()
	{
		gameResumeButton.setSize({ 300, 100 });
		gameResumeButton.setPosition({ 640, 360 });
		gameResumeButton.setCharSize(32);
		gameResumeButton.setBackgroundColor({ 0, 153, 51 });
		gameResumeButton.setText("Resume game");

		gameRestartButton.setSize({ 300, 100 });
		gameRestartButton.setPosition({ 640, 490 });
		gameRestartButton.setCharSize(32);
		gameRestartButton.setBackgroundColor({ 0, 153, 51 });
		gameRestartButton.setText("Restart game");

		gameExitButton.setSize({ 300, 100 });
		gameExitButton.setPosition({ 640, 620 });
		gameExitButton.setCharSize(32);
		gameExitButton.setBackgroundColor({ 0, 153, 51 });
		gameExitButton.setText("Exit game");


	}

	void entry() override {


        gameRestartButtonPressedConn = gameRestartButton.buttonPressed.connect([this](){
            gameRestartButton.setBackgroundColor({0,163,61});
        });

		gameRestartButtonReleasedConn = gameRestartButton.buttonReleased.connect([this]() {
			statemachine.resetState("running");
			statemachine.doTransition("running");
		});

        gameExitButtonPressedConn = gameExitButton.buttonPressed.connect([this]() {
			gameExitButton.setBackgroundColor({ 0, 163, 61 });
		});

		gameExitButtonReleasedConn = gameExitButton.buttonReleased.connect([this]() {
			statemachine.resetState("running");
			statemachine.doTransition("game-over");
		});

        gameResumeButtonReleasedConn = gameResumeButton.buttonReleased.connect([this]() {
            statemachine.doTransition("running");
        });

		gameResumeButtonPressedConn = gameResumeButton.buttonPressed.connect([this]() {
			gameResumeButton.setBackgroundColor({ 0, 163, 61 });
		});
		
		exitMouseEnterConn = gameExitButton.mouseEnter.connect([this]() {
			gameExitButton.setBackgroundColor({ 0, 123, 21 });
		});

		exitMouseLeaveConn = gameExitButton.mouseLeave.connect([this]() {
			gameExitButton.setBackgroundColor({ 0, 153, 51 });
		});

        restartMouseEnterConn = gameRestartButton.mouseEnter.connect([this](){
            gameRestartButton.setBackgroundColor({0,121,21});
        });
        restartMouseLeaveConn = gameRestartButton.mouseLeave.connect([this](){
            gameRestartButton.setBackgroundColor({0,153,51});
        });

		resumeMouseEnterConn = gameResumeButton.mouseEnter.connect([this]() {
			gameResumeButton.setBackgroundColor({ 0, 123, 21 });
		});

		resumeMouseLeaveConn = gameResumeButton.mouseLeave.connect([this]() {
			gameResumeButton.setBackgroundColor({ 0, 153, 51 });
		});

		keyReleasedConnection = game.keyboard.keyReleased.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::Escape) {
				statemachine.doTransition("running");
			}
		});
	}

	void exit() override {
		gameResumeButtonPressedConn.disconnect();
		gameResumeButtonReleasedConn.disconnect();
		gameRestartButtonPressedConn.disconnect();
		gameRestartButtonReleasedConn.disconnect();
		gameExitButtonPressedConn.disconnect();
		gameExitButtonReleasedConn.disconnect();
		exitMouseEnterConn.disconnect();
		exitMouseLeaveConn.disconnect();
		resumeMouseEnterConn.disconnect();
		resumeMouseLeaveConn.disconnect();
		restartMouseEnterConn.disconnect();
		restartMouseLeaveConn.disconnect();

		keyReleasedConnection.disconnect();
	}

	void update(const float elapsedTime) override {
		gameResumeButton.draw(statemachine.window);
		gameRestartButton.draw(statemachine.window);
		gameExitButton.draw(statemachine.window);
	}
};