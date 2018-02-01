#pragma once

#include "Player.hpp"
#include "Body.hpp"
#include "Map.hpp"

/**
 * @class	PowerUp
 *
 * @brief	A power up.
 *
 * @author	Jeffrey
 * @date	1/31/2018
 */

class PowerUp : public Body {
	EventConnection collidedConn;
	IntersectionGroup* powerUpIntersectionGroup;

	bool used = false;

	void _apply(Player& player) {
		if (!used) {
			used = true;
			apply(player);
		}
	}
public:

	/**
	 * @fn	PowerUp::PowerUp(IntersectionGroup* powerUpIntersectionGroup)
	 *
	 * @brief	Constructor
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param [in,out]	powerUpIntersectionGroup	If non-null, group the power up intersection
	 * 												belongs to.
	 */

	PowerUp(IntersectionGroup* powerUpIntersectionGroup) : powerUpIntersectionGroup(powerUpIntersectionGroup) { }

	/**
	 * @fn	virtual void PowerUp::apply(Player& player) const = 0;
	 *
	 * @brief	Applies the power-upto the given player
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param [in,out]	player	The player.
	 */

	virtual void apply(Player& player) const = 0;

	/**
	 * @fn	void PowerUp::connect(Player& player, Map& map)
	 *
	 * @brief	Connects the power-up
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param [in,out]	player	The player.
	 * @param [in,out]	map   	The map.
	 */

	void connect(Player& player, Map& map) {
		collidedConn = collided.connect([&](Collidable& other) {
			_apply(player);

			map.eraseDrawable(*this);
		});
	}

	/**
	 * @fn	void PowerUp::disconnect()
	 *
	 * @brief	Disconnects this object
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 */

	void disconnect() {
		collidedConn.disconnect();
	}
};