#pragma once

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