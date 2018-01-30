#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "DrawableGroup.hpp"
#include "CollisionGroup.hpp"
#include "InteractionGroup.hpp"
#include "IntersectionGroup.hpp"
#include "Events.hpp"

/**
 * @fn	template <class T> bool operator== (const std::unique_ptr<T>& lhs, T* rhs)
 *
 * @brief	Equality operator for PhysicsObjects
 *
 * @author	Wiebe
 * @date	30-1-2018
 *
 * @tparam	T	Generic type parameter.
 * @param 		  	lhs	The first instance to compare.
 * @param [in,out]	rhs	If non-null, the second instance to compare.
 *
 * @return	True if the parameters are considered equivalent.
 */

template <class T>
bool operator== (const std::unique_ptr<T>& lhs, T* rhs) {
	return &(*lhs) == rhs;
}

/**
 * @fn	template <class T> bool operator== (T* lhs, const std::unique_ptr<T>& rhs)
 *
 * @brief	Equality operator for PhysicsObjects
 *
 * @author	Wiebe
 * @date	30-1-2018
 *
 * @tparam	T	Generic type parameter.
 * @param [in,out]	lhs	If non-null, the first instance to compare.
 * @param 		  	rhs	The second instance to compare.
 *
 * @return	True if the parameters are considered equivalent.
 */

template <class T>
bool operator== (T* lhs, const std::unique_ptr<T>& rhs) {
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

class Map : public std::vector<std::unique_ptr<Body>> {
	/** @brief	Vector of drawables */
	DrawableGroup drawableGroup;

	/** @brief	Vector of collidables directly managed by Map */
	CollisionGroup primaryCollisionGroup;

	/** @brief	Interaction group references that Map::resolve() should resolve as well. unique_ptrs are used since the InteractionGroups belong to this map only. */
	std::vector<std::unique_ptr<InteractionGroup>> interactionGroups;

public:
	EventSource<Body&> objectAdded;
	EventSource<Body&> objectRemoving;

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

	void addDrawable(sf::Drawable* drawable) {
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

	void addDrawable(sf::Drawable& drawable) {
		drawableGroup.add(drawable);
	}

	void eraseDrawable(sf::Drawable& drawable) {
		drawableGroup.erase(drawable);
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
		primaryCollisionGroup.setPrimary(*collidable);
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
		primaryCollisionGroup.setPrimary(collidable);
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
	 * @fn	void Map::resolve()
	 *
	 * @brief	Resolve collisions of primaryCollisionGroup and all attached collision groups
	 *
	 * @author	Julian
	 * @date	2018-01-25
	 */

	void resolve() {
		primaryCollisionGroup.resolve();

		for (const std::unique_ptr<InteractionGroup>& collisionGroup : interactionGroups) {
			collisionGroup->resolve();
		}
	}

	/**
	 * @fn	void Map::addObjectGroup(InteractionGroup& collisionGroup)
	 *
	 * @brief	Adds a collision group reference to the collisionsGroups vector
	 *
	 * @author	Julian
	 * @date	2018-01-25
	 *
	 * @param [in,out]	collisionGroup	Group the collision belongs to.
	 */

	void addObjectGroup(InteractionGroup& collisionGroup) {
		addObjectGroup(&collisionGroup);
	}

	void addObjectGroup(InteractionGroup* collisionGroup) {
		interactionGroups.emplace_back(collisionGroup);
	}

	void removeObjectGroup(InteractionGroup& collisionGroup) {
		removeObjectGroup(&collisionGroup);
	}

	void removeObjectGroup(InteractionGroup* collisionGroup) {
		auto it = std::find(interactionGroups.begin(), interactionGroups.end(), collisionGroup);

		if (it != interactionGroups.end()) {
			it->release();
			interactionGroups.erase(it);
		}
	}

	/**
	 * @fn	void Map::addObject(Body& physicsObject)
	 *
	 * @brief	Adds an object
	 *
	 * @author	Wiebe
	 * @date	30-1-2018
	 *
	 * @param [in,out]	physicsObject	The physics object.
	 */

	void addObject(Body& physicsObject) {
		addObject(&physicsObject);
	}

	/**
	 * @fn	void Map::addObject(Body* physicsObject)
	 *
	 * @brief	Adds an object
	 *
	 * @author	Wiebe
	 * @date	30-1-2018
	 *
	 * @param [in,out]	physicsObject	If non-null, the physics object.
	 */

	void addObject(Body* physicsObject) {
		emplace_back(physicsObject);
		objectAdded.fire(*physicsObject);
	}

	/**
	 * @fn	void Map::removeObject(Body& physicsObject)
	 *
	 * @brief	Removes the object from the map
	 *
	 * @author	Wiebe
	 * @date	30-1-2018
	 *
	 * @param [in,out]	physicsObject	The physics object.
	 */

	void removeObject(Body& physicsObject) {
		removeObject(&physicsObject);
	}

	/**
	 * @fn	void Map::removeObject(Body* physicsObject)
	 *
	 * @brief	Removes the object from the map
	 *
	 * @author	Wiebe
	 * @date	30-1-2018
	 *
	 * @param [in,out]	physicsObject	If non-null, the physics object.
	 */

	void removeObject(Body* physicsObject) {
		objectRemoving.fire(*physicsObject);

		auto it = std::find(begin(), end(), physicsObject);

		if (it != end()) {
			erase(it);
		}
		
		drawableGroup.erase(*physicsObject);
		primaryCollisionGroup.erase(*physicsObject);
	}
};