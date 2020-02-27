//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#include <Socket/Events/MacEventManager.hpp>

#include <cstdio>
#include <sys/epoll.h>
#include <Abstractions/Logger.hpp>

EventManager::EventManager() {
    this->handle = epoll_create(32);
}

void EventManager::RegisterEvent(SocketContext* ctx, EventType, EventAction action, bool saveContext) {

    epoll_event evSet{};

    evSet.events = EPOLLIN | EPOLLET;

    if (saveContext)
        evSet.data.ptr = ctx;

    if (epoll_ctl(this->GetHandle(), action, ctx->Socket.Handle, &evSet) == -1)
        TRACE("%s", "Failed to register event.");
}

void EventManager::RegisterEvent(SocketHandle socket, EventType, EventAction action) {

    epoll_event evSet{};

    evSet.events = EPOLLIN | EPOLLET;
    evSet.data.fd = socket;

    if (epoll_ctl(this->GetHandle(), action, socket, &evSet) == -1)
        TRACE("%s", "Failed to register event.");
}

int EventManager::GetFiredNotifications(Event *eventList, int maxEvents) {

    auto nEvents = epoll_wait(this->GetHandle(), eventList, maxEvents, -1);
    if (nEvents < 1) {
        TRACE("%s", "Failed getting fired notifications..");
    }

    return nEvents;
}

EventHandle EventManager::GetHandle() {
    return this->handle;
}

