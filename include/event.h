#pragma once
#include "defs.h"

enum event_type_e {
	QUIT,
	NONE,
	CHANGE_SCENE,
	KEYDOWN,
	SPAWN_PIPE,
	KILL_ENT,
	GAME_RESTART,
	INC_SCORE,
	MOUSE_BUTT
};

struct Event {
	Event(event_type_e type, const char* data);
	~Event() = default;

	event_type_e type;
	const char* data;
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
