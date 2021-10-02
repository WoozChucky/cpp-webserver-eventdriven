//
// Created by nunol on 04/03/2020.
//

#include <Events/EventManager.hpp>
#include <Abstractions/Logger.hpp>

EventManager::EventManager() {
    //this->handle = epoll_create(32);
    this->handle = kqueue();
}

void EventManager::RegisterEvent(SocketContext* ctx, EventType, EventAction action, bool saveContext) {

    Event evt{};

    evt.filter = EVFILT_READ;

    EV_SET(&evt, ctx->Socket.Handle, EVFILT_READ, action, 0, 0, saveContext ? ctx : nullptr);

    if (kevent(this->GetHandle(), &evt, 1, nullptr, 0, nullptr ) == -1)
        TRACE("%s", "Failed to register event.");
}

void EventManager::RegisterEvent(SocketHandle socket, EventType evtType, EventAction action) const {

    Event evt{};

    EV_SET(&evt, socket, evtType, action, 0, 0, nullptr);

    if (kevent(this->GetHandle(), &evt, 1, nullptr, 1, nullptr) == -1)
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
