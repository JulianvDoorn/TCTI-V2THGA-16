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
		std::cout << velocity << std::endl;
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

	virtual bool intersects(PhysicsObject &o) {
		return getBounds().intersects(o.getBounds());
	}
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

	using PhysicsObject::setPosition;
	using PhysicsObject::getPosition;
	using PhysicsObject::draw;
};

int main() {
	const float FPS = 60.0f; // The desired FPS. (The number of updates each second).
	bool redraw = true;      // Do I redraw everything on the screen?

	std::vector<Drawable*> drawables;
	sf::View view2(sf::Vector2f(150, 10), sf::Vector2f(300, 200));

	sf::RenderWindow window(sf::VideoMode(300, 900, 32), "Hello");
	window.setFramerateLimit((int) FPS);
	sf::Clock clock;

	Ball ball = Ball();
	ball.setRadius(10);
	ball.setPosition({ 150, 0 });
	
	Ball ball2 = Ball();
	ball2.setRadius(20);
	ball2.setPosition({ 150, 200 });
	window.setView(view2);
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

			window.clear(sf::Color(0, 0, 0));
			
			ball.draw(window);
			ball2.draw(window);
			ball.update(elapsedTime);
			ball2.update(elapsedTime);
            view2.move(1,1);
             window.setView(view2);
			window.display();
		}
	}

	return 0;
}