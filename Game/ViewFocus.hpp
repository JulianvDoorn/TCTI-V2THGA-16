#pragma once

#include "PhysicsObject.hpp"

class ViewFocus {
	sf::View view;
	sf::RenderWindow& window;
	PhysicsObject* focus;

public:
	ViewFocus(sf::RenderWindow& window, PhysicsObject& physicsObject) :
		view(sf::Vector2f(150, 10), sf::Vector2f(1280, 720)),
		window(window),
		focus(&physicsObject)
	{ }

	ViewFocus(sf::RenderWindow& window) :
		view(sf::Vector2f(150, 10), sf::Vector2f(1280, 720)),
		window(window),
		focus(nullptr)
	{ }

	void setFocus(PhysicsObject& focus) {
		this->focus = &focus;
	}

	void unsetFocus() {
		this->focus = nullptr;
	}

	void update() {
		if (focus != nullptr) {
			view.setCenter(focus->getPosition());
		} else {
			view.setCenter(window.getDefaultView().getCenter());
		}

		window.setView(view);
	}
};