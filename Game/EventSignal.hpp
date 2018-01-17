#pragma once

class EventArguments { };

class EventSignal {
public:
	virtual const EventArguments& getArguments() const = 0;
};