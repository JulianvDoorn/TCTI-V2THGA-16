#pragma once

#include <vector>

#include "Collidable.hpp"

class IntersectionGroup : public std::vector<Collidable*>, public InteractionGroup {
	Collidable* primaryCollidable;

public:
	IntersectionGroup() : primaryCollidable(nullptr) { }
	
	void add(Collidable& collidable) {
		push_back(&collidable);
	}

	void setPrimary(Collidable& collidable) {
		primaryCollidable = &collidable;
	}

	void resolve() override {
		for (Collidable* collidable : *this) {
			if (primaryCollidable->getCollision(*collidable).intersects()) {
				primaryCollidable->onCollide(*collidable);
			}
		}
	}
};