#pragma once

class SubtitleReadException : public std::exception {
private:
	const std::string filepath;

public:
	SubtitleReadException(const std::string filepath) : filepath(filepath) {};

	const char* what() const noexcept {
		return std::string("Error reading subtitle: " + filepath).c_str();
	}
};
