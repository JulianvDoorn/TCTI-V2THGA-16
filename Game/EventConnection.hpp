#pragma once

#include <functional>

template<class... Args>
class EventSource;

template<class... Args>
class EventConnection {
	uint32_t id;
	EventSource<Args...>* eventSource;

	EventConnection(uint32_t id, EventSource<Args...>& eventSource) : id(id), eventSource(&eventSource) { }

public:
	EventConnection() { }

	void disconnect() {
		eventSource->disconnect(*this);
	}

	EventConnection<Args...>& operator= (EventConnection<Args...> rhs) {
		id = rhs.id;
		eventSource = rhs.eventSource;

		return *this;
	}

	friend bool operator== (const EventConnection<Args...>& lhs, const EventConnection<Args...>& rhs) {
		return lhs.id == rhs.id;
	}

	friend EventSource<Args...>;
};

