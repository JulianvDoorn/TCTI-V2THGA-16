#pragma once

#include "BaseFactory.hpp"
#include <istream>
#include <utility>
#include <memory>
#include "Ball.hpp"
#include "IOExceptions.hpp"
#include "Rectangle.hpp"
#include "ColorFactory.hpp"
#include "VectorMath.hpp"
#include "KeyValuePair.hpp"
#include "CurlyBracketList.hpp"

class MapFactory : public BaseFactory<PhysicsObject, std::string, std::istream&> {
	using Type = KeyValuePair::Type;

	void checkTypeMatch(Type lhs, Type rhs) {
		// TODO
	}

public:
	MapFactory() {
		registerCreateMethod("asset", [this](std::istream& is) {
			CurlyBracketList<KeyValuePair> curlyBracketList;

			is >> exceptions >> curlyBracketList;

			std::string* id = nullptr;
			std::string* location = nullptr;

			for (const KeyValuePair& pair : curlyBracketList) {
				if (pair.key == "Id") {
					checkTypeMatch(pair.type, Type::Color);
					id = pair.value.s;
				}
				else if (pair.key == "Location") {
					checkTypeMatch(pair.type, Type::String);
					location = pair.value.s;
				}
			}

			if (id != nullptr && location != nullptr) {
				AssetManager::instance()->load(*id, *location);
			}
			else {
				// TODO: throw exception
			}

			return nullptr; // no real return value
		});

		registerCreateMethod("rectangle", [this](std::istream& is) {
			CurlyBracketList<KeyValuePair> curlyBracketList;

			is >> curlyBracketList;

			Rectangle* rectangle = new Rectangle();

			for (const KeyValuePair& pair : curlyBracketList) {
				if (pair.key == "Color") {
					checkTypeMatch(pair.type, Type::Color);
					rectangle->setFillColor(*pair.value.c);
				}
				else if (pair.key == "Size") {
					checkTypeMatch(pair.type, Type::Vector);
					rectangle->setSize(*pair.value.v);
				}
				else if (pair.key == "Position") {
					checkTypeMatch(pair.type, Type::Vector);
					rectangle->setPosition(*pair.value.v);
				}
				else if (pair.key == "TextureId") {
					checkTypeMatch(pair.type, Type::String);
					sf::Texture& texture = AssetManager::instance()->getTexture(*pair.value.s);
					rectangle->setTexture(texture);
				}
			}

			return rectangle;
		});
	}
};