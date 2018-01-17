#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

std::ostream& operator<<(std::ostream& os, sf::Vector2f v) {
	os << v.x;
	os << ", ";
	os << v.y;
	return os;
}

class Drawable {
protected:
	sf::Drawable& drawable;

	Drawable(sf::Drawable& drawable) : drawable(drawable) { }

public:
	virtual void update(const float elapsedTime) = 0;

	virtual void draw(sf::RenderTarget& renderTarget) final {
		renderTarget.draw(drawable);
	};
};

class PhysicsObject : public Drawable {
protected:
	sf::Transformable& transformable;
	sf::Vector2f velocity = sf::Vector2f(0, 0);
	sf::Vector2f gravity = sf::Vector2f(0, 981.0f);

	PhysicsObject(sf::Shape& shape) : Drawable(shape), transformable(shape) { }
	PhysicsObject(sf::Sprite& sprite) : Drawable(sprite), transformable(sprite) { }

public:
	virtual void update(const float elapsedTime) {
		applyForce(gravity * elapsedTime);
		setPosition(getPosition() + velocity * elapsedTime);
	}

	virtual void setGravity(const sf::Vector2f& gravity) {
		this->gravity = gravity;
	}

	virtual void setPosition(const sf::Vector2f& position) {
		transformable.setPosition(position);
	}

	virtual sf::Vector2f getPosition() {
		return transformable.getPosition();
	}

	virtual sf::FloatRect getBounds() = 0;

	virtual void applyForce(const sf::Vector2f& force) {
		velocity += force;
	}

	virtual void setVelocity(const sf::Vector2f& velocity) {
		this->velocity = velocity;
	}

	virtual sf::Vector2f getVelocity() {
		return velocity;
	}
};

class Collision {
private:
	sf::Vector2f delta;
	sf::Vector2f intersect;

public:
    /**
     * @brief ADT holding collision information.
     * @param delta Delta between two objects.
     * @param intersect Intersection vector between two objects.
     */
	Collision(const sf::Vector2f delta, const sf::Vector2f intersect) :
		delta(delta),
		intersect(intersect)
	{};

    /**
     * @brief Check if there has been a collision.
     * @return Result.
     */
	bool check() {
		if (getIntersect().x < 0.0f && getIntersect().y < 0.0f) {
			return true;
		}

		return false;
	}

    /**
     * @brief Get collision delta between two objects.
     * @return Delta.
     */
	sf::Vector2f getDelta() const { return delta; };
    
    /**
     * @brief Get intersect between two objects.
     * @return Intersect.
     */
	sf::Vector2f getIntersect() const { return intersect; };
};

class Ball : public PhysicsObject, public sf::CircleShape {
public:
	Ball() : sf::CircleShape(), PhysicsObject(static_cast<sf::CircleShape&>(*this)) {
		sf::CircleShape::setFillColor({ 255, 255, 255 });
	}

	void update(const float elapsedTime) override {
		PhysicsObject::update(elapsedTime);
	}

	void setRadius(const float radius) {
		sf::CircleShape::setOrigin(radius / 2.0f, radius / 2.0f);
		sf::CircleShape::setRadius(radius);
	}

	sf::FloatRect getBounds() override {
		return sf::CircleShape::getGlobalBounds();
	}

	using PhysicsObject::setPosition;
	using PhysicsObject::getPosition;
	using PhysicsObject::draw;
};

class Rectangle : public PhysicsObject, public sf::RectangleShape {
public:
	Rectangle() : sf::RectangleShape(), PhysicsObject(static_cast<sf::RectangleShape&>(*this)) {
		sf::RectangleShape::setFillColor({ 255, 255, 255 });
	}

	void update(const float elapsedTime) override {
		PhysicsObject::update(elapsedTime);
	}

	void setSize(sf::Vector2f size) {
		sf::RectangleShape::setOrigin(size.x / 2, size.y / 2);
		sf::RectangleShape::setSize(size);
	}

	void move(sf::Vector2f offset) {
		sf::RectangleShape::move(offset);
	}

	sf::FloatRect getBounds() override {
		return sf::RectangleShape::getGlobalBounds();
	}

    /**
     * @brief Resolve an collision.
     * @param other Rectangle.
     */
	void resolveCollision(Rectangle &other) {
		Collision collision = getCollision(other);

		if (collision.check()) {
			if (collision.getIntersect().x > collision.getIntersect().y) {
				if (collision.getDelta().x > 0.0f) {
					move(sf::Vector2f(collision.getIntersect().x * 1.0f, 0.0f));
				}
				else {
					move(sf::Vector2f(-collision.getIntersect().x * 1.0f, 0.0f));
				}
			}
			else {
				if (collision.getDelta().y > 0.0f) {
					move(sf::Vector2f(0.0f, collision.getIntersect().y * 1.0f));
				}
				else {
					move(sf::Vector2f(0.0f, -collision.getIntersect().y * 1.0f));
				}
			}
		}
	}

    /**
     * @brief Get an collision instance containing the delta and intersect position between two rectangles.
     * @param other Rectangle.
     * @return Collision class instance.
     */
	Collision getCollision(Rectangle &other) {
		sf::Vector2f otherPosition = other.getPosition();
		sf::Vector2f otherHalfSize = other.getSize() / 2.0f;
		sf::Vector2f thisPosition = getPosition();
		sf::Vector2f thisHalfSize = getSize() / 2.0f;

		sf::Vector2f delta(otherPosition.x - thisPosition.x, otherPosition.y - thisPosition.y);
		sf::Vector2f intersect(abs(delta.x) - (otherHalfSize.x + thisHalfSize.x), abs(delta.y) - (otherHalfSize.y + thisHalfSize.y));

		return Collision(delta, intersect);
	}

    /**
     * @brief Check if two rectangles intersects with each other.
     * @param other Rectangle.
     * @return Result.
     */
	bool intersects(Rectangle &other) {
		Collision collision = getCollision(other);

		return collision.check();
	}

	using PhysicsObject::setPosition;
	using PhysicsObject::getPosition;
	using PhysicsObject::draw;
};

int main() {
	const float FPS = 60.0f; // The desired FPS. (The number of updates each second).
	bool redraw = true;      // Do I redraw everything on the screen?

	std::vector<Drawable*> drawables;

	sf::RenderWindow window(sf::VideoMode(300, 900, 32), "Hello");
	window.setFramerateLimit((int) FPS);
	sf::Clock clock;

	Rectangle rect = Rectangle();
	rect.setSize(sf::Vector2f(10, 10));
	rect.setPosition({ 150, 0 });
	
	Rectangle rect2 = Rectangle();
	rect2.setSize(sf::Vector2f(25, 25));
	rect2.setPosition({ 150, 600 });
	rect2.setGravity({ 0,0 });


	sf::Event ev;
	while (window.isOpen()) {
		// Wait until 1/60th of a second has passed, then update everything.
		float elapsedTime = clock.getElapsedTime().asSeconds();

		// Handle input
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) window.close();
		}

		if (elapsedTime >= 1.0f / FPS) {
			window.setTitle(std::to_string(1 / elapsedTime));

			clock.restart();

			window.clear();

			if (rect.intersects(rect2)) {
				std::cout << "Intersect!\n";

				rect.resolveCollision(rect2);

				rect.setVelocity(sf::Vector2f(0, 0));
				rect.setGravity(sf::Vector2f(0, 0));
			}

			rect.draw(window);
			rect2.draw(window);
			rect.update(elapsedTime);
			rect2.update(elapsedTime);
			window.display();
		}
	}

	return 0;
}