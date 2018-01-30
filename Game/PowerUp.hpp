#pragma once

#include "Player.hpp"
#include "Rectangle.hpp"
#include "Map.hpp"

class PowerUp : public Rectangle {
	EventConnection collidedConn;
	IntersectionGroup* powerUpIntersectionGroup;

public:
	PowerUp(IntersectionGroup* powerUpIntersectionGroup) : powerUpIntersectionGroup(powerUpIntersectionGroup) { }

	virtual void apply(Player& player) const = 0;

	void connect(Player& player, Map& map) {
		collidedConn = collided.connect([&](Collidable& other) {
			apply(player);

			map.eraseDrawable(*this);
			map.removeObjectGroup(powerUpIntersectionGroup);

			delete powerUpIntersectionGroup;
		});
	}

	void disconnect() {
		collidedConn.disconnect();
	}
};