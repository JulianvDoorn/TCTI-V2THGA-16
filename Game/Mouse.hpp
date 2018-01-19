#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEventDecoder.hpp"
#include "EventSource.hpp"

/**
 * @class	Mouse
 *
 * @brief	Mouse class for handling sf::Events and to turn them into EventSource<Args...> signals
 *
 * @author	Julian
 * @date	2018-01-19
 */

class Mouse : public SFMLEventDecoder {
	sf::Window* window;

public:

	/**
	 * @fn	Mouse::Mouse()
	 *
	 * @brief	Default constructor.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 */

	Mouse() : window(nullptr) { }

	/**
	 * @fn	Mouse::Mouse(sf::Window& window)
	 *
	 * @brief	Constructor.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	window	The window to get the mouse posititions from.
	 */

	Mouse(sf::Window& window) : window(&window) { }


	/** @brief	Mouse left button up, fires when the left mouse button is released. */
	EventSource<sf::Vector2i> mouseLeftButtonUp;

	/** @brief	Mouse right button up, fires when the right mouse button is released. */
	EventSource<sf::Vector2i> mouseRightButtonUp;

	/** @brief	Mouse left button down, fires when the left mouse button is pressed. */
	EventSource<sf::Vector2i> mouseLeftButtonDown;

	/** @brief	Mouse right button down, fires when the right mouse button is pressed. */
	EventSource<sf::Vector2i> mouseRightButtonDown;

	/** @brief	Mouse moved, fires when the mouse is moved. */
	EventSource<sf::Vector2i> mouseMoved;

	/**
	 * @fn	virtual void SFMLEventDecoder::decodeSFMLEvent(sf::Event ev) const = 0;
	 *
	 * @brief	Decode sfml event into an EventSource signal.
	 * 			sf::Event::MouseMoved fires Mouse::mouseMoved<sf::Vector2i>::fire(mousePosition)
	 * 			sf::Event::MouseButtonPressed with button == sf::Mouse::Left invokes Mouse::mouseLeftButtonDown<sf::Vector2i>::fire(mousePosition)
	 * 			sf::Event::MouseButtonPressed with button == sf::Mouse::Right invokes Mouse::mouseRightButtonDown<sf::Vector2i>::fire(mousePosition)
	 * 			sf::Event::MouseButtonReleased with button == sf::Mouse::Left invokes Mouse::mouseLeftButtonUp<sf::Vector2i>::fire(mousePosition)
	 * 			sf::Event::MouseButtonReleased with button == sf::Mouse::Right invokes Mouse::mouseRightButtonUp<sf::Vector2i>::fire(mousePosition)
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param	ev	SFML event to decode into an EventSource signal.
	 */
	
	void decodeSFMLEvent(sf::Event ev) const override {
		if (ev.type == sf::Event::MouseMoved) {
			mouseMoved.fire(sf::Mouse::getPosition(*window));
		} else if (ev.type == sf::Event::MouseButtonPressed) {
			if (ev.mouseButton.button == sf::Mouse::Left) {
				mouseLeftButtonDown.fire(sf::Mouse::getPosition(*window));
			} else if (ev.mouseButton.button == sf::Mouse::Right) {
				mouseRightButtonDown.fire(sf::Mouse::getPosition(*window));
			}
		} else if (ev.type == sf::Event::MouseButtonReleased) {
			 if (ev.mouseButton.button == sf::Mouse::Left) {
				 mouseLeftButtonUp.fire(sf::Mouse::getPosition(*window));
			 } else if (ev.mouseButton.button == sf::Mouse::Right) {
				 mouseRightButtonUp.fire(sf::Mouse::getPosition(*window));
			 }
		}
	}

	/**
	 * @fn	Mouse& Mouse::operator= (Mouse& rhs)
	 *
	 * @brief	Assignment operator
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	rhs	The right hand side.
	 *
	 * @return	A shallow copy of this object.
	 */

	Mouse& operator= (const Mouse& rhs) {
		window = rhs.window;

		return *this;
	}
};