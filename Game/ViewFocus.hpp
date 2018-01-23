#pragma once

#include "PhysicsObject.hpp"

class ViewFocus {
	sf::View view;
	sf::RenderWindow& window;
	PhysicsObject* focus;

public:

	/**
	 * @fn	ViewFocus::ViewFocus(sf::RenderWindow& window, PhysicsObject& physicsObject) : view(sf::Vector2f(150, 10), sf::Vector2f(1280, 720)), window(window), focus(&physicsObject)
	 *
	 * @brief	Constructs a view for window with a focus on physicsObject
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	window		 	The window.
	 * @param [in,out]	physicsObject	The physics object.
	 */

	ViewFocus(sf::RenderWindow& window, PhysicsObject& physicsObject) :
		view(sf::Vector2f(150, 10), sf::Vector2f(1280, 720)),
		window(window),
		focus(&physicsObject)
	{ }

	/**
	 * @fn	ViewFocus::ViewFocus(sf::RenderWindow& window) : view(sf::Vector2f(150, 10), sf::Vector2f(1280, 720)), window(window), focus(nullptr)
	 *
	 * @brief	Constructs a view for window without a focus.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	window	The window.
	 */

	ViewFocus(sf::RenderWindow& window) :
		view(sf::Vector2f(150, 10), sf::Vector2f(1280, 720)),
		window(window),
		focus(nullptr)
	{ }

	/**
	 * @fn	void ViewFocus::setFocus(PhysicsObject& focus)
	 *
	 * @brief	Sets the focus of ViewFocus on focus
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	focus	The focus to apply on ViewFocus.
	 */

	void setFocus(PhysicsObject& focus) {
		this->focus = &focus;
	}

	/**
	 * @fn	void ViewFocus::unsetFocus()
	 *
	 * @brief	Unsets focus from any object
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 */

	void unsetFocus() {
		this->focus = nullptr;
	}

	/**
	 * @fn	void ViewFocus::update()
	 *
	 * @brief	Updates this object
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 */

	void update() {
		if (focus != nullptr) {
			view.setCenter(focus->getPosition());
		} else {
			view.setCenter(window.getDefaultView().getCenter());
		}

		//view.setViewport(sf::FloatRect(0.f, 0.f, 0.5f, 1.f));

		window.setView(view);
	}
};