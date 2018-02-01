#pragma once

#include "Collidable.hpp"
#include "InteractionGroup.hpp"

/**
 * @class	CollisionGroup
 *
 * @brief	A collision group.
 *
 * @author	Jeffrey
 * @date	2/1/2018
 */

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

	/**
	 * @fn	Collidable& CollisionGroup::getPrimary()
	 *
	 * @brief	Gets the primary
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @return	The primary.
	 */

	Collidable& getPrimary() {
		return *primaryCollidable;
	}

	/**
	 * @fn	void CollisionGroup::setPrimary(Collidable& collidable)
	 *
	 * @brief	Sets a primarycollidable
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param [in,out]	collidable	The collidable.
	 */

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

	/**
	 * @fn	void CollisionGroup::erase(Collidable& collidable)
	 *
	 * @brief	Erases the given collidable
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param [in,out]	collidable	The collidable.
	 */

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