#pragma once

#include <iomanip>
#include "State.hpp"
#include "Statemachine.hpp"
#include "SubtitleParser.hpp"
#include "Label.hpp"

class Cutscene : public State {
	Statemachine& statemachine;

	SubtitleVector subtitles;
	SubtitleParser parser;
	Label subtitleText;
	Rectangle imageBackground;

	sf::Clock clock;
	sf::Time lastTime;

	EventConnection<sf::Keyboard::Key> keyReleasedConnection;
public:
	Cutscene(Statemachine& statemachine) :
		statemachine(statemachine),
		parser("test.srt")
	{}

	void entry() override {
		subtitleText.setFont(AssetManager::instance()->getFont("arial"));
		subtitleText.setPosition({100.0f, statemachine.window.getSize().y - 80.0f});
		subtitleText.setText("Hello World!");

		imageBackground.setPosition({640, 360});
		imageBackground.setSize({ 1280, 720 });
		//imageBackground.setTextureRect({ 0, 0, 1280, 720 });
		imageBackground.setFillColor(sf::Color{ 0, 0 ,0 });

		keyReleasedConnection = game.keyboard.keyReleased.connect([this](sf::Keyboard::Key key) {
			if (key == sf::Keyboard::Key::Escape) {
				statemachine.doTransition("running");
			}
		});

		// Parse the srt file
		parser.parse(subtitles);

		// Reserve the result
		if (subtitles.size() > 1) {
			std::reverse(subtitles.begin(), subtitles.end());
		}
	}

	void exit() override {
		// Empty subtitles vector
		subtitles.clear();

		keyReleasedConnection.disconnect();
	}

	void update(const float elapsedTime) override {
		if ((clock.getElapsedTime().asSeconds() - lastTime.asSeconds()) > 0) {
			if (subtitles.size() > 0) {
				auto item = subtitles.back();

				std::tm subTimeStart, subTimeEnd;

				std::istringstream ssStart(item->getTimeStart());
				std::istringstream ssStop(item->getTimeEnd());

				ssStart >> std::get_time(&subTimeStart, "%H:%M:%S");
				ssStop >> std::get_time(&subTimeEnd, "%H:%M:%S");

				subTimeStart.tm_min = 0;
				subTimeEnd.tm_min = 0;

				if (clock.getElapsedTime().asSeconds() >= (subTimeStart.tm_min * 60) + subTimeStart.tm_sec) {
					subtitleText.setText(item->getText());
				}

				if (clock.getElapsedTime().asSeconds() >= (subTimeEnd.tm_min * 60) + subTimeEnd.tm_sec) {
					subtitleText.setText("");
					subtitles.pop_back();
				}
			}
			else {
				// Done with the cutscene, transition to running state.
				statemachine.doTransition("running");
			}

		}

		
		imageBackground.draw(statemachine.window);
		subtitleText.draw(statemachine.window);
		
		
		//gameExitButton.draw(statemachine.window);
		//gameResumeButton.draw(statemachine.window);
	}
};