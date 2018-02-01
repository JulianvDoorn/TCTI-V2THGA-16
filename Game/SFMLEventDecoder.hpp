#pragma once

/**
 * @class	SFMLEventDecoder
 *
 * @brief	A sfml event decoder.
 *
 * @author	Jeffrey
 * @date	2/1/2018
 */

class SFMLEventDecoder {

	/**
	 * @fn	virtual void SFMLEventDecoder::decodeSFMLEvent(sf::Event ev) const = 0;
	 *
	 * @brief	Decode sfml event into an EventSource signal.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param	ev	SFML event to decode into an EventSource signal.
	 */

	virtual void decodeSFMLEvent(sf::Event ev) = 0;
};