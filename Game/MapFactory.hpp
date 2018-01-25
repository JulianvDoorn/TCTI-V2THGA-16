#pragma once

#include "BaseFactory.hpp"
#include <istream>
#include <utility>
#include <memory>
#include <functional>
#include "Ball.hpp"
#include "IOExceptions.hpp"
#include "Rectangle.hpp"
#include "ColorFactory.hpp"
#include "VectorMath.hpp"
#include "KeyValuePair.hpp"
#include "CurlyBracketList.hpp"
#include "CollisionDetection.hpp"
#include "DrawableGroup.hpp"

class KeyDefinition {
public:
	const std::string name;
	const KeyValuePair::Type type;
	const std::function<void(KeyValuePair::Value)> func;

	KeyDefinition(const std::string& name, const KeyValuePair::Type type, const std::function<void(KeyValuePair::Value)>& func) : name(std::move(name)), type(type), func(std::move(func)) { }

	bool operator== (const std::string& rhs) const {
		return name == rhs;
	}
};

class Map : public BaseFactory<void, std::string, std::istream&> {
public:
	using Type = KeyValuePair::Type;
	using Value = KeyValuePair::Value;

	static void readDefinitions(const std::vector<KeyValuePair>& vector, std::vector<KeyDefinition> definitions) {
		for (const KeyValuePair& pair : vector) {
			auto it = std::find(definitions.begin(), definitions.end(), pair.key);

			if (it != definitions.end()) {
				if (it->type == pair.type) {
					it->func(pair.value);
				}
			}
		}
	}

	DrawableGroup drawableGroup;
	CollisionGroup collisionGroup;

	void readFile(std::istream& is) {
		while (!is.eof()) {
			std::string name;
			is >> name;
			create(name, is);
			is >> std::ws;
		}
	}

	Map() {
		registerCreateMethod("asset", [&](std::istream& is) {
			CurlyBracketList<KeyValuePair> curlyBracketList;

			is >> exceptions >> curlyBracketList;

			std::string* id = nullptr;
			std::string* location = nullptr;

			Map::readDefinitions(curlyBracketList, {
				{ "Id", Type::String, [&](Value value) { id = value.stringValue; if (location != nullptr) AssetManager::instance()->load(*id, *location); } },
				{ "Location", Type::String, [&](Value value) { location = value.stringValue; if (id != nullptr) AssetManager::instance()->load(*id, *location); } }
			});

			if (id == nullptr || location == nullptr) {
				// TODO: throw exception
			}
		});

		registerCreateMethod("texture", [&](std::istream& is) {
			CurlyBracketList<KeyValuePair> curlyBracketList;

			is >> exceptions >> curlyBracketList;

			const std::string* id = nullptr;
			const std::string* location = nullptr;

			bool loaded = false;

			Map::readDefinitions(curlyBracketList, {
				{ "Id", Type::String, [&](Value value) { id = value.stringValue; if (location != nullptr) AssetManager::instance()->loadTexture(*id, *location); } },
				{ "Location", Type::String, [&](Value value) { location = value.stringValue; if (id != nullptr) AssetManager::instance()->loadTexture(*id, *location); } },
				{ "Repeated", Type::Bool, [&](Value value) { if (id != nullptr) AssetManager::instance()->getTexture(*id).setRepeated(value.b); } },
				{ "Smooth", Type::Bool, [&](Value value) { if (id != nullptr) AssetManager::instance()->getTexture(*id).setSmooth(value.b); } }
			});

			if (id == nullptr || location == nullptr) {
				// TODO: throw exception
			}
		});

		registerCreateMethod("rectangle", [&](std::istream& is) {
			CurlyBracketList<KeyValuePair> curlyBracketList;

			is >> curlyBracketList;

			Rectangle* rectangle = new Rectangle();

			bool canCollide = true;

			Map::readDefinitions(curlyBracketList, {
				{ "Color", Type::Color, [&](Value value) { rectangle->setFillColor(*value.colorValue); } },
				{ "Size", Type::Vector, [&](Value value) { rectangle->setSize(*value.vectorValue); } },
				{ "Position", Type::Vector, [&](Value value) { rectangle->setPosition(*value.vectorValue); } },
				{ "TextureId", Type::String, [&](Value value) { rectangle->setTexture(AssetManager::instance()->getTexture(*value.stringValue)); } },
				{ "TextureRect", Type::Rect, [&](Value value) { rectangle->setTextureRect(static_cast<sf::IntRect>(*value.rectValue)); } },
				{ "CanCollide", Type::Bool, [&](Value value) { canCollide = value.b; } }
			});

			drawableGroup.add(*rectangle);

			if (canCollide) {
				collisionGroup.add(*rectangle);
			}
		});
	}
};