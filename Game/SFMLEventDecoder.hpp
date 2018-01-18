#pragma once

class SFMLEventDecoder {
	virtual void decodeSFMLEvent(sf::Event ev) const = 0;
};