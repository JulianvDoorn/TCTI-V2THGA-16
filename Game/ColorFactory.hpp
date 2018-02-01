#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>

#include "BaseStreamExceptions.hpp"
#include "QuotedString.hpp"
#include "SpecialCharacter.hpp"
#include "StreamManipulators.hpp"

/**
 * @class	MalformedColorStringException
 *
 * @brief	Exception for signalling when a color is malformed.
 *
 * @author	Julian
 * @date	2018-01-23
 */

class MalformedColorStringException : public StreamReadException {
public:
	explicit MalformedColorStringException(const std::string& s) : StreamReadException("Malformed/unknown color: \"" + s + "\"") { }
};

/**
 * @class	UnexpectedHexChar
 *
 * @brief	Thrown when an unexpected hexadecimal character has been read.
 * 			E.g. anything outside the following range: 0123456789ABCDEF
 *
 * @author	Julian
 * @date	2018-01-23
 */

class UnexpectedHexChar {
public:
	/** @brief	The erroneous character */
	char erroneousChar;

	/**
	 * @fn	explicit UnexpectedHexChar::UnexpectedHexChar(char c)
	 *
	 * @brief	Constructor
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param	c	The character.
	 */

	explicit UnexpectedHexChar(char c) : erroneousChar(c) { }

	/**
	 * @fn	operator UnexpectedHexChar::char() const
	 *
	 * @brief	Cast that converts the given  to a char
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @return	The result of the operation.
	 */

	operator char() const {
		return erroneousChar;
	}
};

/**
 * @class	UnexpectedHexCharException
 *
 * @brief	Exception for signalling unexpected hexadecimal character errors.
 *
 * @author	Julian
 * @date	2018-01-23
 */

class UnexpectedHexCharException : public StreamReadException {
public:
	explicit UnexpectedHexCharException(UnexpectedHexChar e, const std::string& s) : StreamReadException(std::string("Malformed hex char \"") + (char) e + "\" in \"" + s + "\"") { }
};

/**
 * @class	ConversionTable
 *
 * @brief	A map based conversion table template.
 * 			Doing conversions from L to R are generally more efficient than from R to L.
 * 			
 * @detailed @code
 * 			 ConversionTable<std::string, sf::Color> colorNameConversionTable = {
 * 				{ "red", sf::Color::Red },
 * 				{ "black", sf::Color::Black },
 * 				{ "blue", sf::Color::Blue },
 * 				{ "cyan", sf::Color::Cyan },
 * 				{ "green", sf::Color::Green },
 * 				{ "magenta", sf::Color::Magenta },
 * 				{ "transparent", sf::Color::Transparent },
 * 				{ "white", sf::Color::White },
 * 				{ "yellow", sf::Color::Yellow }
 * 			 };
 * 			 @endcode
 *
 * @author	Julian
 * @date	2018-01-23
 *
 * @tparam	L	Type of lhs
 * @tparam	R	Type of rhs
 */

template<class L, class R>

/**
 * @class	ConversionTable
 *
 * @brief	A conversion table.
 *
 * @author	Jeffrey
 * @date	1/31/2018
 */

class ConversionTable : public std::map<const L, const R> {
	using ConversionMap = std::map<const L, const R>;

public:
	template<class T>

	/**
	 * @struct	Conversion
	 *
	 * @brief	A conversion.
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 */

	struct Conversion {
		/** @brief	True if the operation was a success, false if it failed */
		bool success;
		T conversion;

		Conversion(bool success, T conversion) : success(success), conversion(conversion) { }

		operator T() const {
			return conversion;
		}

		operator T&() {
			return conversion;
		}
	};

	/**
	 * @fn	ConversionTable::ConversionTable(std::initializer_list<std::pair<const L, const R>> list)
	 *
	 * @brief	Constructor
	 *
	 * @author	Julian
	 * @date	2018-01-23
	 *
	 * @param	list	List of std::pair<const L, const R> to put inside the conversion map.
	 */

	ConversionTable(std::initializer_list<std::pair<const L, const R>> list) : ConversionMap(list) { }

	/**
	 * @fn	iterator ConversionTable::convertLeftToRight(const L& lhs)
	 *
	 * @brief	Converts the given L value to the corresponding R value.
	 * 			Returns the iterator for allowing to check if iterator == ConversionTable.end()
	 *
	 * @author	Julian
	 * @date	2018-01-23
	 *
	 * @param	L value to convert into R value
	 *
	 * @return	iterator
	 */

	Conversion<const R&> convert(const L& lhs) {
		auto it = ConversionMap::find(lhs);

		if (it != ConversionMap::end()) {
			return { true, it->second };
		} else {
			return { false, R() };
		}
	}

	/**
	* @fn	iterator ConversionTable::convertRightToLeft(const R& rhs)
	*
	* @brief	Converts the given R value to the corresponding L value.
	* 			Returns the iterator for allowing to check if iterator == ConversionTable.end()
	*
	* @author	Julian
	* @date		2018-01-23
	*
	* @param	R value to convert into L value
	*
	* @return	iterator
	*/

	Conversion<const L&> convert(const R& rhs) {
		typename ConversionMap::iterator it;
		
		for (it = ConversionMap::begin(); it != ConversionMap::end(); it++) {
			if (it->second == rhs) {
				return { true, it->first };
			}
		}
		
		return { false, L() };
	}
};

/**
 * @class	ColorFactory
 *
 * @brief	Color factory used for turning std::istream& input into colors, to turn std::string int colors and turn colors into std::strings.
 *
 * @author	Julian
 * @date	2018-01-23
 */

// TODO: try to work out if ColorFactory can inherit from BaseFactory

class ColorFactory {
	std::istream& input;
	
