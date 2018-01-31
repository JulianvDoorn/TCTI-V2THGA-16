#pragma once

#include <functional>
#include <vector>
#include <deque>

#include "EventConnection.hpp"
#include "EventDisconnectable.hpp"

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
 * @class	LockGuard
 *
 * @brief	A lock guard.
 *
 * @author	Jeffrey
 * @date	1/31/2018
 */

class LockGuard {
	std::function<void()> releaseLambda;

public:
	template<class CLAIM>
	LockGuard(CLAIM claim, std::function<void()> release) : releaseLambda(std::move(release)) {
		claim();
	}

	~LockGuard() {
		releaseLambda();
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
class EventSource : public EventDisconnectable {
public:
	/** @brief	The event function. */
	using EventFunction = std::function<void(Args...)>;

private:
	/** @brief	True to lock, false to unlock */
	bool locked = false;
	/** @brief	The identifier counter */
	uint32_t idCounter = 0;

	/**
	 * @struct	EventBinding
	 *
	 * @brief	An event binding.
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 */

	struct EventBinding {
		/** @brief	The identifier */
		uint32_t id;
		/** @brief	The function */
		EventFunction func;

		/**
		 * @fn	EventBinding(EventFunction func, uint32_t id) : id(id), func(std::move(func))
		 *
		 * @brief	Constructor
		 *
		 * @author	Jeffrey
		 * @date	1/31/2018
		 *
		 * @param	func	The function.
		 * @param	id  	The identifier.
		 */

		EventBinding(EventFunction func, uint32_t id) : id(id), func(std::move(func)) { }

		/**
		 * @fn	EventBinding& operator= (const EventBinding& rhs)
		 *
		 * @brief	Assignment operator
		 *
		 * @author	Jeffrey
		 * @date	1/31/2018
		 *
		 * @param	rhs	The right hand side.
		 *
		 * @return	A shallow copy of this object.
		 */

		EventBinding& operator= (const EventBinding& rhs) {
			id = rhs.id;
			func = std::move(rhs.func);

			return *this;
		}

		/**
		 * @fn	bool operator== (const EventConnection& rhs) const
		 *
		 * @brief	Equality operator
		 * 			returnes rhs == id
		 *
		 * @author	Jeffrey
		 * @date	1/31/2018
		 *
		 * @param	rhs	The right hand side.
		 *
		 * @return	True if the parameters are considered equivalent.
		 */

		bool operator== (const EventConnection& rhs) const {
			return rhs == id;
		}
	};

	std::vector<EventBinding> boundFunctions;
	std::deque<EventBinding> queuedFunctions;

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

protected:

	/**
	 * @fn	void EventSource::disconnect(EventConnection& conn) override
	 *
	 * @brief	Disconnects the given conn
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @exception	DisconnectedEventConnectionException	Thrown when a Disconnected Event
	 * 														Connection error condition occurs.
	 *
	 * @param [in,out]	conn	The Connection to disconnect.
	 */

	void disconnect(EventConnection& conn) override {
		/** @brief	True if disconnected */
		bool disconnected = false;

		{
			auto it = std::find(boundFunctions.begin(), boundFunctions.end(), conn);

			if (it != boundFunctions.end()) {
				boundFunctions.erase(it);
				disconnected = true;
			}
		}

		{
			auto it = std::find(queuedFunctions.begin(), queuedFunctions.end(), conn);

			if (it != queuedFunctions.end()) {
				queuedFunctions.erase(it);
				disconnected = true;
			}
		}

		if (!disconnected) {
			//throw DisconnectedEventConnectionException();
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

	EventConnection connect(EventFunction func) {
		if (!locked) {
			boundFunctions.emplace_back(func, idCounter++);
			return EventConnection(boundFunctions.back().id, *this);
		}
		else {
			//std::cout << "event not yet connected; placed in queue" << std::endl;
			//std::cout << "before: " << queuedFunctions.size() << std::endl;
			queuedFunctions.emplace_back(func, idCounter++);
			//std::cout << "after: " << queuedFunctions.size() << std::endl;

			return EventConnection(queuedFunctions.back().id, *this);
		}
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

	void fire(Args... args) {
		LockGuard lockGuard (
			[&]() { locked = true; },
			[&]() { locked = false; }
		);

		//if (queuedFunctions.size() > 0) {
		//	std::cout << "flushing queue" << std::endl;
		//}

		while (queuedFunctions.size() > 0) {
			boundFunctions.push_back(queuedFunctions.front());
			queuedFunctions.pop_front();
		}

		for (const EventBinding& binding : boundFunctions) {
			binding.func(args...);
		}
	}
};