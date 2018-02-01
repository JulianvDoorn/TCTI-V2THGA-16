#pragma once

#include <vector>

#include "Collidable.hpp"

/**
 * @class	IntersectionGroup
 *
 * @brief	An intersection group.
 *
 * @author	Jeffrey
 * @date	1/31/2018
 */

class IntersectionGroup : public std::vector<Collidable*>, public InteractionGroup {
	/** @brief	The primary collidable */
	Collidable* primaryCollidable;

public:

	/**
	 * @fn	IntersectionGroup::IntersectionGroup()
	 *
	 * @brief	Default constructor
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 */

	IntersectionGroup() : primaryCollidable(nullptr) { }

	/**
	 * @fn	void IntersectionGroup::add(Collidable& collidable)
	 *
	 * @brief	Adds collidable
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param [in,out]	collidable	The collidable to add.
	 */

	void add(Collidable& collidable) {
		add(&collidable);
	}

	/**
	 * @fn	void IntersectionGroup::add(Collidable* collidable)
	 *
	 * @brief	Adds collidable
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param [in,out]	collidable	If non-null, the collidable to add.
	 */

	void add(Collidable* collidable) {
		push_back(collidable);
	}

	/**
	 * @fn	void IntersectionGroup::erase(Collidable& collidable)
	 *
	 * @brief	Erases the given collidable
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param [in,out]	collidable	The collidable.
	 */

	void erase(Collidable& collidable) {
		erase(&collidable);
	}

	/**
	 * @fn	void IntersectionGroup::erase(Collidable* collidable)
	 *
	 * @brief	Erases the given collidable
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param [in,out]	collidable	If non-null, the collidable.
	 */

	void erase(Collidable* collidable) {
		auto it = std::find(begin(), end(), collidable);

		if (it != end()) {
			std::vector<Collidable*>::erase(it);
		}
	}

	/**
	 * @fn	void IntersectionGroup::setPrimary(Collidable& collidable)
	 *
	 * @brief	Sets a primarycollidable
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param [in,out]	collidable	The collidable.
	 */

	void setPrimary(Collidable& collidable) {
		setPrimary(&collidable);
	}

	/**
	 * @fn	void IntersectionGroup::setPrimary(Collidable* collidable)
	 *
	 * @brief	Sets a primarycollidable
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param [in,out]	collidable	If non-null, the collidable.
	 */

	void setPrimary(Collidable* collidable) {
		primaryCollidable = collidable;
	}

	/**
	 * @fn	void IntersectionGroup::resolve() override
	 *
	 * @brief	Resolves this object
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 */

	void resolve() override {
		std::for_each(begin(), end(), [this](Collidable* collidable) {
			if (primaryCollidable->getCollision(*collidable).intersects()) {
				primaryCollidable->onCollide(*collidable);
			}
		});
	}
};