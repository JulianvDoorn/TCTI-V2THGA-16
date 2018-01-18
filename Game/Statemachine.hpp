#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "GameState.hpp"
#include "Events.hpp"

class Statemachine {
	std::vector<GameState*> states;
	GameState* currentState = nullptr;

public:
	sf::RenderWindow& window;
	Keyboard keyboard;
	Mouse mouse;

	Statemachine(sf::RenderWindow& window) : window(window), keyboard(window), mouse(window) { }

	void addState(GameState& gameState) {
		states.push_back(&gameState);
	}

	void doTransition(const std::string& name) {
		if (currentState != nullptr) {
			currentState->exit();
		}

		auto it = std::find(states.begin(), states.end(), name); // requires: operator==(GameState*, const std::string&)
		currentState = *it; // dereference iterator to get pointer to GameState

		currentState->entry();
	}

	void update(const float elapsedTime) {
		window.clear(sf::Color(0, 0, 0));
		currentState->update(elapsedTime);
		window.display();
	}
};