#include "event.h"
#include <iostream>

Event::Event(event_type_e type, const char* data) {
	this->type = type;
	this->data = data;
}

EventManager::EventManager() {
	while (!eventQueue.empty())
		eventQueue.pop();
}

void EventManager::PumpEvents() {
	while (!eventQueue.empty()) {
		Event* event = eventQueue.front();
		for (size_t j = 0; j < listeners.size(); ++j) {
			listeners[j]->Responder(event);
		}
		eventQueue.pop();
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
