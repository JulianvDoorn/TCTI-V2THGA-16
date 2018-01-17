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
	static std::vector<EventHandler*> activeEventHandlers;
	std::map<EventType, std::stack<EventFunction>> boundFunctions;
	std::vector<PhysicsObject*> physicsObjects;

public:
	EventHandler() { }

	static EventHandler& newEventHandler() {
		EventHandler* eventHandler = new EventHandler();
		activeEventHandlers.push_back(std::move(eventHandler));
		return *activeEventHandlers.back();
	}

	static void handleSFMLEvent(sf::Event ev) {
		switch (ev.type) {
		case sf::Event::KeyReleased:
			for (const EventHandler* eventHandler : activeEventHandlers) {
				eventHandler->fireEvent<KeyUpSignal>(EventType::KeyUp, ev.key.code);
			}
			return;
		case sf::Event::KeyPressed:
			for (const EventHandler* eventHandler : activeEventHandlers) {
				eventHandler->fireEvent<KeyDownSignal>(EventType::KeyDown, ev.key.code);
			}
			return;
		case sf::Event::MouseButtonPressed:
			if (ev.mouseButton.button == sf::Mouse::Right) {
				for (const EventHandler* eventHandler : activeEventHandlers) {
					eventHandler->fireEvent<MouseClickSignal>(EventType::MouseRightPressed, sf::Vector2i(ev.mouseButton.x, ev.mouseButton.y));
				}
			} else if (ev.mouseButton.button == sf::Mouse::Left) {
				for (const EventHandler* eventHandler : activeEventHandlers) {
					eventHandler->fireEvent<MouseClickSignal>(EventType::MouseLeftPressed, sf::Vector2i(ev.mouseButton.x, ev.mouseButton.y));
				}
			}
			return;
		case sf::Event::MouseButtonReleased:
			if (ev.mouseButton.button == sf::Mouse::Right) {
				for (const EventHandler* eventHandler : activeEventHandlers) {
					eventHandler->fireEvent<MouseClickSignal>(EventType::MouseRightReleased, sf::Vector2i(ev.mouseButton.x, ev.mouseButton.y));
				}
			} else if (ev.mouseButton.button == sf::Mouse::Left) {
				for (const EventHandler* eventHandler : activeEventHandlers) {
					eventHandler->fireEvent<MouseClickSignal>(EventType::MouseLeftReleased, sf::Vector2i(ev.mouseButton.x, ev.mouseButton.y));
				}
			}

			return;
		}
	}

	void bindPhysicsObject(PhysicsObject& bindingPhysicsObject) {
		physicsObjects.push_back(&bindingPhysicsObject);
	}

	void bindFunction(EventType type, EventFunction func) {
		if (boundFunctions.find(type) == boundFunctions.end()) {
			boundFunctions.insert(std::make_pair(type, std::move(std::stack<EventFunction>())));
		}

		boundFunctions.at(type).push(func);
	}

	void unbindFunction(EventType type) {
		if (boundFunctions.find(type) != boundFunctions.end()) {
			boundFunctions.at(type).pop();
		}
	}

	template<class T, class... Args>
	void fireEvent(EventType type, Args... args) const {
		T eventSignal(args...);
		
		if (boundFunctions.find(type) != boundFunctions.end()) {
			const std::stack<EventFunction>& bindingStack = boundFunctions.at(type);

			if (!bindingStack.empty()) {
				const EventFunction& func = boundFunctions.at(type).top();
				func(eventSignal);
			}
		}
	}
};

std::vector<EventHandler*> EventHandler::activeEventHandlers = std::vector<EventHandler*>();