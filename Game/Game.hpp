#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEventDecoder.hpp"
#include "EventSource.hpp"
#include "AssetManager.hpp"

#include "Keyboard.hpp"
#include "Mouse.hpp"

/**
 * @class	Game
 *
 * @brief	Game class used for global event handling.
 * 			Use this class to connect to game events, keyboard and/or mouse inputs.
 *
 * @author	Julian
 * @date	2018-01-19
 */

class Game : public SFMLEventDecoder {
public:
	Game() { }

	Game(sf::Window& window) : keyboard(window), mouse(window) { }

	/** @brief	Died event, meant to be fired when the player dies. */
	EventSource<> died;

	/** @brief	Fell off map event, meant to be fired when the player falls off the map. */
	EventSource<> fellOffMap;

	/** @brief	Keyboard event collection, events in this object get fired accordingly */
	Keyboard keyboard;

	/** @brief	Mouse event collection, events in this object get fired accordingly */
	Mouse mouse;

	AssetManager assets;

	/**
	 * @fn	void Game::decodeSFMLEvent(sf::Event ev) const override
	 *
	 * @brief	Propagated the decodeSFMLEvent to Keyboard::decodeSFMLEvent(sf::Event) and Mouse::decodeSFMLEvent(sf::Event)
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param	ev	SFML event to decode into an EventSource signal.
	 */

	void decodeSFMLEvent(sf::Event ev) const override {
		keyboard.decodeSFMLEvent(ev);
		mouse.decodeSFMLEvent(ev);
	}

	/**
	 * @fn	Game& Game::operator= (const Game& rhs)
	 *
	 * @brief	Assignment operator
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param	rhs	The right hand side.
	 *
	 * @return	A shallow copy of this object.
	 */

	Game& operator= (const Game& rhs) {
		keyboard = rhs.keyboard;
		mouse = rhs.mouse;

		return *this;
	}
};

/** @brief	Global game instance, HAS TO BE ASSIGNED IN MAIN() USING @code Game game = Game(window); @endcode */
static Game game;