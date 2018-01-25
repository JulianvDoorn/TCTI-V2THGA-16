#pragma once

#include <map>
#include <string>
#include <exception>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

/**
 * @class	AssetTypeNotResolvedException
 *
 * @brief	Exception for signalling when an asset type could not been resolved.
 *
 * @author	Wiebe
 * @date	24-1-2018
 */

class AssetTypeNotResolvedException : public std::exception {
private:
	std::string msg;

public:
	AssetTypeNotResolvedException(const std::string &filename) : msg("Cannot determine asset type with filename '" + filename + "'!") {};

	const char* what() const noexcept {
		return msg.c_str();
	}
};

/**
 * @class	AssetNotFoundByPathException
 *
 * @brief	Exception for signalling when an asset is not found by the given path.
 *
 * @author	Wiebe
 * @date	24-1-2018
 */

class AssetNotFoundByPathException : public std::exception {
private:
	std::string msg;

public:
	AssetNotFoundByPathException(const std::string &filename, const std::string &type) : msg("Asset type with filename '" + filename + "' and type '" + type + "' cannot been found!") {};

	const char* what() const noexcept {
		return msg.c_str();
	}
};

/**
 * @class	AssetNotLoadedException
 *
 * @brief	Exception for signalling when an asset was not loaded into the AssetManager.
 *
 * @author	Wiebe
 * @date	24-1-2018
 */

class AssetNotLoadedException : public std::exception {
private:
	std::string msg;
	
public:
	AssetNotLoadedException(const std::string& id) : msg("Asset with id '" + id +  "' is not loaded! Please load it first.") {};

	const char* what() const noexcept {
		return msg.c_str();
	}
};

/**
 * @class	AssetManager
 *
 * @brief	Manager for assets.
 *
 * @author	Wiebe
 * @date	24-1-2018
 */

class AssetManager {
private:


	/** @brief	The textures */
	std::map<std::string, sf::Texture> textures;

	/** @brief	The fonts */
	std::map<std::string, sf::Font> fonts;

	/** @brief	The sounds */
	std::map<std::string, sf::Sound> sounds;

	/**
	 * @property	std::vector < std::unique_ptr < sf::SoundBuffer> > soundBuffers
	 *
	 * @brief	Gets the sound buffers
	 *
	 * @return	The sound buffers.
	 */

	std::vector < std::unique_ptr < sf::SoundBuffer> > soundBuffers;

	static AssetManager* sInstance;

public:

	/**
	 * @fn	static AssetManager* AssetManager::instance()
	 *
	 * @brief	Get an instance of AssetManager. This instance will always be the same in the entire application (aka an Singleton).
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @return	Pointer to an AssetManager.
	 */

	static AssetManager* instance()
	{
		if (!sInstance)
			sInstance = new AssetManager();
		return sInstance;
	}

	/**
	 * @fn	AssetManager & AssetManager::operator=(const AssetManager &rhs)
	 *
	 * @brief	Assignment operator
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @param	rhs	The right hand side.
	 *
	 * @return	A shallow copy of this object.
	 */

	AssetManager & operator=(const AssetManager &rhs) {
		textures = std::move(rhs.textures);
		fonts = std::move(rhs.fonts);
		sounds = std::move(rhs.sounds);

		for (const auto& e : rhs.soundBuffers)
			soundBuffers.push_back(std::make_unique<sf::SoundBuffer>(*e));

		return *this;
	}

	/**
	 * @fn	void AssetManager::load(const std::string id, const std::string filename)
	 *
	 * @brief	Load an asset.
	 * 
	 * @detailed Matches an file extension with an appropiate asset. Throws
	 * 			 an AssetTypeNotResolvedException when this can't be done.
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @exception	AssetTypeNotResolvedException	Thrown when an Asset Type Not Resolved error
	 * 												condition occurs.
	 *
	 * @param	id			The asset identifier.
	 * @param	filename	Filename of the asset.
	 */

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

