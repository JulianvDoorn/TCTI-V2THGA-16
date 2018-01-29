#pragma once
#pragma once

#include <fstream>
#include <SFML/Graphics.hpp>
#include "AssetManager.hpp"
#include "Rectangle.hpp"
#include "RectangleContainer.hpp"

/*
rectangle {
Size = Vector2(6400, 200)
TextureRect = Rect(0, 0, 6400, 200)
Position = Vector2(0, 600)
TextureId = "rockFloor"
}
}*/

class ShapeFileGenerator {
private:
	std::ofstream &outputFileStream;

	void generateRectangle(const Rectangle &r) {
		outputFileStream << "rectangle {\n";
		outputFileStream << " Size = Vector2(" << r.getSize().x << ", " << r.getSize().y << ")\n";
		// left, top, width, height
		outputFileStream << " TextureRect = Rect(" << r.getTextureRect().left << ", " << r.getTextureRect().top << ", " << r.getTextureRect().width << ", " << r.getTextureRect().height  << ")\n";
		outputFileStream << " Position = Vector2(" << r.getPosition().x << ", " << r.getPosition().y << ")\n";
		outputFileStream << " TextureId = \"" << AssetManager::instance()->resolveTextureID(*r.getTexture()) << "\"\n";
		outputFileStream << " Color = #" << std::uppercase << std::hex << r.getFillColor().toInteger() << "\n";
		outputFileStream << " CanCollide = true\n";
		outputFileStream << "}\n";
	}


public:
	ShapeFileGenerator(std::ofstream &outputFileStream) :
		outputFileStream(outputFileStream)
	{};

	void generate(std::string filename, RectangleContainer &container) {
		outputFileStream.open(filename);

		for (const auto &rect : container) {
			generateRectangle(*rect);
		}

		outputFileStream.close();
	}
};
