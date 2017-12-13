#include <SFML/Graphics.hpp>

#include "BallBody.hpp"
#include "RectangleBody.hpp"

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	
	BallBody ball0 = { 0 };
	BallBody ball1 = { 0 };
//	RectangleBody rectangle = { sf::Vector2f(100, 20) };
	BallBody ball2 = { 0 };
	BallBody ball3 = { 0 };
	BallBody ball4 = { 0 };
	
	std::vector<Body*> bodies = { &ball0, &ball1, &ball2, &ball3, &ball4 };
	
	for (int i = 0; i < 600 / 40; i++) {
		Body* ball = new BallBody(20);
		ball->setAnchored(true);
		ball->setPosition(sf::Vector2f(0, i * 40));
		bodies.push_back(ball);
	}
	
	for (int i = 0; i < 800 / 40; i++) {
		Body* ball = new BallBody(20);
		ball->setAnchored(true);
		ball->setPosition(sf::Vector2f(i * 40, 600));
		bodies.push_back(ball);
	}
	
	for (int i = 0; i < 640 / 40; i++) {
		Body* ball = new BallBody(20);
		ball->setAnchored(true);
		ball->setPosition(sf::Vector2f(800, i * 40));
		bodies.push_back(ball);
	}
	
	for (int i = 0; i < 800 / 40; i++) {
		Body* ball = new BallBody(20);
		ball->setAnchored(true);
		ball->setPosition(sf::Vector2f(i * 40, 0));
		bodies.push_back(ball);
	}
	
	for (int i = 0; i < 10; i++) {
		Body* ball = new BallBody(20);
		ball->setPosition(sf::Vector2f(90 + 60*i, 50));
		bodies.push_back(ball);
	}
	
	for (int i = 0; i < 10; i++) {
		Body* ball = new BallBody(20);
		ball->setPosition(sf::Vector2f(120 + 60*i, 0));
		bodies.push_back(ball);
	}
	
	ball0.setPosition(sf::Vector2f(395, 100));
	ball1.setPosition(sf::Vector2f(405, 155));
	ball2.setPosition(sf::Vector2f(400, 400));
	ball2.setAnchored(true);
	ball3.setPosition(sf::Vector2f(620, 400));
	ball3.setAnchored(true);
	ball4.setPosition(sf::Vector2f(180, 400));
	ball4.setAnchored(true);
//	rectangle.setPosition(sf::Vector2f(400, 400));
	
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		
		window.clear();
		
		for (Body* b : bodies) {
			for (Body* other : bodies) {
				if (b != other) {
					if (b->checkIntersect(*other)) {
						b->resolveCollision(*other);
					}
				}
			}
			
			b->updatePhysics();
			b->draw(window);
		}
		
		window.display();
		
		sf::sleep(sf::milliseconds(20));
	}
	
	return 0;
}