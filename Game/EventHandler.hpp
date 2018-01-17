#pragma once

#include <functional>
#include <stack>
#include <vector>
#include <exception>

#include "EventType.hpp"
#include "EventSignal.hpp"
#include "AllSignals.hpp"

using EventFunction = std::function<void(EventSignal&)>;

class EventHandler {
	std::map<EventType, std::stack<EventFunction>> boundFunctions;
	std::vector<PhysicsObject*> objects;

public:
	void BindFunction(EventType type, EventFunction func) {
		if (boundFunctions.find(type) == boundFunctions.end()) {
			boundFunctions.insert(std::make_pair(type, std::stack<EventFunction>()));
		}

		boundFunctions.at(type).push(func);
	}

	void UnbindFunction(EventType type) {
		if (boundFunctions.find(type) != boundFunctions.end()) {
			boundFunctions.at(type).pop();
		}
	}

	void BindPhysicsObject(PhysicsObject* object) {
		objects.push_back(object);
	}

	void HandlePhysicsObjectsEvents(sf::Vector2i position, sf::Event ev) {
		for (PhysicsObject* object : objects) {
			if (object->getPosition().x >= position.x && object->getPosition().y <= position.y) {
				std::cout << "Object pressed!\n";
			}
		}
	}

	template<class T, class... Args>
	void FireEvent(EventType type, Args... args) {
		T eventSignal(args...);

		if (boundFunctions.find(type) != boundFunctions.end()) {
			std::stack<EventFunction>& bindingStack = boundFunctions.at(type);

			if (!bindingStack.empty()) {
				EventFunction& func = boundFunctions.at(type).top();
				func(eventSignal);
			}
		}
	}

	void HandleSFMLEvent(sf::Event ev) {
		switch (ev.type) {
		case sf::Event::KeyReleased:
			FireEvent<KeyUpSignal>(EventType::KeyUp, ev.key.code);
			return;
		case sf::Event::KeyPressed:
			FireEvent<KeyDownSignal>(EventType::KeyDown, ev.key.code);
			return;
		case sf::Event::MouseButtonPressed:
			if (ev.mouseButton.button == sf::Mouse::Right) {
				FireEvent<MouseClickSignal>(EventType::MouseRightPressed, sf::Vector2i(ev.mouseButton.x, ev.mouseButton.y));
			}
			else {
				FireEvent<MouseClickSignal>(EventType::MouseLeftPressed, sf::Vector2i(ev.mouseButton.x, ev.mouseButton.y));
			}
			return;
		case sf::Event::MouseButtonReleased:
			if (ev.mouseButton.button == sf::Mouse::Right) {
				FireEvent<MouseClickSignal>(EventType::MouseRightReleased, sf::Vector2i(ev.mouseButton.x, ev.mouseButton.y));
			}
			else {
				FireEvent<MouseClickSignal>(EventType::MouseLeftReleased, sf::Vector2i(ev.mouseButton.x, ev.mouseButton.y));
			}

			return;
		}
	}
};