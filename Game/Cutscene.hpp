#pragma once

#include <iomanip>
#include <memory>
#include "State.hpp"
#include "Statemachine.hpp"
#include "SubtitleParser.hpp"
#include "Label.hpp"

/**
 * @class	Cutscene
 *
 * @brief	The cutscene state.
 *
 * @author	Wiebe
 * @date	25-1-2018
 */

class Cutscene : public State {

	/** @brief	The statemachine */
	Statemachine& statemachine;

	/** @brief	Vector containing subtitles for the cutscene. */
	SubtitleVector subtitles;

	/** @brief	The subtitle file parser. */
	SubtitleParser parser;

	/** @brief	The subtitle text displayed on the window. */
	Label subtitleText;

	/** @brief	The clock used for timing the subtitle display events. */
	sf::Clock clock;

	/** @brief	The last subtitle update time. */
	sf::Time lastSubtitleUpdate;

	/** @brief	The key released connection */
	EventConnection keyReleasedConnection;

	/** @brief	The background */
	Body background;

	/** @brief	The texture */
	sf::Texture texture;

	/** @brief	True if displaying an subtitle */
	bool displaying = false;
public:

	/**
	 * @fn	Cutscene::Cutscene(Statemachine& statemachine)
	 *
	 * @brief	Constructor
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @param [in,out]	statemachine	The statemachine.
	 */

	Cutscene(Statemachine& statemachine) :
		statemachine(statemachine),
		parser("cutscene.srt")
	{}

	/**
	 * @fn	void Cutscene::entry() override
	 *
	 * @brief	Preparation for the cutscene.
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 */

	void entry() override {
		// Prepare the cutscene.
		subtitleText.setFont(AssetManager::instance()->getFont("arial"));
		subtitleText.setPosition({50.0f, statemachine.window.getSize().y - 80.0f});
		subtitleText.setText("");

		background.setPosition({ 640, 360 });
		background.setSize({ 1280, 720 });

		keyReleasedConnection = game.keyboard.keyReleased.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::Escape) {
				statemachine.doTransition("running");
			}
		});

		try {
			// Parse the srt file.
			parser.parse(subtitles);
		}
		catch (SubtitleReadException &ex) {
			// Cannot read the subtitle file, transition to the running state.
			std::cout << ex.what() << std::endl;
			statemachine.doTransition("running");
			return;
		}

		// Reserve the result.
		if (subtitles.size() > 1) {
			std::reverse(subtitles.begin(), subtitles.end());
		}
		else {
			// No subtitles available, skip the cutscene.
			statemachine.doTransition("running");
		}
	}

	/**
	 * @fn	void Cutscene::exit() override
	 *
	 * @brief	Exits the cutscene state
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 */

	void exit() override {
		// Empty subtitles vector
		subtitles.clear();

		keyReleasedConnection.disconnect();
	}

	/**
	 * @fn	void Cutscene::update(const float elapsedTime) override
	 *
	 * @brief	Updates the cutscene window
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @param	elapsedTime	The elapsed time.
	 */

	void update(const float elapsedTime) override {
		if ((clock.getElapsedTime().asSeconds() - lastSubtitleUpdate.asSeconds()) > 0) {
			if (subtitles.size() > 0) {
				const SubtitleItem& item = *subtitles.back();

				float subTimeStart = item.getTimeStart();
				float subTimeEnd = item.getTimeEnd();

				if (!displaying && clock.getElapsedTime().asSeconds() >= subTimeStart) {
					subtitleText.setText(item.getText());	

					if (item.getImagePath().length() > 0) {
						if (item.getImagePath().compare("NONE") == 0) {
							background.setSize({ 0, 0 });
						}
						else {
							background.setSize({ 1280, 720 });
							
							if (texture.loadFromFile(item.getImagePath())) {
								background.setTexture(&texture, true);
							}
						}
					}

					displaying = true;
				}

				if (clock.getElapsedTime().asSeconds() >= subTimeEnd) {
					subtitleText.setText("");
					
					subtitles.pop_back();
					displaying = false;
				}
			}
			else if (subtitleText.getText() == "") {
				// Done with the cutscene, transition to running state.
				statemachine.doTransition("running");
			}

		}

		statemachine.window.draw(background);
		//statemachine.window.draw(imageBackground);
		statemachine.window.draw(subtitleText);
	}
};