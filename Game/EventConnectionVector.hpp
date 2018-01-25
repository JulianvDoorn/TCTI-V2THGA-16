#pragma once

#include <vector>

#include "EventSource.hpp"
#include "EventConnection.hpp"

/**
 * @class	EventConnectionVector
 *
 * @brief	An event connection vector.
 * 	
 * The EventConnectionVector class manages a list of event connections of the same types.
 *
 * @author	Julian
 * @date	2018-01-25
 *
 * @tparam	Args	Type of the arguments.
 */

class EventConnectionVector : public std::vector<EventConnection> {
public:
	template <class... Args, class LAMBDA>
	void connect(EventSource<Args...>& source, LAMBDA func) {
		std::vector<EventConnection>::push_back(source.connect(func));
	}

	void disconnect() {
		for (EventConnection& conn : *this) {
			conn.disconnect();
		}

		std::vector<EventConnection>::clear();
	}
};