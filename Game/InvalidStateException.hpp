#pragma once

#include "StatemachineException.hpp"

/**
* @class	InvalidStateException
*
* @brief	Exception for when a transition is requested to an invalid state. Usually non-existent states.
*
* @author	Julian
* @date	2018-01-19
*/

class InvalidStateException : public StatemachineException {
public:

	/**
	 * @fn	InvalidStateException::InvalidStateException()
	 *
	 * @brief	Default constructor
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 */

	InvalidStateException() : StatemachineException("Attempted to perform transition into an invalid state. Most likely an unregistered state.") { }
};