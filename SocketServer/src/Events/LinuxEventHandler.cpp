//
// Created by nunol on 26/02/2020.
//

#include <Socket/Events/EventHandler.hpp>
#include <sys/epoll.h>

EventHandler::EventHandler(EventHandle serverHandle) : _serverHandle(serverHandle) {

}

HandledEvent EventHandler::DigestEvent(Event event) {

    HandledEvent evt;

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
