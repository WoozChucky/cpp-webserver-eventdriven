//
// Created by nunol on 04/03/2020.
//

#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <Abstractions/Types.hpp>

class EventManager {

public:

    EventManager();

    void RegisterEvent(SocketContext* ctx, EventType type, EventAction action, bool saveContext = false);
    void RegisterEvent(SocketHandle socket, EventType type, EventAction action) const;
    [[nodiscard]] EventHandle GetHandle() const;
    int GetFiredNotifications(Event *eventList, int maxEvents);

private:

    EventHandle handle;

};

#endif //EVENTMANAGER_HPP
