#pragma once

#include "Collidable.hpp"

class CollisionGroup : public std::vector<Collidable*> {
	/** @brief	The primary collidable to compare with all otherCollidables */
	Collidable* primaryCollidable;

public:

	/**
	 * @fn	CollisionGroup::CollisionGroup()
	 *
	 * @brief	Default constructor.
	 * 			Constructs a CollisionGroup without a primaryCollidable.
	 * 			Undefined behavior when CollisionGroup::resolveCollisions() is invoked when primaryCollidable == nullptr.
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

	void setPrimaryCollidable(Collidable& collidable) {
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

	/**
	 * @fn	void CollisionGroup::resolveCollisions()
	 *
	 * @brief	Resolve collisions between primaryCollidable and all otherCollidables.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 */

	void resolveCollisions() {
		for (Collidable* collidable : *this) {
			Collision collision = primaryCollidable->getCollision(*collidable);

			if (collision.intersects()) {
				primaryCollidable->resolveCollision(*collidable, collision);
			}
		}
	}
};