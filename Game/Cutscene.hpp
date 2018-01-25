#pragma once

#include <iomanip>
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

	/** @brief	The image background */
	Rectangle imageBackground;

	/** @brief	The clock used for timing the subtitle display events. */
	sf::Clock clock;

	/** @brief	The last subtitle update time. */
	sf::Time lastSubtitleUpdate;

	/** @brief	The key released connection */
	EventConnection keyReleasedConnection;
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
		subtitleText.setPosition({100.0f, statemachine.window.getSize().y - 80.0f});
		subtitleText.setText("");

		imageBackground.setPosition({640, 360});
		imageBackground.setSize({ 1280, 720 });
		imageBackground.setFillColor(sf::Color{ 0, 0 ,0 });

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
				auto item = subtitles.back();

				std::tm subTimeStart, subTimeEnd;

				std::istringstream ssStart(item->getTimeStart());
				std::istringstream ssStop(item->getTimeEnd());

				// Parse the start and end time into an std::td (time) object.
				ssStart >> std::get_time(&subTimeStart, "%H:%M:%S");
				ssStop >> std::get_time(&subTimeEnd, "%H:%M:%S");

				if (clock.getElapsedTime().asSeconds() >= (subTimeStart.tm_min * 60) + subTimeStart.tm_sec) {
					subtitleText.setText(item->getText());
				}

				if (clock.getElapsedTime().asSeconds() >= (subTimeEnd.tm_min * 60) + subTimeEnd.tm_sec) {
					subtitleText.setText("");
					
					subtitles.pop_back();
				}
			}
			else if (subtitleText.getText() == "") {
				// Done with the cutscene, transition to running state.
				statemachine.doTransition("running");
			}

		}

		imageBackground.draw(statemachine.window);
		subtitleText.draw(statemachine.window);
	}
};