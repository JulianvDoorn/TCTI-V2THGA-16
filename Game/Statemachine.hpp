#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "GameState.hpp"
#include "Events.hpp"

/**
 * @class	InvalidStateException
 *
 * @brief	Exception for when a transition is requested to an invalid state. Usually non-existent states.
 *
 * @author	Julian
 * @date	2018-01-19
 */

class InvalidStateException : public std::exception {
public:
	const char* what() const noexcept override {
		return "You tried to transist into an invalid state";
	}
};

class Statemachine {
	/** @brief	Available states to do transitions towards. */
	std::vector<GameState*> states;

	/** @brief	The current state. */
	GameState* currentState = nullptr;

public:
	/** @brief	The window to render to. */
	sf::RenderWindow& window;

	/**
	 * @fn	Statemachine::Statemachine(sf::RenderWindow& window)
	 *
	 * @brief	Constructor
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	window	The render window to apply rendering to.
	 */

	Statemachine(sf::RenderWindow& window) : window(window) { }

	/**
	 * @fn	void Statemachine::addState(GameState& gameState)
	 *
	 * @brief	Adds a state to Statemachine::states
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	gameState	State of the game.
	 */

	void addState(GameState& gameState) {
		states.push_back(&gameState);
	}

	/**
	 * @fn	void Statemachine::doTransition(const std::string& name)
	 *
	 * @brief	Executes a transition to the state with the specified name.
	 * 			Executes GameState::exit() on the leaving state and GameState::entry() on the entering state.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @exception	InvalidStateException	Thrown when a transition is requested to an invalid state. Usually non-existent states.
	 *
	 * @param	name	The name of the state to transist to.
	 */

	void doTransition(const std::string& name) {
		if (currentState != nullptr) {
			currentState->exit();
		}

		auto it = std::find(states.begin(), states.end(), name); // requires: operator==(GameState*, const std::string&)

		if (it == states.end()) { // 'name' not a registered state
			throw InvalidStateException();
		}

		currentState = *it; // dereference iterator to get pointer to GameState

		currentState->entry();
	}

	/**
	* @fn	virtual void GameState::update(const float elapsedTime) = 0;
	*
	* @brief	Runs an update cycle for the current state
	*
	* @author	Julian
	* @date	2018-01-19
	*
	* @param	elapsedTime	The elapsed time.
	*/
	
	void update(const float elapsedTime) {
		window.clear(sf::Color(0, 0, 0));
		currentState->update(elapsedTime);
		window.display();
	}
};