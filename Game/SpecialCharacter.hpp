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

class SpecialCharacter {
public:
	const static SpecialCharacterType<'{', OpeningBracketMissingException> LeftCurlyBracket;
	const static SpecialCharacterType<'}', ClosingBracketMissingException> RightCurlyBracket;
	const static SpecialCharacterType<'(', OpeningBracketMissingException> LeftBracket;
	const static SpecialCharacterType<')', ClosingBracketMissingException> RightBracket;
	const static SpecialCharacterType<',', CommaMissingException> Comma;
	const static SpecialCharacterType<'\"', QuoteMissingException> Quote;
	const static SpecialCharacterType<'#', HashtagMissingException> Hashtag;
	const static SpecialCharacterType<'=', HashtagMissingException> Assignment;
};

const SpecialCharacterType<'{', OpeningBracketMissingException> SpecialCharacter::LeftCurlyBracket;
const SpecialCharacterType<'}', ClosingBracketMissingException> SpecialCharacter::RightCurlyBracket;
const SpecialCharacterType<'(', OpeningBracketMissingException> SpecialCharacter::LeftBracket;
const SpecialCharacterType<')', ClosingBracketMissingException> SpecialCharacter::RightBracket;
const SpecialCharacterType<',', CommaMissingException> SpecialCharacter::Comma;
const SpecialCharacterType<'\"', QuoteMissingException> SpecialCharacter::Quote;
const SpecialCharacterType<'#', HashtagMissingException> SpecialCharacter::Hashtag;
const SpecialCharacterType<'=', HashtagMissingException> SpecialCharacter::Assignment;

template<char C, class E>
std::istream& operator>>(std::istream& is, SpecialCharacterType<C, E> _) {
	SpecialCharacterType<C, E>::check(is);
	return is;
}