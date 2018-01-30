#pragma once

#include <SFML/Graphics.hpp>

class GameObject {
public:
	virtual void update(const float elapsedTime) = 0;
};