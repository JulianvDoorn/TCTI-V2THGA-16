#pragma once

#include <istream>
#include "IOExceptions.hpp"

/**
 * @class	SpecialCharacterType
 *
 * @brief	Special character type.
 * 			
 * @detailed When operator >> applied to it, it doesn't require a variable to store the read value in.
 * 			 It just has to be checked whether the read variable matches the required variable.
 *
 * @author	Julian
 * @date	2018-01-23
 *
 * @tparam	C	Type of the c.
 * @tparam	E	Type of the e.
 */

template<char C, class E>
class SpecialCharacterType {
public:

	/**
	 * @fn	SpecialCharacterType::SpecialCharacterType() noexcept =default;
	 *
	 * @brief	Default constructor
	 *
	 * @author	Julian
	 * @date	2018-01-23
	 */

	SpecialCharacterType() noexcept =default;

	/**
	 * @fn	static void SpecialCharacterType::check(std::istream& is)
	 *
	 * @brief	Reads from the given std::istream& whether the next c matches the SpecialCharacterType.
	 *
	 * @author	Julian
	 * @date	2018-01-23
	 *
	 * @exception	EOFException	Thrown when an EOF error condition occurs.
	 * @exception	E				Thrown when an unexpected symbol is found and the according exception is thrown.
	 *
	 * @param [in,out]	is	Input stream to read from
	 */

	static void check(std::istream& is) {
		char c;
		
		try {
			is >> exceptions >> c;
		} catch (const std::istream::failure&) {
			throw EOFException("Unexpected");
		}
		
		if (c != C) {
			int64_t p = is.tellg();
			is.seekg(-1, std::ios::cur);
			std::string unexpectedSymbol;
			
			if (is >> unexpectedSymbol) {
				throw E(p, unexpectedSymbol);
			}
			
			throw E(p);
		}
	}

	/**
	 * @fn	operator SpecialCharacterType::char() const
	 *
	 * @brief	Implicit cast that converts the given SpecialCharacterType to a char
	 *
	 * @author	Julian
	 * @date	2018-01-23
	 *
	 * @return	A char corresponding to the special char.
	 */

	operator char() const {
		return C;
	}
};

namespace SpecialCharacter {
	/** @brief	The left curly bracket { special character */
	const SpecialCharacterType<'{', OpeningBracketMissingException> LeftCurlyBracket;

	/** @brief	The right curly bracket } special character */
	const SpecialCharacterType<'}', ClosingBracketMissingException> RightCurlyBracket;

	/** @brief	The left bracket ( special character */
	const SpecialCharacterType<'(', OpeningBracketMissingException> LeftBracket;

	/** @brief	The right bracket ) special character */
	const SpecialCharacterType<')', ClosingBracketMissingException> RightBracket;

	/** @brief	The comma , special character */
	const SpecialCharacterType<',', CommaMissingException> Comma;

	/** @brief	The quote " special character */
	const SpecialCharacterType<'\"', QuoteMissingException> Quote;

	/** @brief	The hashtag # special character */
	const SpecialCharacterType<'#', HashtagMissingException> Hashtag;

	/** @brief	The assignment = special character */
	const SpecialCharacterType<'=', AssignmentMissingException> Assignment;
}

/**
 * @fn	template<char C, class E> std::istream& operator>>(std::istream& is, SpecialCharacterType<C, E> _)
 *
 * @brief	Stream extraction operator
 * 			
 * @detailed Runs the SpecialCharacterType<C, E>::check(std::istream&) in the given input stream.
 *
 * @author	Julian
 * @date	2018-01-23
 *
 * @tparam	C	Type of the c.
 * @tparam	E	Type of the e.
 * @param [in,out]	is	The is.
 * @param 		  	_ 	The.
 *
 * @return	std::istream& is
 */

template<char C, class E>
std::istream& operator>>(std::istream& is, SpecialCharacterType<C, E> _) {
	SpecialCharacterType<C, E>::check(is);
	return is;
}