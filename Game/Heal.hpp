#pragma once

#include "PowerUp.hpp"
#include "Player.hpp"

/**
 * @class	Heal
 *
 * @brief	A healing power-up
 *
 * @author	Jeffrey
 * @date	1/31/2018
 */

class Heal : public PowerUp {
	/** @brief	The value */
	float value;

public:

	/**
	 * @fn	Heal::Heal(IntersectionGroup* powerUpIntersectionGroup)
	 *
	 * @brief	Constructor
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param [in,out]	powerUpIntersectionGroup	If non-null, group the power up intersection
	 * 												belongs to.
	 */

	Heal(IntersectionGroup* powerUpIntersectionGroup) : PowerUp(powerUpIntersectionGroup) {
		setGravity({ 0, 0 });
		setSize({ 30, 30 });
	}

	/**
	 * @fn	void Heal::setHealValue(float value)
	 *
	 * @brief	Sets heal value
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param	value	The value.
	 */

	void setHealValue(float value) {
		this->value = value;
	}

	/**
	 * @fn	void Heal::apply(Player& player) const override
	 *
	 * @brief	Applies the healing to the player
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param [in,out]	player	The player.
	 */

	void apply(Player& player) const override {
		std::cout << "healing" << std::endl;
		player.healBodyParts();
	}
};