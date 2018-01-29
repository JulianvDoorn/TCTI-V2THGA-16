#pragma once

#include <fstream>
#include <SFML/Graphics.hpp>
#include "AssetManager.hpp"

/*
  texture {
	Id = "ground"
	Location = "ground.png"
	Repeated = true
}*/

class AssetFileGenerator {
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


public:
	AssetFileGenerator(std::ofstream &outputFileStream) :
		outputFileStream(outputFileStream)
	{};

	void generate(std::string filename) {
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

		outputFileStream.close();
	}
};
