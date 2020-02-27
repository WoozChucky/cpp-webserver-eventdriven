//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#include <Socket/Events/MacEventManager.hpp>

#include <cstdio>
#include <sys/epoll.h>

EventManager::EventManager() {
    this->handle = epoll_create1(0);
}

void EventManager::RegisterEvent(SocketContext* ctx, EventType, EventAction action, bool saveContext) {

    epoll_event evSet{};

    if (saveContext)
        evSet.data.ptr = ctx;

    if (epoll_ctl(this->GetHandle(), action, ctx->Socket.Handle, &evSet) == -1)
        fprintf(stdout, "Failed to register event\n");
}

void EventManager::RegisterEvent(SocketHandle socket, EventType, EventAction action) {

    epoll_event evSet{};

    if (epoll_ctl(this->GetHandle(), action, socket, &evSet) == -1)
        fprintf(stdout, "Failed to register event\n");
}

int EventManager::GetFiredNotifications(Event *eventList, int maxEvents) {

    auto nEvents = epoll_wait(this->GetHandle(), eventList, maxEvents, -1);
    if (nEvents < 1) {
        fprintf(stdout, "Error on evt\n");
    }

    return nEvents;
}

EventHandle EventManager::GetHandle() {
    return this->handle;
}

