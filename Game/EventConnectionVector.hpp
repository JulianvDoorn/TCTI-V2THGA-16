#pragma once

#include <vector>

#include "EventSource.hpp"
#include "EventConnection.hpp"

template <class... Args>
class EventConnectionVector : public std::vector<EventConnection<Args...>> {
public:
	void connect(EventSource<Args...>& source, class EventSource<Args...>::EventFunction func) {
		std::vector<EventConnection<Args...>>::push_back(source.connect(func));
	}

	void disconnect() {
		for (EventConnection<Args...>& conn : *this) {
			conn.disconnect();
		}

		std::vector<EventConnection<Args...>>::clear();
	}
};