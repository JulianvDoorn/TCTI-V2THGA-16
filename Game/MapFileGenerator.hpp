#pragma once

#include <fstream>
#include <SFML\Graphics.hpp>
#include "AssetManager.hpp"
#include "Rectangle.hpp"

/**
 * @class	MapFileGenerator
 *
 * @brief	A map file generator.
 *
 * @author	Wiebe
 * @date	29-1-2018
 */

class MapFileGenerator {
private:

	/** @brief	The output file stream */
	std::ofstream &outputFileStream;

	/**
	 * @fn	void MapFileGenerator::generateTexture(const std::string& id, const sf::Texture &t)
	 *
	 * @brief	Generates a texture
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 *
	 * @param	id	The texture identifier.
	 * @param	t 	A sf::Texture to process.
	 */

	void generateTexture(const std::string& id, const sf::Texture &t) {
		outputFileStream << "texture {\n";
		outputFileStream << " Id = \"" << id << "\"\n";
		outputFileStream << " Location = \"" << AssetManager::instance()->getFilename(id) << "\"\n";
		outputFileStream << " Repeated = ";
		if (t.isRepeated()) {
			outputFileStream << "true\n";
		}
		else {
			outputFileStream << "false\n";
		}

		outputFileStream << "}\n";
	}

	/**
	 * @fn	void MapFileGenerator::generateAsset(const std::string& id)
	 *
	 * @brief	Generates an asset listing in the map file
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 *
	 * @param	id	The identifier.
	 */

	void generateAsset(const std::string& id) {
		outputFileStream << "asset {\n";
		outputFileStream << " Id = \"" << id << "\"\n";
		outputFileStream << " Location = \"" << AssetManager::instance()->getFilename(id) << "\"\n";

		outputFileStream << "}\n";
	}

	/**
	 * @fn	void MapFileGenerator::generateGeneric(const Rectangle& rect, const std::string &id)
	 *
	 * @brief	Generates a generic listing in the map file
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 *
	 * @param	rect	The rectangle.
	 * @param	id  	The identifier.
	 */

	void generateGeneric(const Rectangle& rect, const std::string &id) {
		outputFileStream << id << " {\n";
		outputFileStream << " Position = Vector2(" << rect.getPosition().x << ", " << rect.getPosition().y << ")\n";
		outputFileStream << "}\n";
	}

public:

	/**
	 * @fn	MapFileGenerator::MapFileGenerator(std::ofstream &outputFileStream)
	 *
	 * @brief	Constructor
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 *
	 * @param [in,out]	outputFileStream	The output file stream.
	 */

	MapFileGenerator(std::ofstream &outputFileStream) :
		outputFileStream(outputFileStream)
	{};

	/**
	 * @fn	void MapFileGenerator::generate(std::string filename, Map &map, Rectangle &player, Rectangle &death)
	 *
	 * @brief	Generates an map file
	 *
	 * @author	Wiebe
	 * @date	29-1-2018
	 *
	 * @param 		  	filename	Filename of the file.
	 * @param [in,out]	map			The map.
	 * @param [in,out]	player  	The player.
	 * @param [in,out]	death   	The death.
	 */

	void generate(std::string filename, Map &map, Rectangle &player, Rectangle &death) {
		outputFileStream.open(filename);

		for (auto &asset : AssetManager::instance()->getSounds()) {
			generateAsset(asset.first);
		}

		for (auto &asset : AssetManager::instance()->getFonts()) {
			generateAsset(asset.first);
		}

		for (auto &t : AssetManager::instance()->getTextures()) {
			generateTexture(t.first, t.second);
		}

		generateGeneric(player, "player");
		generateGeneric(death, "death");

		for (const std::unique_ptr<PhysicsObject>& object : map) {
			try {
				object->writeToFile(outputFileStream);
			}
			catch (const char* s) {
				std::cout << s << std::endl;
			}
			
		}

		outputFileStream.close();
	}
};
