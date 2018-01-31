#pragma once

#include <fstream>

class Serializable {
protected:
	/**
	* @fn	virtual std::ofstream& Body::writeToFile(std::ofstream &of) = 0;
	*
	* @brief	Write an Body to an output file stream
	*
	* @author	Wiebe
	* @date	29-1-2018
	*
	* @param [in,out]	of	The output file stream
	*
	* @return	A reference to a std::ofstream.
	*/

	virtual std::ofstream& writeToFile(std::ofstream &of) const = 0;

public:
	friend std::ofstream& operator<< (std::ofstream& lhs, const Serializable& rhs) {
		rhs.writeToFile(lhs);
		return lhs;
	}
};