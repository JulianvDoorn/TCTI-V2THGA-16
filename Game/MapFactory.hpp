#pragma once

#include <istream>
#include <string>

#include "KeyValuePair.hpp"
#include "Map.hpp"
#include "AssetManager.hpp"
#include "Rectangle.hpp"
#include "MapItemProperties.hpp"
#include "StreamManipulators.hpp"

/**
 * @class	MapFactory
 *
 * @brief	A map factory.
 * 	
 * @detailed MapFactory is used for reading a map out of a file that contains a list of dictionary styled items.
 * 			 It follows this format continuously until an exception is thrown or EOF is detected.
 * 			 asset SomeIdentifier {
 * 				SomeKey = "someValue"
 * 				SomeOtherKey = 42
 * 				FinalKey = Vector2(10, 10)
 * 			 }
 *
 * @author	Julian
 * @date	2018-01-25
 */

class MapFactory : public BaseFactory<void, std::string, Map&, const MapItemProperties&> {
	std::istream& input;

public:
	/** @brief	Retrieve KeyValuePair::Type into local scope */
	using Type = KeyValuePair::Type;

	/** @brief	Retrieve KeyValuePair::Value into local scope */
	using Value = KeyValuePair::Value;

	/**
	 * @fn	Map MapFactory::buildMap()
	 *
	 * @brief	Reads all the content from std::stream& input and puts it data converted into workable data in a map.
	 *
	 * @author	Julian
	 * @date	2018-01-25
	 *
	 * @return	Newly generated map.
	 */

	Map buildMap() {
		Map map;

		while (!input.eof()) {
			std::string name;
			input >> exceptions >> name;

			MapItemProperties mapItemProperties;
			input >> exceptions >> mapItemProperties;

			create(name, map, mapItemProperties);
			input >> std::ws;
		}

		return map;
	}

	/**
	 * @fn	MapFactory::MapFactory(std::istream& is)
	 *
	 * @brief	Constructor
	 *
	 * @author	Julian
	 * @date	2018-01-25
	 *
	 * @param [in,out]	is	Input stream to read from.
	 */

	MapFactory(std::istream& is) : input(is) {
		/** @brief	Register asset loader */
		registerCreateMethod("asset", [&](Map& map, const MapItemProperties& properties) {
			std::string* id = nullptr;
			std::string* location = nullptr;

			properties.read({
				{ "Id", Type::String, [&](Value value) { id = value.stringValue; if (location != nullptr) AssetManager::instance()->load(*id, *location); } },
				{ "Location", Type::String, [&](Value value) { location = value.stringValue; if (id != nullptr) AssetManager::instance()->load(*id, *location); } }
			});

			if (id == nullptr || location == nullptr) {
				// TODO: throw exception
			}
		});

		/** @brief	Register texture loader */
		registerCreateMethod("texture", [&](Map& map, const MapItemProperties& properties) {
			const std::string* id = nullptr;
			const std::string* location = nullptr;

			bool loaded = false;

			properties.read({
				{ "Id", Type::String, [&](Value value) { id = value.stringValue; if (location != nullptr) AssetManager::instance()->loadTexture(*id, *location); } },
				{ "Location", Type::String, [&](Value value) { location = value.stringValue; if (id != nullptr) AssetManager::instance()->loadTexture(*id, *location); } },
				{ "Repeated", Type::Bool, [&](Value value) { if (id != nullptr) AssetManager::instance()->getTexture(*id).setRepeated(value.b); } },
				{ "Smooth", Type::Bool, [&](Value value) { if (id != nullptr) AssetManager::instance()->getTexture(*id).setSmooth(value.b); } }
			});

			if (id == nullptr || location == nullptr) {
				// TODO: throw exception
			}
		});

		/** @brief	Register rectangle loader */
		registerCreateMethod("rectangle", [&](Map& map, const MapItemProperties& properties) {
			Rectangle* rectangle = new Rectangle();

			bool canCollide = true;

			properties.read({
				{ "Color", Type::Color, [&](Value value) { rectangle->setFillColor(*value.colorValue); } },
				{ "Size", Type::Vector, [&](Value value) { rectangle->setSize(*value.vectorValue); } },
				{ "Position", Type::Vector, [&](Value value) { rectangle->setPosition(*value.vectorValue); } },
				{ "TextureId", Type::String, [&](Value value) { rectangle->setTexture(AssetManager::instance()->getTexture(*value.stringValue)); } },
				{ "TextureRect", Type::Rect, [&](Value value) { rectangle->setTextureRect(static_cast<sf::IntRect>(*value.rectValue)); } },
				{ "CanCollide", Type::Bool, [&](Value value) { canCollide = value.b; } }
			});

			map.addDrawable(rectangle);

			if (canCollide) {
				map.addCollidable(rectangle);
			}
		});
	}
};