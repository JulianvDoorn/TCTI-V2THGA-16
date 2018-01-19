#pragma once

#include <SFML/Graphics.hpp>

class KeyScheme {
public:
	enum class Difficulty {
		EASY,
		MODERATE,
		HARD
	};

	Difficulty difficulty;

	sf::Keyboard::Key moveLeft;
	sf::Keyboard::Key moveRight;
	sf::Keyboard::Key jump;
	sf::Keyboard::Key roll;
};