//
// Created by nunol on 26/02/2020.
//

#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <Socket/Sugar.hpp>

class EventHandler {

public:

    explicit EventHandler(EventHandle serverHandle);

    HandledEvent DigestEvent(Event event);

private:
    EventHandle  _serverHandle;
};

#endif //EVENTHANDLER_HPP
