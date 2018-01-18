#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "PhysicsObject.hpp"

class CollisionObjects {
private:
	std::vector<PhysicsObject*> objects;
public:
	CollisionObjects() {
		objects = {};
	}

	auto getSize() {
		return objects.size();
	}

	auto at(int posision) {
		return objects.at(posision);
	}

	void add(PhysicsObject &object) {
		objects.push_back(&object);
	}

	PhysicsObject* get(int position) {
		return objects[position];
	}

	//Rectangle is_in() {}
};