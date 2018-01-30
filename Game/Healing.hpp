#pragma once
#include <vector>
#include <exception>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Rectangle.hpp"
#include "Ball.hpp"
#include "EventSource.hpp"
#include "Keyboard.hpp"
#include "KeyScheme.hpp"

class Healing : public Rectangle {
private:

public:
	Healing() {
		setSize({ 40, 40 });
		setFillColor(sf::Color(0, 255, 0));
		setGravity({ 0, 0 });
	};
};
