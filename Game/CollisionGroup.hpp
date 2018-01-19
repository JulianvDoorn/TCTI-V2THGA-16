#pragma once

#include "Collidable.hpp"

class CollisionGroup {
	Collidable* primaryCollidable;
	std::vector<Collidable*> otherCollidables;

public:
	CollisionGroup() : primaryCollidable(nullptr) { }

	CollisionGroup(Collidable& collidable) : primaryCollidable(&collidable) { }

	void add(Collidable& collidable) {
		otherCollidables.push_back(&collidable);
	}

	void resolveCollisions() {
		for (Collidable* collidable : otherCollidables) {
			Collision collision = primaryCollidable->getCollision(*collidable);

			if (collision.intersects()) {
				primaryCollidable->resolveCollision(*collidable, collision);
			}
		}
	}
};