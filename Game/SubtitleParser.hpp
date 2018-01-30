#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "SubtitleReadException.hpp"
#include "SubtitleItem.hpp"

/**
 * @typedef	std::vector<std::shared_ptr<SubtitleItem>> SubtitleVector
 *
 * @brief	Defines an alias representing the subtitle vector
 */

typedef std::vector<std::shared_ptr<SubtitleItem>> SubtitleVector;

/**
 * @class	SubtitleParser
 *
 * @brief	A subtitle parser.
 *
 * @author	Wiebe
 * @date	24-1-2018
 */

class SubtitleParser {
private:


	/** @brief	Subtitle file filepath */
	const std::string filepath;


	/** @brief	The file stream */
	std::ifstream fileStream;

public:
	SubtitleParser(const std::string filepath) : filepath(filepath), fileStream(filepath) {};

	/**
	 * @fn	SubtitleVector& SubtitleParser::parse(SubtitleVector& subtitles)
	 *
	 * @brief	Parses an subtitle file and saves the individual SubtitleItem instances into an SubtitleVector 
	 *
	 * @author	Wiebe
	 * @date	24-1-2018
	 *
	 * @exception	SubtitleReadException	Thrown when a subtitle file read error occurres.
	 *
	 * @param [in,out]	subtitles SubtitleVector containg the individual SubtitleItem instances.
	 *
	 * @return	A reference to a SubtitleVector.
	 */

	SubtitleVector& parse(SubtitleVector& subtitles) {
		if (!fileStream.is_open()) {
			throw SubtitleReadException(filepath);
		}

		std::string line, start, end, completeLine = "", timeLine = "", imageLine = "";
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
				else if (line.find("#~") != std::string::npos) {
					imageLine += line;
					imageLine.erase(0, 2);
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

				subtitles.push_back(std::make_shared<SubtitleItem>(subtitleNumber, completeLine, imageLine, start, end));
				completeLine = timeLine = imageLine = "";
			}

			if (fileStream.eof()) {
				subtitles.push_back(std::make_shared<SubtitleItem>(subtitleNumber, completeLine, imageLine, start, end));
			}
		}

		// Close file
		fileStream.close();

		return subtitles;
	}

	/**
	 * @fn	inline std::vector<std::string>& SubtitleParser::split(const std::string &s, char delim, std::vector<std::string> &elems)
	 *
	 * @brief	Splits an subtitle using a delimeter.
	 *
	 * @author	Wiebe
	 * @date	24-1-2018
	 *
	 * @param 		  	s	 	A std::string subtitle to process.
	 * @param 		  	delim	The delimiter.
	 * @param [in,out]	elems	The parsed elements.
	 *
	 * @return	A reference to a std::vector&lt;std::string&gt;
	 */

	inline std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;

		while (getline(ss, item, delim)) {
			elems.push_back(item);
		}

		return elems;
	}

};