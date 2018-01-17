#pragma once

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
