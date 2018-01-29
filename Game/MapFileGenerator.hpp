#pragma once

#include <fstream>
#include <SFML\Graphics.hpp>
#include "AssetManager.hpp"
#include "Rectangle.hpp"
#include "RectangleContainer.hpp"


class MapFileGenerator {
private:
	std::ofstream &outputFileStream;

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

	void generateAsset(const std::string& id) {
		outputFileStream << "asset {\n";
		outputFileStream << " Id = \"" << id << "\"\n";
		outputFileStream << " Location = \"" << AssetManager::instance()->getFilename(id) << "\"\n";

		outputFileStream << "}\n";
	}

	void generateGeneric(const Rectangle& rect, const std::string &id) {
		outputFileStream << id << " {\n";
		outputFileStream << " Position = Vector2(" << rect.getPosition().x << ", " << rect.getPosition().y << ")\n";
		outputFileStream << "}\n";
	}

public:
	MapFileGenerator(std::ofstream &outputFileStream) :
		outputFileStream(outputFileStream)
	{};

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
