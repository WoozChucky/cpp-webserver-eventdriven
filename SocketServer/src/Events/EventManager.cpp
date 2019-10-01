//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#include <Socket/Events/EventManager.hpp>

#include <cstdio>
#include <sys/event.h>

EventManager::EventManager() {
    this->handle = kqueue();
}

void EventManager::RegisterEvent(SocketContext* ctx, EventType type, EventAction action, bool saveContext) {

    Event evSet;

    EV_SET(&evSet, ctx->Socket.Handle, type, action, 0, 0, nullptr);

    if (saveContext)
        evSet.udata = ctx;

    if (kevent(this->GetHandle(), &evSet, 1, nullptr, 0, nullptr) == -1)
        fprintf(stdout, "Failed to register event\n");
}

void EventManager::RegisterEvent(SocketHandle socket, EventType type, EventAction action) {

    Event evSet;

    EV_SET(&evSet, socket, type, action, 0, 0, nullptr);

    if (kevent(this->GetHandle(), &evSet, 1, nullptr, 0, nullptr) == -1)
        fprintf(stdout, "Failed to register event\n");

}

int EventManager::GetFiredNotifications(Event *eventList, int maxEvents) {

    auto nEvents = kevent(this->GetHandle(), nullptr, 0, eventList, maxEvents, nullptr);
    if (nEvents < 1) {
        fprintf(stdout, "Error on evt\n");
    }

    return nEvents;
}

EventHandle EventManager::GetHandle() {
    return this->handle;
}

