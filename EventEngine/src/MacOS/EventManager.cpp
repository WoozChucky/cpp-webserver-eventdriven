//
// Created by nunol on 04/03/2020.
//

#include <Events/EventManager.hpp>

#include <cstdio>
#include <sys/event.h>
#include <Abstractions/Logger.hpp>

EventManager::EventManager() {
    this->handle = kqueue();
}

void EventManager::RegisterEvent(SocketContext* ctx, EventType type, EventAction action, bool saveContext) {

    Event evSet;

    EV_SET(&evSet, ctx->Socket.Handle, type, action, 0, 0, nullptr);

    if (saveContext)
        evSet.udata = ctx;

    if (kevent(this->GetHandle(), &evSet, 1, nullptr, 0, nullptr) == -1)
        TRACE("%s", "Failed to register event.");
}

void EventManager::RegisterEvent(SocketHandle socket, EventType type, EventAction action) const {

    Event evSet;

    EV_SET(&evSet, socket, type, action, 0, 0, nullptr);

    if (kevent(this->GetHandle(), &evSet, 1, nullptr, 0, nullptr) == -1)
        TRACE("%s", "Failed to register event.");

}

int EventManager::GetFiredNotifications(Event *eventList, int maxEvents) {

    auto nEvents = kevent(this->GetHandle(), nullptr, 0, eventList, maxEvents, nullptr);
    if (nEvents < 1) {
        TRACE("%s", "Failed getting fired notifications..");
    }

    return nEvents;
}

EventHandle EventManager::GetHandle() const {
    return this->handle;
}
