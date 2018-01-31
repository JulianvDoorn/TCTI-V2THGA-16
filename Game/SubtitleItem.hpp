#pragma once

#include <string>

#include "SubtitleTime.hpp"

/**
 * @class	SubtitleItem
 *
 * @brief	Holding information about one subtitle listing.
 *
 * @author	Wiebe
 * @date	24-1-2018
 */

class SubtitleItem {
private:

	/** @brief	The subtitle number */
	const int subtitleNumber;


	/** @brief	The subtitle text */
	std::string text;

	/**
	 * @property	std::string timeStart, timeEnd
	 *
	 * @brief	Subtitle start and end timers.
	 */

	SubtitleTime timeStart, timeEnd;

	std::string image;

public:

	/**
	* @fn	SubtitleItem::SubtitleItem(int subtitleNumber, std::string text, std::string timeStart, std::string timeEnd)
	*
	* @brief	Constructor
	*
	* @author	Wiebe
	* @date	24-1-2018
	*
	* @param	subtitleNumber	The subtitle number.
	* @param	text		  	The subtitle text.
	* @param	timeStart	  	The subtitle time start.
	* @param	timeEnd		  	The subtitle time end.
	*/

	SubtitleItem(int subtitleNumber, std::string text, std::string image, std::string timeStart, std::string timeEnd) :
		subtitleNumber(subtitleNumber),
		text(text),
		image(image),
		timeStart(timeStart),
		timeEnd(timeEnd)
	{ };

	/**
	* @fn	SubtitleItem::SubtitleItem(int subtitleNumber, std::string text)
	*
	* @brief	Constructor
	*
	* @author	Wiebe
	* @date	24-1-2018
	*
	* @param	subtitleNumber	The subtitle number.
	* @param	text		  	The subtitle text.
	*/

	SubtitleItem(int subtitleNumber, std::string text, std::string image) :
		subtitleNumber(subtitleNumber),
		text(text),
		image(image)
	{ };

	/**
	 * @fn	int SubtitleItem::getSubtitleNumber() const
	 *
	 * @brief	Gets subtitle number
	 *
	 * @author	Wiebe
	 * @date	24-1-2018
	 *
	 * @return	The subtitle number.
	 */

	int getSubtitleNumber() const {
		return subtitleNumber;
	}

	/**
	 * @fn	std::string SubtitleItem::getText() const
	 *
	 * @brief	Gets the subtitle text
	 *
	 * @author	Wiebe
	 * @date	24-1-2018
	 *
	 * @return	The subtitle text.
	 */

	std::string getText() const {
		return text;
	}

	/**
	 * @fn	std::string SubtitleItem::getTimeStart() const
	 *
	 * @brief	Gets subtitle time start
	 *
	 * @author	Wiebe
	 * @date	24-1-2018
	 *
	 * @return	The subtitle time start.
	 */

	SubtitleTime getTimeStart() const {
		return timeStart;
	}

	/**
	 * @fn	std::string SubtitleItem::getTimeEnd() const
	 *
	 * @brief	Gets subtitloe time end
	 *
	 * @author	Wiebe
	 * @date	24-1-2018
	 *
	 * @return	The subtitle time end.
	 */

	SubtitleTime getTimeEnd() const {
		return timeEnd;
	}

	std::string getImagePath() const {
		return image;
	}
};