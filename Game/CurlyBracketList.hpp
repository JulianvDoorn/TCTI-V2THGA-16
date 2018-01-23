#pragma once

#include <string>
#include <sstream>
#include <istream>

std::istream& ignoreLine(std::istream& is)
{
	// skip until end-of-line
	is.ignore(std::numeric_limits<int>::max(), is.widen('\n'));
	return is;
}

template<class T>
class CurlyBracketList : public std::vector<T> { };

template<class T>
std::istream& operator>> (std::istream& is, CurlyBracketList<T>& list) {
	is >> SpecialCharacter::LeftCurlyBracket;
	is >> ignoreLine;

	while (true) {
		std::string line;
		getline(is, line);
		std::istringstream iss(line);

		char c;
		iss >> c;

		if (c != SpecialCharacter::RightCurlyBracket) {
			iss.seekg(0);
			list.push_back(T());
			iss >> list.back();
		}
		else {
			// } found
			break;
		}
	}

	return is;
}