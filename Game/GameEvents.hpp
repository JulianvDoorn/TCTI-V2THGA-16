#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEventDecoder.hpp"
#include "EventSource.hpp"

class GameEvents {
public:
	GameEvents() { }

	EventSource<> died;
	EventSource<> fellOffMap;
};