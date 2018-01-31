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

	/**
	 * @fn	Game::Game()
	 *
	 * @brief	Default constructor
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 */

	Game() { }

	/**
	 * @fn	Game::Game(sf::RenderWindow& window)
	 *
	 * @brief	Constructor
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param [in,out]	window	The window.
	 */

	Game(sf::RenderWindow& window) : window(&window), keyboard(window), mouse(window){ }

	/** @brief	The window */
	sf::RenderWindow* window;

	/** @brief	Died event, meant to be fired when the player dies. */
	EventSource<> died;

	/** @brief	Fell off map event, meant to be fired when the player falls off the map. */
	EventSource<> fellOffMap;

	/** @brief	Keyboard event collection, events in this object get fired accordingly */
	Keyboard keyboard;

	/** @brief	Mouse event collection, events in this object get fired accordingly */
	Mouse mouse;

	//std::shared_ptr<AssetManager> assets = nullptr;

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

	void decodeSFMLEvent(sf::Event ev) override {
		keyboard.decodeSFMLEvent(ev);
		mouse.decodeSFMLEvent(ev);
	}

	/**~Game() {
		std::cout << "Destructing game...\n";
	}**/
};

/** @brief	Global game instance, HAS TO BE ASSIGNED IN MAIN() USING @code Game game = Game(window); @endcode */
static Game game;