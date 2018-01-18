#pragma once

#include <functional>
#include <vector>

#include "EventConnection.hpp"

class DisconnectedEventConnectionException : public std::exception {
	const char* what() const noexcept override {
		return "You're trying to deconnect an already deconnected connection";
	}
};

template<class... Args>
class EventSource {
public:
	using EventFunction = std::function<void(Args...)>;

private:
	uint32_t idCounter = 0;

	struct EventBinding {
		EventConnection<Args...> conn;
		EventFunction func;

		EventBinding(EventFunction func, uint32_t id, EventSource<Args...>& eventSource) : conn(id, eventSource), func(std::move(func)) { }

		bool operator== (const EventConnection<Args...>& rhs) const {
			return conn == rhs;
		}
	};

	std::vector<EventBinding> boundFunctions;

	void disconnect(EventConnection<Args...>& conn) {
		auto it = std::find(boundFunctions.begin(), boundFunctions.end(), conn);
		
		if (it != boundFunctions.end()) {
			boundFunctions.erase(it);
		} else {
			throw DisconnectedEventConnectionException();
		}
	}

public:
	EventConnection<Args...> connect(EventFunction func) {
		boundFunctions.push_back(EventBinding(func, idCounter++, *this));

		return boundFunctions.back().conn;
	}
	
	void fire(Args... args) const {
		for (const EventBinding& binding : boundFunctions) {
			binding.func(args...);
		}
	}

	friend class EventConnection<Args...>;
};