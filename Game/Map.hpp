#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "DrawableGroup.hpp"
#include "CollisionGroup.hpp"
#include "Events.hpp"

bool operator== (const std::unique_ptr<PhysicsObject>& lhs, PhysicsObject* rhs) {
	return &(*lhs) == rhs;
}

bool operator== (PhysicsObject* lhs, const std::unique_ptr<PhysicsObject>& rhs) {
	return lhs == &(*rhs);
}

/**
 * @class	Map
 *
 * @brief	Map container class.
 * 			All built maps using MapFactory are put into this container.
 *
 * @author	Julian
 * @date	2018-01-25
 */

class Map : public std::vector<std::unique_ptr<PhysicsObject>> {
	/** @brief	Vector of drawables */
	DrawableGroup drawableGroup;

	/** @brief	Vector of collidables directly managed by Map */
	CollisionGroup primaryCollisionGroup;

	/** @brief	Collision group references that Map::resolveCollisions() should resolve as well. Nude pointers are used since the references CollisionGroups belong to other objects. */
	std::vector<CollisionGroup*> collisionGroups;

public:
	EventSource<PhysicsObject&> objectAdded;
	EventSource<PhysicsObject&> objectRemoving;

	/**
	 * @fn	void Map::addDrawable(Drawable* drawable)
	 *
	 * @brief	Adds a drawable to drawableGroup
	 *
	 * @author	Julian
	 * @date	2018-01-25
	 *
	 * @param [in,out]	drawable	If non-null, the drawable.
	 */

	void addDrawable(Drawable* drawable) {
		drawableGroup.add(*drawable);
	}

	/**
	 * @fn	void Map::addDrawable(Drawable& drawable)
	 *
	 * @brief	Adds a drawable to drawableGroup
	 *
	 * @author	Julian
	 * @date	2018-01-25
	 *
	 * @param [in,out]	drawable	The drawable.
	 */

	void addDrawable(Drawable& drawable) {
		drawableGroup.add(drawable);
	}

	/**
	 * @fn	void Map::addCollidable(Collidable* collidable)
	 *
	 * @brief	Adds a collidable to primaryCollisionGroup
	 *
	 * @author	Julian
	 * @date	2018-01-25
	 *
	 * @param [in,out]	collidable	If non-null, the collidable.
	 */

	void addCollidable(Collidable* collidable) {
		primaryCollisionGroup.add(*collidable);
	}

	/**
	 * @fn	void Map::addCollidable(Collidable& collidable)
	 *
	 * @brief	Adds a collidable to primaryCollisionGroup
	 *
	 * @author	Julian
	 * @date	2018-01-25
	 *
	 * @param [in,out]	collidable	The collidable.
	 */

	void addCollidable(Collidable& collidable) {
		primaryCollisionGroup.add(collidable);
	}

	/**
	 * @fn	void Map::setPrimaryCollidable(Collidable* collidable)
	 *
	 * @brief	Sets primary collidable of primaryCollisionGroup
	 *
	 * @author	Julian
	 * @date	2018-01-25
	 *
	 * @param [in,out]	collidable	If non-null, the collidable.
	 */

	void setPrimaryCollidable(Collidable* collidable) {
		primaryCollisionGroup.setPrimaryCollidable(*collidable);
	}

	/**
	 * @fn	void Map::setPrimaryCollidable(Collidable& collidable)
	 *
	 * @brief	Sets primary collidable of primaryCollisionGroup
	 *
	 * @author	Julian
	 * @date	2018-01-25
	 *
	 * @param [in,out]	collidable	The collidable.
	 */

	void setPrimaryCollidable(Collidable& collidable) {
		primaryCollisionGroup.setPrimaryCollidable(collidable);
	}

	/**
	 * @fn	void Map::draw(sf::RenderTarget& renderTarget)
	 *
	 * @brief	Draws the drawableGroup to the given render target
	 *
	 * @author	Julian
	 * @date	2018-01-25
	 *
	 * @param [in,out]	renderTarget	The render target.
	 */

	void draw(sf::RenderTarget& renderTarget) {
		drawableGroup.draw(renderTarget);
	}

	/**
	 * @fn	void Map::resolveCollisions()
	 *
	 * @brief	Resolve collisions of primaryCollisionGroup and all attached collision groups
	 *
	 * @author	Julian
	 * @date	2018-01-25
	 */

	void resolveCollisions() {
		primaryCollisionGroup.resolveCollisions();

		for (CollisionGroup* collisionGroup : collisionGroups) {
			collisionGroup->resolveCollisions();
		}
	}

	/**
	 * @fn	void Map::addCollisionGroup(CollisionGroup& collisionGroup)
	 *
	 * @brief	Adds a collision group reference to the collisionsGroups vector
	 *
	 * @author	Julian
	 * @date	2018-01-25
	 *
	 * @param [in,out]	collisionGroup	Group the collision belongs to.
	 */

	void addCollisionGroup(CollisionGroup& collisionGroup) {
		collisionGroups.push_back(&collisionGroup);
	}

	void addObject(PhysicsObject& physicsObject) {
		addObject(&physicsObject);
	}

	void addObject(PhysicsObject* physicsObject) {
		emplace_back(physicsObject);
		objectAdded.fire(*physicsObject);
	}

	void removeObject(PhysicsObject& physicsObject) {
		removeObject(&physicsObject);
	}

	void removeObject(PhysicsObject* physicsObject) {
		objectRemoving.fire(*physicsObject);

		auto it = std::find(begin(), end(), physicsObject);

		if (it != end()) {
			erase(it);
		}
		
		drawableGroup.erase(*physicsObject);
		primaryCollisionGroup.erase(*physicsObject);
	}
};