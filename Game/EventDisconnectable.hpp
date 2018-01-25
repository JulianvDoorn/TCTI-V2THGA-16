#pragma once

class EventConnection;

class EventDisconnectable {
protected:
	virtual void disconnect(EventConnection& conn) = 0;

	friend class EventConnection;
};