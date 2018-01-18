#pragma once

class GameState {
	const std::string name;
public:

	GameState(const std::string& name) : name(std::move(name)) { }

	virtual void update(const float elapsedTime) = 0;

	virtual void entry() { };

	virtual void exit() { };

	friend bool operator==(GameState* state, const std::string& name) {
		return state->name == name;
	}
};