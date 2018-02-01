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
	/** @brief	The message */
	std::string msg;

public:

	/**
	 * @fn	SubtitleReadException::SubtitleReadException(const std::string& filepath) : msg(std::move(std::string("Error reading subtitle
	 *
	 * @brief	Constructor
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param	filepath	The filepath.
	 */

	SubtitleReadException(const std::string& filepath) : msg(std::move(std::string("Error reading subtitle: " + filepath).c_str())) {};

	const char* what() const noexcept {
		return msg.c_str();
	}
};
