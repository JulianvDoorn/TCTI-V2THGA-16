#pragma once

#include <map>
#include <string>
#include <exception>
#include <memory>
#include <vector>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

class AssetTypeNotResolvedException : public std::exception {
private:
	const std::string& filename;

public:
	AssetTypeNotResolvedException(const std::string &filename) : filename(filename) {};

	const char* what() const noexcept {
		return std::string("Cannot determine asset type with filename '" + filename + "'!").c_str();
	}
};

class AssetNotFoundByPathException : public std::exception {
private:
	const std::string& filename;
	const std::string& type;

public:
	AssetNotFoundByPathException(const std::string &filename, const std::string &type) : filename(filename), type(type) {};

	const char* what() const noexcept {
		return std::string("Asset type with filename '" + filename + "' cannot been found!").c_str();
	}
};

class AssetNotLoadedException : public std::exception {
private:
	const std::string& id;
	
public:
	AssetNotLoadedException(const std::string& id) : id(id) {};

	const char* what() const noexcept {
		return std::string("Asset with id '" + id + "' is not loaded! Please load it first.").c_str();
	}
};

class AssetManager {
private:
	std::map<std::string, sf::Texture> textures;
	std::map<std::string, sf::Font> fonts;
	std::map<std::string, sf::Sound> sounds;
	std::vector < std::unique_ptr < sf::SoundBuffer> > soundBuffers;

public:
	void load(const std::string id, const std::string filename) {
		std::string::size_type idx;

		idx = filename.rfind('.');

		if (idx != std::string::npos) {
			std::string extension = filename.substr(idx + 1);

			if (extension == "png" || extension == "jpg" || extension == "jpeg" || extension == "gif") {
				loadTexture(id, filename);
			}
			else if (extension == "ttf" || extension == "otf") {
				loadFont(id, filename);
			}
			else if (extension == "wav") {
				loadSound(id, filename);
			}
			else {
				// Unknown asset extension
				throw AssetTypeNotResolvedException(filename);
			}
		}
		else {
			// Extension error
			throw AssetTypeNotResolvedException(filename);
		}
	}

	sf::Texture& loadTexture(const std::string id, const std::string filename) {
		sf::Texture _texture;

		if (_texture.loadFromFile(filename)) {
			textures[id] = _texture;

			return textures.at(id);
		}
		else {
			throw AssetNotFoundByPathException(filename, "texture");
		}
	}

	sf::Texture& getTexture(const std::string id) {
		try {
			return textures.at(id);
		}
		catch (const std::out_of_range) {
			throw AssetNotLoadedException(id);
		}
	}

	sf::Font& loadFont(const std::string id, const std::string filename) {
		sf::Font _font;

		if (_font.loadFromFile(filename)) {
			fonts[id] = _font;

			return fonts.at(id);
		}
		else {
			throw AssetNotFoundByPathException(filename, "font");
		}
	}

	sf::Font& getFont(const std::string id) {
		try {
			return fonts.at(id);
		}
		catch (const std::out_of_range) {
			throw AssetNotLoadedException(id);
		}
	}

	sf::Sound& loadSound(const std::string id, const std::string filename) {
		soundBuffers.push_back(std::make_unique<sf::SoundBuffer>());

		if (soundBuffers.back()->loadFromFile(filename)) {
			sf::Sound _sound;
			
			_sound.setBuffer(*soundBuffers.back());

			sounds[id] = _sound;

			return sounds.at(id);
		}
		else {
			throw AssetNotFoundByPathException(filename, "sound");
		}
	}

	sf::Sound& getSound(const std::string id) {
		try {
			return sounds.at(id);
		}
		catch (const std::out_of_range) {
			throw AssetNotLoadedException(id);
		}
	}

	void clear() {
		textures.clear();
		fonts.clear();
		sounds.clear();
	}
};