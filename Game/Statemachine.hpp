#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

#include "State.hpp"
#include "Events.hpp"
#include "BaseFactory.hpp"

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
	std::map<std::string, std::unique_ptr<State>> states;

	/** @brief	The current state. */
	State* currentState = nullptr;

public:
	BaseFactory<State, std::string> stateFactory;

	template<class T>
	void registerState(const std::string& name) {
		stateFactory.registerCreateMethod(name, [this]() {
			return new T(*this);
		});

		addState(name, *stateFactory.create(name));
	}

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
	 * @fn	void Statemachine::addState(State& gameState)
	 *
	 * @brief	Adds a state to Statemachine::states
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	gameState	State of the game.
	 */

	void addState(const std::string& name, State& gameState) {
		states[name] = std::move(std::unique_ptr<State>(&gameState));
	}

	void resetState(const std::string& name) {
		auto it = states.find(name); // requires: operator==(std::unique_ptr<State>&, const std::string&)

		if (it != states.end()) {
			it->second.reset();
			it->second = std::move(std::unique_ptr<State>(stateFactory.create(name)));
		} else { // 'name' not a registered state
			throw InvalidStateException();
		}
	}

	/**
	 * @fn	void Statemachine::doTransition(const std::string& name)
	 *
	 * @brief	Executes a transition to the state with the specified name.
	 * 			Executes State::exit() on the leaving state and State::entry() on the entering state.
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

		auto it = states.find(name); // requires: operator==(std::unique_ptr<State>&, const std::string&)

		if (it != states.end()) {
			currentState = &(*it->second); // dereference iterator to get unique_ptr<State>, dereference unique_ptr<State> to get State, get address of State to store
			currentState->entry();
		} else { // 'name' not a registered state
			throw InvalidStateException();
		}
	}

	/**
	* @fn	virtual void State::update(const float elapsedTime) = 0;
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