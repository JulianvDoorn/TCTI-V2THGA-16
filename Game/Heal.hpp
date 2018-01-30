#pragma once

#include "PowerUp.hpp"
#include "Player.hpp"

class Heal : public PowerUp {
	float value;

public:
	Heal(IntersectionGroup* powerUpIntersectionGroup) : PowerUp(powerUpIntersectionGroup) {
		setGravity({ 0, 0 });
		setSize({ 10, 10 });
	}

	void setHealValue(float value) {
		this->value = value;
	}

	void apply(Player& player) const override {
		std::cout << "healing" << std::endl;
		// heal player
	}
};