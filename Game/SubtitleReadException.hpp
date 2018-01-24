#pragma once

class SubtitleReadException : public std::exception {
private:
	std::string msg;

public:
	SubtitleReadException(const std::string& filepath) : msg(std::move(std::string("Error reading subtitle: " + filepath).c_str())) {};

	const char* what() const noexcept {
		return msg.c_str();
	}
};
