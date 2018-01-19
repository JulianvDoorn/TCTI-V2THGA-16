#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEventDecoder.hpp"
#include "EventSource.hpp"

/**
 * @class	Keyboard
 *
 * @brief	Keyboard class for handling sf::Events and to turn them into EventSource<Args...> signals
 *
 * @author	Julian
 * @date	2018-01-19
 */

class Keyboard : public SFMLEventDecoder {
public:

	/**
	 * @fn	Keyboard::Keyboard(sf::Window &window)
	 *
	 * @brief	Constructor.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	window	The window for which to turn keyRepeatedEnabled off.
	 */

	Keyboard(sf::Window &window) { window.setKeyRepeatEnabled(false); }


	/** @brief	Key pressed, fires when any key on the keyboard is pressed down. */
	EventSource<sf::Keyboard::Key> keyPressed;


	/** @brief	Key released, fires when any key on the keyboard is released up. */
	EventSource<sf::Keyboard::Key> keyReleased;

	/**
	 * @fn	virtual void SFMLEventDecoder::decodeSFMLEvent(sf::Event ev) const = 0;
	 *
	 * @brief	Decode sfml event into an EventSource signal.
	 * 			sf::Event::KeyPressed invokes Keyboard::keyPressed<sf::Keyboard::Key>::fire(key)
	 * 			sf::Event::KeyReleased invokes Keyboard::keyReleased<sf::Keyboard::Key>::fire(key)
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param	ev	SFML event to decode into an EventSource signal.
	 */

	void decodeSFMLEvent(sf::Event ev) const override {
		if (ev.type == sf::Event::KeyPressed) {
			keyPressed.fire(ev.key.code);
		}
		else if (ev.type == sf::Event::KeyReleased) {
			keyReleased.fire(ev.key.code);
		}
	}
};