#pragma once

#include "EventSignal.hpp"
#include "PhysicsObject.hpp"

class ShapeClickArguments : public EventArguments {
public:
	PhysicsObject& physicsObject;

	ShapeClickArguments(PhysicsObject& physicsObject) : physicsObject(physicsObject) { }
	ShapeClickArguments(const EventArguments& arguments) : physicsObject(static_cast<const ShapeClickArguments&>(arguments).physicsObject) { }
};

class ShapeClickSignal : public EventSignal {
	ShapeClickArguments shapeClickArguments;

public:
	ShapeClickSignal(PhysicsObject& physicsObject) : shapeClickArguments(physicsObject) { }

	const EventArguments& getArguments() const override {
		return shapeClickArguments;
	}
};