#pragma once

#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "Button.hpp"
#include <SFML/Audio.hpp>

/**
 * @class	GameOver
 *
 * @brief	The Gameover screen with restart, exit and mainmenu button. Using the event system to bind actions to the buttons.
 *
 * @author	Jeffrey de Waal
 * @date	1/25/2018
 */

class GameOver : public State {

	/** @brief	The statemachine */
	Statemachine& statemachine;

    /** @brief	The menu label */
    Label menuLabel;
	/** @brief	The main menu button */
	Button mainMenuButton;
    /** @brief	The restart game button */
    Button restartGameButton;
    /** @brief	The exit button */
    Button exitButton;

	/** @brief	The gameover sound */
	sf::Music gameOver;

	/** @brief	The main menu button pressed connection */
	EventConnection mainMenuButtonPressedConn;
    /** @brief	The main menu button released connection */
    EventConnection mainMenuButtonReleasedConn;
    /** @brief	The restart game button pressed connection */
    EventConnection restartGameButtonPressedConn;
    /** @brief	The restart game button released connection */
    EventConnection restartGameButtonReleasedConn;
    /** @brief	The exit button pressed connection */
    EventConnection exitButtonPressedConn;
    /** @brief	The exit button released connection */
    EventConnection exitButtonReleasedConn;
	/** @brief	The mouse enter connection */
	EventConnection mouseEnterConn;
	/** @brief	The mouse leave connection */
	EventConnection mouseLeaveConn;

public:

	/**
	 * @fn	GameOver::GameOver(Statemachine& statemachine)
	 *
	 * @brief	Gameover state constructor, This constructor sets-up all the buttons and the gameover text.
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/25/2018
	 *
	 * @param [in,out]	statemachine	Give's a reference to the statemachine build in the main.cpp file.
	 */

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

	/**
	 * @fn	void GameOver::entry() override
	 *
	 * @brief	On entry sets-up, connects all the eventconnections to the correct event and on event sets action.
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/25/2018
	 */

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
            restartGameButton.setBackgroundColor({0,153,51});
        });
        mouseLeaveConn = exitButton.mouseLeave.connect([this]() {
            exitButton.setBackgroundColor({ 0, 153, 51 });
        });

		AssetManager::instance()->getSound("game-over").play();
	}

	/**
	 * @fn	void GameOver::exit() override
	 *
	 * @brief	Disconnects all the eventconnections used in this state.
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/25/2018
	 */

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

	/**
	 * @fn	void GameOver::update(const float elapsedTime) override
	 *
	 * @brief	Updates the buttons
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/25/2018
	 *
	 * @param	elapsedTime	The elapsed time.
	 */

	void update(const float elapsedTime) override {
		menuLabel.draw(statemachine.window);
        mainMenuButton.draw(statemachine.window);
        restartGameButton.draw(statemachine.window);
        exitButton.draw(statemachine.window);
	}
};