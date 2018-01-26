#pragma once

#include <functional>
#include <vector>

#include "EventConnection.hpp"

/**
 * @class	DisconnectedEventConnectionException
 *
 * @brief	Exception for when an EventConnection<Args...> was tried to be disconnected but already is disconnected.
 *
 * @author	Julian
 * @date	2018-01-19
 */

class DisconnectedEventConnectionException : public std::exception {
	const char* what() const noexcept override {
		return "You're trying to deconnect an already deconnected connection";
	}
};

/**
 * @class	EventSource
 *
 * @brief	An event source.
 *
 * @author	Julian
 * @date	2018-01-19
 *
 * @tparam	Args...	Type of the arguments that are passed into the connected lambda functions.
 */

template<class... Args>
class EventSource {
public:
	/** @brief	The event function. */
	using EventFunction = std::function<void(Args...)>;

private:
	uint32_t idCounter = 0;

	struct EventBinding {
		uint32_t id;
		EventFunction func;

		EventBinding(EventFunction func, uint32_t id) : id(id), func(std::move(func)) { }

		bool operator== (const EventConnection<Args...>& rhs) const {
			return rhs == id;
		}
	};

	std::vector<EventBinding> boundFunctions;

	/**
	 * @fn	void EventSource::disconnect(EventConnection<Args...>& conn)
	 *
	 * @brief	Disconnects the given EventConnection<Args...>&.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @exception	DisconnectedEventConnectionException	Thrown when a Disconnected Event
	 * 														Connection error condition occurs.
	 *
	 * @param [in,out]	conn	The Connection to disconnect.
	 */

	void disconnect(EventConnection<Args...>& conn) {
		auto it = std::find(boundFunctions.begin(), boundFunctions.end(), conn);
		
		if (it != boundFunctions.end()) {
			boundFunctions.erase(it);
		} else {
			throw DisconnectedEventConnectionException();
		}
	}

public:

	/**
	 * @fn	EventConnection<Args...> EventSource::connect(EventFunction func)
	 *
	 * @brief	Connects the given function.
	 * 			
	 * @code{.cpp}
	 * 			  EventSource<char> sampleEventSource;
	 * 			  
	 * 			  sampleEventSource.connect([](char someChar) {
	 * 				std::cout << someChar << std::endl;
	 * 			  });
	 * 			  
	 * 			  sampleEventSource.fire('a'); // prints a
	 * 			  sampleEventSource.fire('b'); // prints b
	 * 			  sampleEventSource.fire('c'); // prints c
	 * 			  sampleEventSource.fire('d'); // prints d
	 * @endcode
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param	func	The Function to connect.
	 *
	 * @return	An EventConnection<Args...>;
	 */

	EventConnection<Args...> connect(EventFunction func) {
		boundFunctions.push_back(EventBinding(func, idCounter++));

		return EventConnection<Args...>(boundFunctions.back().id, *this);
	}

	/**
	 * @fn	void EventSource::fire(Args... args) const
	 *
	 * @brief	Fires all bound functions with the given arguments
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param	args	Variable arguments providing the arguments.
	 */

	void fire(Args... args) const {
		for (const EventBinding& binding : boundFunctions) {
			binding.func(args...);
		}
	}

	friend class EventConnection<Args...>;
};