#pragma once

#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "Button.hpp"

/**
 * @class	MainMenu
 *
 * @brief	A Mainmenu screen with start, setting and exit button. Using the eventsystem to bind actions to the buttons.
 *
 * @author	Jeffrey de Waal
 * @date	1/25/2018
 */

class MainMenu : public State {
	/** @brief	The statemachine */
	Statemachine& statemachine;

    /** @brief	The menu label */
    Label menuLabel;
	/** @brief	The start button */
	Button startButton;
	/** @brief	The exit button */
	Button exitButton;
    /** @brief	The settings button */
    Button settingsButton;

	Button mapEditorButton;

	/** @brief	The start button pressed connection */
	EventConnection startButtonPressedConn;
	/** @brief	The start button released connection */
	EventConnection startButtonReleasedConn;
    /** @brief	The exit button pressed connection */
    EventConnection exitButtonPressedConn;
    /** @brief	The exit button released connection */
    EventConnection exitButtonReleasedConn;
    /** @brief	The settings button pressed connection */
    EventConnection settingsButtonPressedConn;
    /** @brief	The settings button released connection */
    EventConnection settingsButtonReleasedConn;
	/** @brief	The mouse enter connection */
	EventConnection mouseEnterConn;
	/** @brief	The mouse leave connection */
	EventConnection mouseLeaveConn;

	EventConnection mapEditorButtonReleasedConn;

public:

	/**
	 * @fn	MainMenu::MainMenu(Statemachine& statemachine)
	 *
	 * @brief	Mainmenu state constructor, This constructor sets-up all the buttons and the fimmy text above the buttons.
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/25/2018
	 *
	 * @param [in,out]	statemachine	Give's a reference to the statemachine build in the main.cpp file.
	 */

	MainMenu(Statemachine& statemachine) :
		statemachine(statemachine),
		startButton(),
		mapEditorButton(),
        settingsButton(),
        menuLabel(),
        exitButton()
	{
        menuLabel.setPosition({ 550, 200 });
        menuLabel.setCharSize(45);
        menuLabel.setColor(sf::Color::White);
        menuLabel.setText("Fimmy");
        menuLabel.setStyle(sf::Text::Bold);

		startButton.setSize({ 300, 100 });
		startButton.setPosition({ 0, 0 });
		startButton.setCharSize(32);
		startButton.setBackgroundColor({ 0, 153, 51 });
		startButton.setText("Start game");

		mapEditorButton.setSize({ 300, 100 });
		mapEditorButton.setPosition({ 960, 360 });
		mapEditorButton.setCharSize(32);
		mapEditorButton.setBackgroundColor({ 0, 153, 51 });
		mapEditorButton.setText("Map editor");

        settingsButton.setSize({300,100});
        settingsButton.setPosition({640,490});
        settingsButton.setCharSize(32);
        settingsButton.setBackgroundColor({0,153,51});
        settingsButton.setText("Settings");

		exitButton.setSize({300,100});
		exitButton.setPosition({640,620});
		exitButton.setCharSize(32);
		exitButton.setBackgroundColor({0,153,51});
		exitButton.setText("Exit game");
	}

	/**
	 * @fn	void MainMenu::entry() override
	 *
	 * @brief	On entry sets-up, connects all the eventconnections to the correct event and on event sets action.
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/25/2018
	 */
	void entry() override {
		startButtonPressedConn = startButton.buttonPressed.connect([this]() {
			startButton.setBackgroundColor({ 0, 163, 61 });
		});
        settingsButtonPressedConn = settingsButton.buttonPressed.connect([this](){
            settingsButton.setBackgroundColor({0,163,61});
        });
        exitButtonPressedConn = exitButton.buttonPressed.connect([this](){
            exitButton.setBackgroundColor({0,163,61});
        });

		startButtonReleasedConn = startButton.buttonReleased.connect([this]() {
			statemachine.resetState("running");
			statemachine.doTransition("running");
		});

		mapEditorButtonReleasedConn = mapEditorButton.buttonReleased.connect([this]() {
			statemachine.resetState("map-editor");
			statemachine.doTransition("map-editor");
		});


        settingsButtonReleasedConn = settingsButton.buttonReleased.connect([this](){
			statemachine.doTransition("settings-menu");

        });
        exitButtonReleasedConn = exitButton.buttonReleased.connect([this]() {
            statemachine.window.close();
        });

		mouseEnterConn = startButton.mouseEnter.connect([this]() {
            startButton.setBackgroundColor({ 0, 123, 21 });
        });
		mouseEnterConn = settingsButton.mouseEnter.connect([this]() {
			settingsButton.setBackgroundColor({ 0, 123, 21 });
		});
		mouseEnterConn = exitButton.mouseEnter.connect([this]() {
			exitButton.setBackgroundColor({ 0, 123, 21 });
		});
        mouseLeaveConn = startButton.mouseLeave.connect([this]() {
            startButton.setBackgroundColor({ 0, 153, 51 });
        });
		mouseLeaveConn = settingsButton.mouseLeave.connect([this]() {
			settingsButton.setBackgroundColor({ 0, 153, 51 });
		});
		mouseLeaveConn = exitButton.mouseLeave.connect([this]() {
			exitButton.setBackgroundColor({ 0, 153, 51 });
		});
	}

	/**
	 * @fn	void exit() override
	 *
	 * @brief	Disconnects all the eventconnections used in this state.
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/25/2018
	 */

	void exit() override {
		startButton.setPosition({ 0,0 });
		startButtonPressedConn.disconnect();
		startButtonReleasedConn.disconnect();
        settingsButtonPressedConn.disconnect();
        settingsButtonReleasedConn.disconnect();
        exitButtonPressedConn.disconnect();
		exitButtonReleasedConn.disconnect();

		mapEditorButtonReleasedConn.disconnect();
	}

	/**
	 * @fn	void update(const float elapsedTime) override
	 *
	 * @brief	Updates the buttons and the fimmy text.
	 *
	 * @author	Jeffrey de Waal
	 * @date	1/25/2018
	 *
	 * @param	elapsedTime	The elapsed time.
	 */

	void update(const float elapsedTime) override {
		startButton.setPosition({ 640, 360 });
		statemachine.window.draw(menuLabel);
		statemachine.window.draw(startButton);
		statemachine.window.draw(settingsButton);
		statemachine.window.draw(exitButton);
		statemachine.window.draw(mapEditorButton);
	}
};