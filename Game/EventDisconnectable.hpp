#pragma once

/**
 * @class	EventConnection
 *
 * @brief	Event connection.
 *
 * @author	Jeffrey
 * @date	1/31/2018
 */

class EventConnection;

/**
 * @class	EventDisconnectable
 *
 * @brief	Event disconnectable.
 *
 * @author	Jeffrey
 * @date	1/31/2018
 */

class EventDisconnectable {
protected:

	/**
	 * @fn	virtual void EventDisconnectable::disconnect(EventConnection& conn) = 0;
	 *
	 * @brief	Disconnects the given connection
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param [in,out]	conn	The Connection to disconnect.
	 */

	virtual void disconnect(EventConnection& conn) = 0;

	/**
	 * @class	EventConnection
	 *
	 * @brief	An event connection.
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 */

	friend class EventConnection;
};