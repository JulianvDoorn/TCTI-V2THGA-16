#pragma once

#include <functional>
#include <stack>

#include "EventType.hpp"
#include "EventSignal.hpp"
#include "AllSignals.hpp"

using EventFunction = std::function<void(EventSignal&)>;

class EventHandler {
	std::map<EventType, std::stack<EventFunction>> boundFunctions;

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
		}
	}
};