#pragma once

#include <istream>
#include <sstream>
#include <iomanip>
#include <ctime>

class SubtitleTime {
	float time;

public:
	SubtitleTime() : time(0) { }

	SubtitleTime(const std::string& str) : time(0) {
		std::tm timeStruct;
		std::istringstream(str) >> std::get_time(&timeStruct, "%H:%M:%S");

		time += timeStruct.tm_sec;
		time += timeStruct.tm_min * 60;
		time += timeStruct.tm_hour * 3600;
	}

	operator float() const {
		return time;
	}

	operator float&() {
		return time;
	}
};