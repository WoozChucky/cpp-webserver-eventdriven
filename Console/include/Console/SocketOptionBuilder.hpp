//
// Created by Nuno Levezinho Silva on 11/09/2019.
//

#ifndef SOCKETOPTIONBUILDER_HPP
#define SOCKETOPTIONBUILDER_HPP

#include <vector>
#include <sys/socket.h>

class SocketOptionBuilder {

public:
    SocketOptionBuilder() = default;
    ~SocketOptionBuilder() = default;

    SocketOptionBuilder* WithKeepAlive() {
        _options.push_back(SO_KEEPALIVE);
        return this;
    }

    SocketOptionBuilder* WithReuseAddress() {
        _options.push_back(SO_REUSEADDR);
        return this;
    }

    SocketOptionBuilder* WithReusePort() {
        _options.push_back(SO_REUSEPORT);
        return this;
    }

    std::vector<int> Build() {
        return _options;
    }
private:

    SocketOptionBuilder(const SocketOptionBuilder& copy) {
        this->_options = copy._options;
    };

    std::vector<int> _options;
};

#endif //SOCKETOPTIONBUILDER_HPP
