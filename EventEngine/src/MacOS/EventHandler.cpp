//
// Created by nunol on 04/03/2020.
//

#include <Events/EventHandler.hpp>

EventHandler::EventHandler(EventHandle serverHandle) {
	this->_serverHandle = serverHandle;
}

HandledEvent EventHandler::DigestEvent(Event event) {

	HandledEvent evt{nullptr, Weird};

	if (event.ident == this->_serverHandle) {

		evt.Context = nullptr;

	} else {

		auto ctx = reinterpret_cast<SocketContext *>(event.udata);

		evt.Context = ctx;

		if ((event.flags & EV_EOF) || (event.flags & EV_ERROR)) { // Error or EOF Event

			evt.Type = EventType::Disconnect;
		}

		if (event.filter == EVFILT_READ) { // Read Event

			evt.Type = EventType::Read;
		}

		if (event.filter == EVFILT_WRITE) { // Write Event

			evt.Type = EventType::Write;
		}

	}

	return evt;
}
