#pragma once

#include <functional>

template<class... Args>
class EventSource;

/**
 * @class	EventConnection
 *
 * @brief	An event connection.
 *
 * @author	Julian
 * @date	2018-01-19
 *
 * @tparam	Args	Type of the arguments that are passed into the lambda function stored in EventConnection<Args...>::EventSource<Args...>*
 */

template<class... Args>
class EventConnection {
	uint32_t id;
	EventSource<Args...>* eventSource;

	/**
	 * @fn	EventConnection::EventConnection(uint32_t id, EventSource<Args...>& eventSource)
	 *
	 * @brief	Constructor.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param 		  	id		   	The identifier for the lambda function stored in EventConnection<Args...>::EventSource<Args...>*.
	 * @param [in,out]	eventSource	The event source to disconnect from.
	 */

	EventConnection(uint32_t id, EventSource<Args...>& eventSource) : id(id), eventSource(&eventSource) { }

public:

	/**
	 * @fn	EventConnection::EventConnection()
	 *
	 * @brief	Default constructor.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 */

	EventConnection() { }

	/**
	 * @fn	void EventConnection::disconnect()
	 *
	 * @brief	Disconnects the lambda function given in EventSource<Args...>::connect(EventFunction) from the EventSource
	 * 			
	 * @code{.cpp}
	 * 			  EventSource<char> sampleEventSource;
	 *
	 * 			  sampleEventSource.connect([](char someChar) {
	 * 				std::cout << someChar << std::endl;
	 * 			  });
	 * 			  
	 * 			  EventConnection<char> conn = sampleEventSource.connect([](char someChar) {
	 * 				std::cout << ++someChar << std::endl;
	 * 			  });
	 *
	 * 			  sampleEventSource.fire('a'); // prints a and b
	 * 			  sampleEventSource.fire('b'); // prints b and c
	 * 			  sampleEventSource.fire('c'); // prints c and d
	 * 			  sampleEventSource.fire('d'); // prints d and e
	 * 			  
	 * 			  conn.disconnect();
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
	 * @exception	DisconnectedEventConnectionException	Thrown when a Disconnected Event
	 * 														Connection error condition occurs.
	 */

	void disconnect() {
		eventSource->disconnect(*this);
	}

	/**
	 * @fn	EventConnection<Args...>& EventConnection::operator= (EventConnection<Args...> rhs)
	 *
	 * @brief	Assignment operator. Allows for @code{.cpp} EventConnection<> eventConnection = otherEventConnection;  @endcode
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param	rhs	The right hand side.
	 *
	 * @return	A shallow copy of this object.
	 */

	EventConnection<Args...>& operator= (EventConnection<Args...> rhs) {
		id = rhs.id;
		eventSource = rhs.eventSource;

		return *this;
	}

	/**
	 * @fn	friend bool EventConnection::operator== (const EventConnection<Args...>& lhs, const EventConnection<Args...>& rhs)
	 *
	 * @brief	Equality operator.
	 * 			Returns lhs.id == rhs.id
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	lhs	The first instance to compare.
	 * @param [in,out]	rhs	The second instance to compare.
	 *
	 * @return	True if the parameters are considered equivalent.
	 */

	friend bool operator== (const EventConnection<Args...>& lhs, const EventConnection<Args...>& rhs) {
		return lhs.id == rhs.id;
	}

	friend EventSource<Args...>;
};

