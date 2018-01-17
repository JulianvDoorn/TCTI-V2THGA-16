#pragma once

#include <SFML\Graphics.hpp>
#include "Ball.hpp"

class Player : public Ball {
private:
	sf::View &view;
	sf::RenderWindow &window;

public:
	Player(sf::View &view, sf::RenderWindow &window) :
		view(view),
		window(window)
	{
		setRadius(20);
		setPosition({ 150, 450 });
		setFillColor(sf::Color(0, 255, 0));
	}
	void update(const float elapsedTime) override {
		PhysicsObject::update(elapsedTime);
		view.setCenter(getCenter());
		window.setView(view);
	}

	sf::FloatRect getBounds() override {
		return sf::CircleShape::getGlobalBounds();
	}

	using Ball::setPosition;
	using Ball::getPosition;
	using Ball::setRadius;
	using Ball::draw;
};