	/** @brief	Conversion table for turning colors into human-friendly string names and vice versa */
	static ConversionTable<std::string, sf::Color> colorNameConversionTable;

	/**
	 * @fn	static uint32_t ColorFactory::readHex(const char* begin, const char* end)
	 *
	 * @brief	Reads a hex string and turns it into a uint32_t
	 * 			
	 * @detailed Processes every char between const char* begin and const char* end.
	 *
	 * @author	Julian
	 * @date	2018-01-23
	 *
	 * @exception	UnexpectedHexChar	Thrown when an unexpected Hexadecimal Character error
	 *  condition occurs.
	 *
	 * @param	begin	Pointer to the begin of the string.
	 * @param	end  	Pointer to the end of the string.
	 *
	 * @return	The int representation of the given string.
	 */

	static uint32_t readHex(const char* begin, const char* end) {
		uint32_t buffer = 0;
		
		while (begin <= end) {
			if (*begin >= '0' && *begin <= '9') {
				buffer <<= 4;
				buffer += *begin - '0';
			} else if (*begin >= 'A' && *begin <= 'F') {
				buffer <<= 4;
				buffer += *begin - 'A' + 10;
			} else {
				throw UnexpectedHexChar(*begin);
			}
			
			begin++;
		}
		
		return buffer;
	}
	
public:

	/**
	 * @fn	ColorFactory::ColorFactory(std::istream& input)
	 *
	 * @brief	Constructor
	 *
	 * @author	Julian
	 * @date	2018-01-23
	 *
	 * @param [in,out]	input	Input stream to run getColor() on.
	 */

	ColorFactory(std::istream& input) : input(input) { }

	/**
	 * @fn	sf::Color ColorFactory::getColor()
	 *
	 * @brief	Retrieves a color from the input string given inside the constructor.
	 * 			
	 * @detailed Turns #........ into a color based on hex values.
	 * 			 Turns "......" into a color based on string values.
	 * 			 Else throws a MalformedColorStringException
	 *
	 * @author	Julian
	 * @date	2018-01-23
	 *
	 * @exception	MalformedColorStringException	Thrown when a Malformed Color String error
	 *  condition occurs.
	 *
	 * @return	The parsed sf::Color
	 */

	sf::Color getColor() {
		char c;
		input >> exceptions >> c;
		input.seekg(-1, std::ios::cur);
		
		if (c == '#') {
			std::string color;
			input >> exceptions >> color;
			return getColorFromString(color);
		} else if (c == '\"') {
			QuotedString color;
			input >> exceptions >> color;
			return getColorFromString(color);
		} else {
			std::string unexpectedSymbol;
			input >> exceptions >> unexpectedSymbol;
			throw MalformedColorStringException(unexpectedSymbol);
		}
	}

	/**
	 * @fn	static sf::Color ColorFactory::getColorFromString(const std::string& name)
	 *
	 * @brief	Gets color from string
	 *
	 * @author	Julian
	 * @date	2018-01-23
	 *
	 * @exception	MalformedColorStringException	Thrown when a Malformed Color String error
	 *  condition occurs.
	 * @exception	UnexpectedHexCharException   	Thrown when an Unexpected Hexadecimal Character
	 *  error condition occurs.
	 *
	 * @param	name	Name of the color to turn into sf::Color
	 *
	 * @return	Color of the string parsed
	 */

	static sf::Color getColorFromString(const std::string& name) {
		auto conversion = colorNameConversionTable.convert(name);
		
		if(conversion.success) {
		   return conversion;
		} else {
			if (name[0] != '#') {
				throw MalformedColorStringException(name);
			}
			
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
			
			try {
				if (name.size() == 7 || name.size() == 9) {
					r = (uint8_t) readHex(&name[1], &name[2]);
					g = (uint8_t) readHex(&name[3], &name[4]);
					b = (uint8_t) readHex(&name[5], &name[6]);
					
					if (name.size() == 9) {
						a = (uint8_t) readHex(&name[7], &name[8]);
					} else {
						a = 255;
					}
				} else {
					throw MalformedColorStringException(name);
				}
			} catch (const UnexpectedHexChar& e) {
				throw UnexpectedHexCharException(e, name);
			}
			
			return { r, g, b, a };
		}
	}

	/**
	 * @fn	static std::string ColorFactory::getStringFromColor(const sf::Color& color)
	 *
	 * @brief	Gets string from color
	 *
	 * @author	Julian
	 * @date	2018-01-23
	 *
	 * @param	color	The color to process into a string
	 *
	 * @return	string representation of either the hex format or the char string format 
	 */

	static std::string getStringFromColor(const sf::Color& color) {
		auto conversion = colorNameConversionTable.convert(color);

		if (conversion.success) {
			std::string str;
			str += SpecialCharacter::Quote;
			str += conversion;
			str += SpecialCharacter::Quote;
			return str;
		} else {
			std::ostringstream ss;
			ss << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (uint32_t) color.r;
			ss << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (uint32_t) color.g;
			ss << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (uint32_t) color.b;
			
			if (color.a < 255) {
				ss << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (uint32_t) color.a;
			}
			
			return "#" + ss.str();
		}
	}
};

/** @brief	The color factory color name conversion table */
ConversionTable<std::string, sf::Color> ColorFactory::colorNameConversionTable = {
		{ "red", sf::Color::Red },
		{ "black", sf::Color::Black },
		{ "blue", sf::Color::Blue },
		{ "cyan", sf::Color::Cyan },
		{ "green", sf::Color::Green },
		{ "magenta", sf::Color::Magenta },
		{ "transparent", sf::Color::Transparent },
		{ "white", sf::Color::White },
		{ "yellow", sf::Color::Yellow }
};