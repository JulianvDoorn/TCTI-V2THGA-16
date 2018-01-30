#pragma once

#include "Collidable.hpp"
#include "InteractionGroup.hpp"

class CollisionGroup : public std::vector<Collidable*>, public InteractionGroup {
	/** @brief	The primary collidable to compare with all otherCollidables */
	Collidable* primaryCollidable;

public:

	/**
	 * @fn	CollisionGroup::CollisionGroup()
	 *
	 * @brief	Default constructor.
	 * 			Constructs a CollisionGroup without a primaryCollidable.
	 * 			Undefined behavior when CollisionGroup::resolve() is invoked when primaryCollidable == nullptr.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 */

	CollisionGroup() : primaryCollidable(nullptr) { }

	/**
	 * @fn	CollisionGroup::CollisionGroup(Collidable& collidable)
	 *
	 * @brief	Constructor.
	 * 			Constructs a CollisionGroup with collidable as primaryCollidable.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	collidable	The collidable.
	 */

	CollisionGroup(Collidable& collidable) : primaryCollidable(&collidable) { }

	Collidable& getPrimary() {
		return *primaryCollidable;
	}

	void setPrimary(Collidable& collidable) {
		primaryCollidable = &collidable;
	}

	/**
	 * @fn	void CollisionGroup::add(Collidable& collidable)
	 *
	 * @brief	Adds collidable to the vector of collidables to compare with primaryCollidable.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	collidable	The collidable to add.
	 */

	void add(Collidable& collidable) {
		push_back(&collidable);
	}

	void erase(Collidable& collidable) {
		auto it = std::find(begin(), end(), &collidable);

		if (it != end()) {
			std::vector<Collidable*>::erase(it);
		}
	}

	/**
	 * @fn	void CollisionGroup::resolve()
	 *
	 * @brief	Resolve collisions between primaryCollidable and all otherCollidables.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 */

	void resolve() override {
		if (primaryCollidable != nullptr) {
			std::for_each(begin(), end(), [this](Collidable* collidable) {
				Collision collision = primaryCollidable->getCollision(*collidable);

				if (collision.intersects()) {
					primaryCollidable->resolveCollision(*collidable, collision);
				}
			});
		}
	}
};