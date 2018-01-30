#pragma once

#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "Button.hpp"

/**
 * @class	SettingsMenu
 *
 * @brief	The settings menu state.
 *
 * @author	Wiebe
 * @date	25-1-2018
 */

class SettingsMenu : public State {

    /** @brief	The statemachine */
    Statemachine& statemachine;

	/** @brief	The global sound volume */
	unsigned int volume;

    /** @brief	The menu label */
    Label menuLabel;

	/** @brief	The volume label */
	Label volumeLabel;

    /** @brief	The exit button displayed on the window. */
    Button exitButton;

    /** @brief	The exit button pressed connection */
    EventConnection exitButtonPressedConn;

    /** @brief	The exit button released connection */
    EventConnection exitButtonReleasedConn;

	/** @brief	The change volume connection */
	EventConnection changeVolumeConn;

    /** @brief	The mouse enter connection */
    EventConnection mouseEnterConn;

    /** @brief	The mouse leave connection */
    EventConnection mouseLeaveConn;

public:

    /**
     * @fn	SettingsMenu::SettingsMenu(Statemachine& statemachine)
     *
     * @brief	Constructor
     *
     * @author	Wiebe
     * @date	25-1-2018
     *
     * @param [in,out]	statemachine	The statemachine.
     */

    SettingsMenu(Statemachine& statemachine) :
            statemachine(statemachine),
            menuLabel(),
            exitButton(),
			volume(0)
    {
		// Converts the SFML global volume (0%-100%) to an new 0-10 scale. For example, the volume level 40% will be converted to 4.
		volume = static_cast<unsigned int>(sf::Listener::getGlobalVolume() / 10.0f);

        menuLabel.setPosition({ 500, 200 });
        menuLabel.setCharSize(45);
        menuLabel.setColor(sf::Color::White);
        menuLabel.setText("Settings Menu");
        menuLabel.setStyle(sf::Text::Bold);

		volumeLabel.setPosition({450, 300});
		volumeLabel.setCharSize(18);
		volumeLabel.setColor(sf::Color::White);
		volumeLabel.setText(std::string("Volume (O, P): " + std::to_string(volume)));

        exitButton.setSize({350,100});
        exitButton.setPosition({640,620});
        exitButton.setCharSize(32);
        exitButton.setBackgroundColor({0,153,51});
        exitButton.setText("Return to main menu");
    }

    /**
     * @fn	void SettingsMenu::entry() override
     *
     * @brief	Preperation for the settings menu.
     *
     * @author	Wiebe
     * @date	25-1-2018
     */

    void entry() override {
        exitButtonPressedConn = exitButton.buttonPressed.connect([this](){
            exitButton.setBackgroundColor({0,163,61});
        });

        exitButtonReleasedConn = exitButton.buttonReleased.connect([this]() {
            statemachine.doTransition("main-menu");
        });

		changeVolumeConn = game.keyboard.keyPressed.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::P) {
				// Change volume up
				if (volume < 10) {
					volume++;
				}
			}
			else if (key == sf::Keyboard::Key::O) {
				// Change volume down
				if (volume > 0) {
					volume--;
				}
			}

			// Set new global volume.
			sf::Listener::setGlobalVolume(static_cast<float>(volume * 10));
			volumeLabel.setText(std::string("Volume (O, P): " + std::to_string(volume)));
		});

        mouseEnterConn = exitButton.mouseEnter.connect([this]() {
            exitButton.setBackgroundColor({ 0, 123, 21 });
        });
        mouseLeaveConn = exitButton.mouseLeave.connect([this]() {
            exitButton.setBackgroundColor({ 0, 153, 51 });
        });
    }

    /**
     * @fn	void SettingsMenu::exit() override
     *
     * @brief	Exits the settings menu.
     *
     * @author	Wiebe
     * @date	25-1-2018
     */

    void exit() override {
        exitButtonPressedConn.disconnect();
        exitButtonReleasedConn.disconnect();
        mouseEnterConn.disconnect();
        mouseLeaveConn.disconnect();
		changeVolumeConn.disconnect();
    }

    /**
     * @fn	void SettingsMenu::update(const float elapsedTime) override
     *
     * @brief	Updates the settings menu window.
     *
     * @author	Wiebe
     * @date	25-1-2018
     *
     * @param	elapsedTime	The elapsed time.
     */

    void update(const float elapsedTime) override {
		statemachine.window.draw(menuLabel);
		statemachine.window.draw(exitButton);
		statemachine.window.draw(volumeLabel);
    }
};