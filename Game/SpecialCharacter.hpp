#pragma once

#include <istream>
#include "IOExceptions.hpp"

template<char C, class E>
class SpecialCharacterType {
public:
	SpecialCharacterType() noexcept =default;
	
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
	
	operator char() const {
		return C;
	}
};

namespace SpecialCharacter {
	const SpecialCharacterType<'{', OpeningBracketMissingException> LeftCurlyBracket;

	const SpecialCharacterType<'}', ClosingBracketMissingException> RightCurlyBracket;

	const SpecialCharacterType<'(', OpeningBracketMissingException> LeftBracket;
	const SpecialCharacterType<')', ClosingBracketMissingException> RightBracket;
	const SpecialCharacterType<',', CommaMissingException> Comma;
	const SpecialCharacterType<'\"', QuoteMissingException> Quote;
	const SpecialCharacterType<'#', HashtagMissingException> Hashtag;
	const SpecialCharacterType<'=', AssignmentMissingException> Assignment;
}
template<char C, class E>
std::istream& operator>>(std::istream& is, SpecialCharacterType<C, E> _) {
	SpecialCharacterType<C, E>::check(is);
	return is;
}