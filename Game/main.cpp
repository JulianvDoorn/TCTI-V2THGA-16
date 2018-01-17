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

	sf::Vector2f getCenter() {
		sf::Vector2f center = getPosition();
		center.x = center.x + (getRadius()/2);
		center.y = center.y + (getRadius()/2);
		return center;
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

class Player : public Ball{
private:
	sf::View &view;
	sf::RenderWindow &window;
	
public:
	Player(sf::View &view, sf::RenderWindow &window): 
		view(view),
		window(window)
	{
		setRadius(20);
		setPosition({ 150, 450 });
		setFillColor(sf::Color(0, 255, 0));
//		ball.draw(window);
//		window.display();
	}
	void update(const float elapsedTime) override {
		PhysicsObject::update(elapsedTime);
		view.setCenter(getCenter());
		window.setView(view);
	}

	sf::FloatRect getBounds() override {
		return sf::CircleShape::getGlobalBounds();
	}

	
	using Ball::setPosition;
	using Ball::getPosition;
	using Ball::setRadius;
	using Ball::draw;
};
int main() {
	const float FPS = 60.0f; // The desired FPS. (The number of updates each second).
	bool redraw = true;      // Do I redraw everything on the screen?

	std::vector<Drawable*> drawables;
	sf::View view2(sf::Vector2f(150, 10), sf::Vector2f(150, 450));

	sf::RenderWindow window(sf::VideoMode(300, 900, 32), "Hello");
	window.setFramerateLimit((int) FPS);
	sf::Clock clock;

	Ball ball = Ball();
	ball.setRadius(10);
	ball.setPosition({ 150, 500 });
	ball.setFillColor(sf::Color(255, 0, 0));


	Player player = Player(view2, window);

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
			player.draw(window);
			player.update(elapsedTime);
			//ball.update(elapsedTime);
			window.display();
		}
	}

	return 0;
}