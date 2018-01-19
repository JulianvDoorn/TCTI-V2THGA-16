#pragma once

#include <SFML/Graphics.hpp>

#include "SFMLEventDecoder.hpp"
#include "EventSource.hpp"

class GameEvents {
public:
	GameEvents() { }

	/** @brief	Died event, meant to be fired when the player dies. */
	EventSource<> died;


	/** @brief	Fell off map event, meant to be fired when the player falls off the map. */
	EventSource<> fellOffMap;
};