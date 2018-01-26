#pragma once

/**
 * @class	SubtitleReadException
 *
 * @brief	Exception for signalling subtitle read errors.
 *
 * @author	Wiebe
 * @date	24-1-2018
 */

class SubtitleReadException : public std::exception {
private:
	std::string msg;

public:
	SubtitleReadException(const std::string& filepath) : msg(std::move(std::string("Error reading subtitle: " + filepath).c_str())) {};

	const char* what() const noexcept {
		return msg.c_str();
	}
};
