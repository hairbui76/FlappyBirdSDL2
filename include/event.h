#pragma once
#include "defs.h"

enum event_type_e {
	QUIT,
	CHANGE_SCENE,
	KEYDOWN,
	KILL_ENT,
	GAME_RESTART,
	INC_SCORE,
	MOUSE_BUTT
};

struct Event {
	Event(event_type_e type, const char* data, double x = 0, double y = 0);
	~Event() = default;

	event_type_e type;
	const char* data;
	double x;
	double y;
};

struct EventListener {
	virtual void Responder(Event* event) = 0;
	EventListener() = default;
	virtual ~EventListener() = default;
};

class EventManager {
	std::vector<EventListener*> listeners;
	std::queue<Event*> eventQueue;

public:
	EventManager();
	~EventManager() = default;

	void PumpEvents();
	void AddListener(EventListener* listener);
	void RemoveListener(EventListener* listener);
	void Post(Event* event);
};
