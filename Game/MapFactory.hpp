#pragma once

#include <istream>
#include <string>

#include "KeyValuePair.hpp"
#include "Map.hpp"
#include "AssetManager.hpp"
#include "Body.hpp"
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
	/** @brief	The input */
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
			std::string id;
			std::string location;

			properties.read({
				{ "Id", Type::String, [&](Value value) { id = value; if (location.size() > 0) AssetManager::instance()->load(id, location); } },
				{ "Location", Type::String, [&](Value value) { location = value; if (id.size() > 0) AssetManager::instance()->load(id, location); } }
			});

			if (id.size() == 0 || location.size() == 0) {
				// TODO: throw exception
			}
		});

		/** @brief	Register texture loader */
		registerCreateMethod("texture", [&](Map& map, const MapItemProperties& properties) {
			std::string id;
			std::string location;

			bool loaded = false;

			properties.read({
				{ "Id", Type::String, [&](Value value) { id = value; if (location.size() > 0) AssetManager::instance()->loadTexture(id, location); } },
				{ "Location", Type::String, [&](Value value) { location = value; if (id.size() > 0) AssetManager::instance()->loadTexture(id, location); } },
				{ "Repeated", Type::Bool, [&](Value value) { if (id.size() > 0) AssetManager::instance()->getTexture(id).setRepeated(value); } },
				{ "Smooth", Type::Bool, [&](Value value) { if (id.size() > 0) AssetManager::instance()->getTexture(id).setSmooth(value); } }
			});

			if (id.size() == 0 || location.size() == 0) {
				// TODO: throw exception
			}
		});

		/** @brief	Register rectangle loader */
		registerCreateMethod("rectangle", [&](Map& map, const MapItemProperties& properties) {
			Body* rectangle = new Body();

			bool canCollide = true;

			properties.read({
				{ "Color", Type::Color, [&](Value value) { rectangle->setFillColor(value); } },
				{ "Size", Type::Vector, [&](Value value) { rectangle->setSize(value); } },
				{ "Position", Type::Vector, [&](Value value) { rectangle->setPosition(value); } },
				{ "TextureId", Type::String, [&](Value value) { rectangle->setTexture(&AssetManager::instance()->getTexture(value)); } },
				{ "TextureRect", Type::Rect, [&](Value value) { rectangle->setTextureRect(value); } },
				{ "CanCollide", Type::Bool, [&](Value value) { canCollide = value; } }
			});

			map.addDrawable(rectangle);
			map.addObject(rectangle);

			if (canCollide) {
				map.addCollidable(rectangle);
			}
		});
	}
};