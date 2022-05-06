#include "event.h"
#include <iostream>

Event::Event(event_type_e type, const char* data, double x, double y) {
	this->type = type;
	this->data = data;
	this->x = x;
	this->y = y;
}

EventManager::EventManager() {
	while (!eventQueue.empty())
		eventQueue.pop();
}

EventManager::~EventManager() {
	while (!eventQueue.empty()) {
		Event* event = eventQueue.front();
		delete event;
		eventQueue.pop();
	}
}

void EventManager::PumpEvents() {
	while (!eventQueue.empty()) {
		Event* event = eventQueue.front();
		eventQueue.pop();
		for (auto listener : listeners)
			listener->Responder(event);
		delete (event);
	}
}

void EventManager::AddListener(EventListener* listener) {
	listeners.push_back(listener);
}

void EventManager::RemoveListener(EventListener* listener) {
	for (auto j = listeners.begin(); j != listeners.end(); ++j) {
		if (*j == listener) {
			listeners.erase(j);
			return;
		}
	}
}

void EventManager::Post(Event* event) {
	eventQueue.push(event);
}