	/**
	 * @fn	void AssetManager::loadTexture(const std::string id, const std::string filename)
	 *
	 * @brief	Loads a texture
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @exception	AssetNotFoundByPathException	Thrown when an asset could not be found by the given filename.
	 * 
	 * @param	id			The identifier.
	 * @param	filename	Filename of the file.
	 */

	void loadTexture(const std::string id, const std::string filename) {
		if (textures.find(id) != textures.end()) {
			return;
		}

		sf::Texture _texture;

		if (_texture.loadFromFile(filename)) {
			textures[id] = _texture;
		}
		else {
			throw AssetNotFoundByPathException(filename, "texture");
		}
	}

	/**
	 * @fn	sf::Texture& AssetManager::getTexture(const std::string id)
	 *
	 * @brief	Gets a texture
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @exception	AssetNotLoadedException	Thrown when an asset was not preloaded into the AssetManager.
	 *
	 * @param	id	The asset identifier.
	 *
	 * @return	The texture.
	 */

	sf::Texture& getTexture(const std::string id) {
		try {
			return textures.at(id);
		}
		catch (const std::out_of_range) {
			throw AssetNotLoadedException(id);
		}
	}

	/**
	 * @fn	void AssetManager::loadFont(const std::string id, const std::string filename)
	 *
	 * @brief	Loads a font
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @exception	AssetNotFoundByPathException	Thrown when an asset could not be found by the given filename.
	 *
	 * @param	id			The asset identifier.
	 * @param	filename	Filename of the file.
	 */

	void loadFont(const std::string id, const std::string filename) {
		if (fonts.find(id) != fonts.end()) {
			return;
		}

		sf::Font _font;

		if (_font.loadFromFile(filename)) {
			fonts[id] = _font;
		}
		else {
			throw AssetNotFoundByPathException(filename, "font");
		}
	}

	/**
	 * @fn	sf::Font& AssetManager::getFont(const std::string id)
	 *
	 * @brief	Gets a font
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @exception	AssetNotLoadedException	Thrown when an asset was not preloaded into the AssetManager.
	 *
	 * @param	id	The asset identifier.
	 *
	 * @return	The font.
	 */

	sf::Font& getFont(const std::string id) {
		try {
			return fonts.at(id);
		}
		catch (const std::out_of_range) {
			throw AssetNotLoadedException(id);
		}
	}

	/**
	 * @fn	void AssetManager::loadSound(const std::string id, const std::string filename)
	 *
	 * @brief	Loads a sound
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @exception	AssetNotFoundByPathException	Thrown when an asset could not be found by the given filename.
	 *
	 * @param	id			The asset identifier.
	 * @param	filename	Filename of the file.
	 */

	void loadSound(const std::string id, const std::string filename) {
		if (sounds.find(id) != sounds.end()) {
			return;
		}

		soundBuffers.push_back(std::move(std::make_unique<sf::SoundBuffer>()));

		if (soundBuffers.back()->loadFromFile(filename)) {
			sf::Sound _sound;
			
			_sound.setBuffer(*soundBuffers.back());

			sounds[id] = _sound;
		}
		else {
			throw AssetNotFoundByPathException(filename, "sound");
		}
	}

	/**
	 * @fn	sf::Sound& AssetManager::getSound(const std::string id)
	 *
	 * @brief	Gets a sound
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 *
	 * @exception	AssetNotLoadedException	Thrown when an asset was not preloaded into the AssetManager.
	 *
	 * @param	id	The asset identifier.
	 *
	 * @return	The sound.
	 */

	sf::Sound& getSound(const std::string id) {
		try {
			return sounds.at(id);
		}
		catch (const std::out_of_range) {
			throw AssetNotLoadedException(id);
		}
	}

	/**
	 * @fn	void AssetManager::clear()
	 *
	 * @brief	Clears all preloaded assets.
	 *
	 * @author	Wiebe
	 * @date	25-1-2018
	 */

	void clear() {
		textures.clear();
		fonts.clear();
		sounds.clear();
	}
};

/** @brief	The asset manager singleton instance */
AssetManager* AssetManager::sInstance = nullptr;