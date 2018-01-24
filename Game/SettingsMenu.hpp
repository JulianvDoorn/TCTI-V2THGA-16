#pragma once

#include "State.hpp"
#include "Statemachine.hpp"
#include "Characters.hpp"
#include "Button.hpp"

class SettingsMenu : public State {
    Statemachine& statemachine;

	unsigned int volume;

    Label menuLabel;
	Label volumeLabel;
    Button exitButton;

    EventConnection<> exitButtonPressedConn;
    EventConnection<> exitButtonReleasedConn;

	EventConnection<sf::Keyboard::Key> changeVolumeConn;
    EventConnection<> mouseEnterConn;
    EventConnection<> mouseLeaveConn;

public:
    SettingsMenu(Statemachine& statemachine) :
            statemachine(statemachine),
            menuLabel(),
            exitButton(),
			volume(0)
    {
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

    void exit() override {
        exitButtonPressedConn.disconnect();
        exitButtonReleasedConn.disconnect();
        mouseEnterConn.disconnect();
        mouseLeaveConn.disconnect();
		changeVolumeConn.disconnect();
    }

    void update(const float elapsedTime) override {
        menuLabel.draw(statemachine.window);
        exitButton.draw(statemachine.window);
		volumeLabel.draw(statemachine.window);
    }
};