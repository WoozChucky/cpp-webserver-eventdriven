//
// Created by nunol on 04/03/2020.
//

#include <Events/EventHandler.hpp>

EventHandler::EventHandler(EventHandle serverHandle) {
    this->_serverHandle = serverHandle;
}

HandledEvent EventHandler::DigestEvent(Event event) {

    HandledEvent evt{nullptr, Weird};

    if (event.data.fd == this->_serverHandle) {

        evt.Context = nullptr;

    } else {

        auto ctx = reinterpret_cast<SocketContext *>(event.data.ptr);

        evt.Context = ctx;

        if ((event.events & EPOLLHUP) || (event.events & EPOLLERR)) {

            evt.Type = Disconnect;
        }

        if (event.events & EPOLLIN) { // Read Event

            evt.Type = Read;
        }

        if (event.events & EPOLLOUT) { // Write Event

            evt.Type = Write;
        }

    }

    return evt;
}


