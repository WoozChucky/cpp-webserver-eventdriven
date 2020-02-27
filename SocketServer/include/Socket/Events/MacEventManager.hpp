//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <Socket/Sugar.hpp>

class EventManager {

public:

    EventManager();

    void RegisterEvent(SocketContext* ctx, EventType type, EventAction action, bool saveContext = false);
    void RegisterEvent(SocketHandle socket, EventType type, EventAction action);
    EventHandle GetHandle();
    int GetFiredNotifications(Event *eventList, int maxEvents);

private:

    EventHandle handle;

};

#endif //EVENTMANAGER_HPP
