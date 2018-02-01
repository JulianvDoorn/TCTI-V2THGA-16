#pragma once

#include <exception>
#include <string>

/**
* @class	StatemachineException
*
* @brief	Exception for signalling statemachine errors.
*
* @author	Julian
* @date	2018-01-22
*/

class StatemachineException : public std::exception {
	/** @brief	The error */
	std::string err;

protected:

	/**
	 * @fn	StatemachineException::StatemachineException(const std::string& err) : err(std::move(err))
	 *
	 * @brief	Constructor
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param	err	The error.
	 */

	StatemachineException(const std::string& err) : err(std::move(err)) { }

public:

	/**
	 * @fn	virtual const char* StatemachineException::what() const noexcept override
	 *
	 * @brief	Gets the what
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @return	Null if it fails, else a pointer to a const char.
	 */

	virtual const char* what() const noexcept override {
		return err.c_str();
	}
};