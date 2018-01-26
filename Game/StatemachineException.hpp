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
	std::string err;

protected:
	StatemachineException(const std::string& err) : err(std::move(err)) { }

public:
	virtual const char* what() const noexcept override {
		return err.c_str();
	}
};