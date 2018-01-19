#pragma once

#include <string>

class SubtitleItem {
private:
	const int subtitleNumber;
	std::string text, timeStart, timeEnd;

public:
	SubtitleItem(int subtitleNumber, std::string text, std::string timeStart, std::string timeEnd) :
		subtitleNumber(subtitleNumber),
		text(text),
		timeStart(timeStart),
		timeEnd(timeEnd)
	{};

	int getSubtitleNumber() const {
		return subtitleNumber;
	}

	std::string getText() const {
		return text;
	}

	std::string getTimeStart() const {
		return timeStart;
	}

	std::string getTimeEnd() const {
		return timeEnd;
	}
};