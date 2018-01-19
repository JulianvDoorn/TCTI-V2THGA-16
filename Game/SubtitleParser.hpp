#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "SubtitleReadException.hpp"
#include "SubtitleItem.hpp"

typedef std::vector<std::shared_ptr<SubtitleItem>> SubtitleVector;

class SubtitleParser {
private:
	const std::string filepath;
	std::ifstream fileStream;

public:
	SubtitleParser(const std::string filepath) : filepath(filepath), fileStream(filepath) {};

	SubtitleVector& parse(SubtitleVector& subtitles) {
		if (!fileStream.is_open()) {
			throw SubtitleReadException(filepath);
		}

		std::string line, start, end, completeLine = "", timeLine = "";
		int subtitleNumber = 0, turn = 0;

		/*
		* turn = 0 -> Add subtitle number
		* turn = 1 -> Add string to timeLine
		* turn > 1 -> Add string to completeLine
		*/

		while (std::getline(fileStream, line)) {
			line.erase(remove(line.begin(), line.end(), '\r'), line.end());

			if (line.compare("")) {
				if (!turn) {
					subtitleNumber = atoi(line.c_str());
					turn++;
					continue;
				}

				if (line.find("-->") != std::string::npos) {
					timeLine += line;

					std::vector<std::string> srtTime;
					srtTime = split(timeLine, ' ', srtTime);

					start = srtTime[0];
					end = srtTime[2];
				}
				else {
					if (completeLine != "")
						completeLine += " "; // Add space

					completeLine += line;
				}

				turn++;
			}
			else {
				turn = 0;

				subtitles.push_back(std::make_shared<SubtitleItem>(subtitleNumber, completeLine, start, end));
				completeLine = timeLine = "";
			}

			if (fileStream.eof()) {
				subtitles.push_back(std::make_shared<SubtitleItem>(subtitleNumber, completeLine, start, end));
			}
		}

		// Close file
		fileStream.close();

		return subtitles;
	}

	inline std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;

		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}

		return elems;
	}

};