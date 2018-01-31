#pragma once

#include "SubtitleDialog.hpp"

class TestState : public State {
	Statemachine& statemachine;

	EventedSubtitleDialog subtitleDialog;

public:
	TestState(Statemachine& statemachine) :
		statemachine(statemachine),
		subtitleDialog {
			SubtitleItem(0, "Dikke tieten0", ""),
			SubtitleItem(0, "Dikke tieten1", ""),
			SubtitleItem(0, "Dikke tieten2", ""),
			SubtitleItem(0, "Dikke tieten3", "")
		}
	{ }

	void entry() override {
		subtitleDialog.waitForEvent(game.keyboard.keyPressed, [](sf::Keyboard::Key key) { return key == sf::Keyboard::Key::Return; });
	}

	void exit() override {

	}

	void update(const float elapsedTime) override {
		subtitleDialog.update(elapsedTime);

		statemachine.window.draw(subtitleDialog);
	}
};