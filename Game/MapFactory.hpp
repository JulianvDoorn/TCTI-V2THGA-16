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
#include "CollisionDetection.hpp"
#include "DrawableGroup.hpp"

class Map : public BaseFactory<void, std::string, std::istream&> {
public:
	using Type = KeyValuePair::Type;

	static void checkTypeMatch(Type lhs, Type rhs) {
		// TODO
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
		});

		registerCreateMethod("rectangle", [&](std::istream& is) {
			CurlyBracketList<KeyValuePair> curlyBracketList;

			is >> curlyBracketList;

			Rectangle* rectangle = new Rectangle();

			bool canCollide = true;

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
				else if (pair.key == "CanCollide") {
					checkTypeMatch(pair.type, Type::Bool);
					canCollide = pair.value.b;
				}
			}

			drawableGroup.add(*rectangle);

			if (canCollide) {
				collisionGroup.add(*rectangle);
			}
		});
	}
};