#pragma once

#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "Button.hpp"

/**
 * @class	GamePauze
 *
 * @brief	A gamePauze screen with resume, restart and exit button. Using the event system to bind actions to the buttons.
 *
 * @author	Jeffrey de Waal
 * @date	1/25/2018
 */

class GamePauze : public State {


	/** @brief	The statemachine */
	Statemachine& statemachine;

	/** @brief	The game resume button */
	Button gameResumeButton;
	/** @brief	The game restart button */
	Button gameRestartButton;
	/** @brief	The game exit button */
	Button gameExitButton;

	/** @brief	The game resume button pressed connection */
	EventConnection gameResumeButtonPressedConn;
	/** @brief	The game resume button released connection */
	EventConnection gameResumeButtonReleasedConn;
	/** @brief	The game restart button pressed connection */
	EventConnection gameRestartButtonPressedConn;
	/** @brief	The game restart button released connection */
	EventConnection gameRestartButtonReleasedConn;
	/** @brief	The game exit button released connection */
	EventConnection gameExitButtonReleasedConn;
	/** @brief	The game exit button pressed connection */
	EventConnection gameExitButtonPressedConn;
	/** @brief	The exit mouse enter connection */
	EventConnection exitMouseEnterConn;
	/** @brief	The exit mouse leave connection */
	EventConnection exitMouseLeaveConn;
	/** @brief	The resume mouse enter connection */
	EventConnection resumeMouseEnterConn;
	/** @brief	The resume mouse leave connection */
	EventConnection resumeMouseLeaveConn;
	/** @brief	The restart mouse enter connection */
	EventConnection restartMouseEnterConn;
	/** @brief	The restart mouse leave connection */
	EventConnection restartMouseLeaveConn;

	/** @brief	The key released connection */
	EventConnection keyReleasedConnection;

public:

	/**
	 * @fn	GamePauze::GamePauze(Statemachine& statemachine)
	 *
	 * @brief	Gamepauze state constructor, This constructor sets-up all the buttons.
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/25/2018
	 *
	 * @param [in,out]	statemachine	Give's a reference to the statemachine build in the main.cpp file.
	 */

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

	/**
	 * @fn	void GamePauze::entry() override
	 *
	 * @brief	On entry sets-up, connects all the eventconnections to the correct event and on event sets action.
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/25/2018
	 */
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

	/**
	 * @fn	void GamePauze::exit() override
	 *
	 * @brief	Disconnects all the eventconnections used in this state.
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/25/2018
	 */

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

	/**
	 * @fn	void GamePauze::update(const float elapsedTime) override
	 *
	 * @brief	Updates the buttons.
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/25/2018
	 *
	 * @param	elapsedTime	The elapsed time.
	 */

	void update(const float elapsedTime) override {
		gameResumeButton.draw(statemachine.window);
		gameRestartButton.draw(statemachine.window);
		gameExitButton.draw(statemachine.window);
	}
};