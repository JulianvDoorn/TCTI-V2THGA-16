#pragma once

#include <vector>

#include "Collidable.hpp"

class IntersectionGroup : public std::vector<Collidable*>, public InteractionGroup {
	Collidable* primaryCollidable;

public:
	IntersectionGroup() : primaryCollidable(nullptr) { }
	
	void add(Collidable& collidable) {
		add(&collidable);
	}

	void add(Collidable* collidable) {
		push_back(collidable);
	}

	void erase(Collidable& collidable) {
		erase(&collidable);
	}

	void erase(Collidable* collidable) {
		auto it = std::find(begin(), end(), collidable);

		if (it != end()) {
			std::vector<Collidable*>::erase(it);
		}
	}

	void setPrimary(Collidable& collidable) {
		setPrimary(&collidable);
	}

	void setPrimary(Collidable* collidable) {
		primaryCollidable = collidable;
	}

	void resolve() override {
		std::for_each(begin(), end(), [this](Collidable* collidable) {
			if (primaryCollidable->getCollision(*collidable).intersects()) {
				primaryCollidable->onCollide(*collidable);
			}
		});
	}
};