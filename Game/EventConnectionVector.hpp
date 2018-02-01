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

	/**
	 * @fn	template <class... Args, class LAMBDA> void EventConnectionVector::connect(EventSource<Args...>& source, LAMBDA func)
	 *
	 * @brief	Connects
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @tparam	Args  	Type of the arguments.
	 * @tparam	LAMBDA	Type of the lambda.
	 * @param [in,out]	source	Source for the.
	 * @param 		  	func  	The function.
	 */

	template <class... Args, class LAMBDA>

	/**
	 * @fn	void public::connect(EventSource<Args...>& source, LAMBDA func)
	 *
	 * @brief	Connects
	 *
	 * @param [in,out]	source	Source for the.
	 * @param 		  	func  	The function.
	 */

	void connect(EventSource<Args...>& source, LAMBDA func) {
		std::vector<EventConnection>::push_back(source.connect(func));
	}

	/**
	 * @fn	void EventConnectionVector::disconnect()
	 *
	 * @brief	Disconnects this object
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 */

	void disconnect() {
		for (EventConnection& conn : *this) {
			conn.disconnect();
		}

		std::vector<EventConnection>::clear();
	}
};