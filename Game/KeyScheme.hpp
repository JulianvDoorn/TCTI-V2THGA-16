#pragma once

#include <SFML/Graphics.hpp>

class KeyScheme {
public:
	enum class Difficulty {
		EASY,
		MODERATE,
		HARD,
		UNDEFINED
	};

	KeyScheme(sf::Keyboard::Key moveLeft, sf::Keyboard::Key moveRight, sf::Keyboard::Key jump, sf::Keyboard::Key roll, Difficulty difficulty = Difficulty::UNDEFINED) :
		moveLeft(moveLeft),
		moveRight(moveRight),
		jump(jump),
		roll(roll),
		difficulty(difficulty)
	{};

	KeyScheme() { difficulty = Difficulty::UNDEFINED; };

	Difficulty difficulty;

	sf::Keyboard::Key moveLeft;
	sf::Keyboard::Key moveRight;
	sf::Keyboard::Key jump;
	sf::Keyboard::Key roll;
};