//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#ifndef MACEVENTMANAGER_HPP
#define MACEVENTMANAGER_HPP

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

#endif //MACEVENTMANAGER_HPP
