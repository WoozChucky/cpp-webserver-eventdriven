//
// Created by Nuno Levezinho Silva on 11/09/2019.
//

#ifndef SOCKETOPTIONBUILDER_HPP
#define SOCKETOPTIONBUILDER_HPP

#include <vector>
#include <sys/socket.h>
#include "exports.h"

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

    SocketOptionBuilder* WithServerPort(uint port) {
        _serverPort = port;
        return this;
    }

    SocketOptionBuilder* WithSSL(bool enabled) {
        this->_sslEnabled = enabled;
        return this;
    }

    SocketOptionBuilder* WithMaxQueuedConnection(uint connections) {
        this->_maxQueuedConnections = connections;
        return this;
    }

    ServerConfiguration* Build() {
        return new ServerConfiguration
        {
            _options,
            _serverPort,
            _maxQueuedConnections,
            _sslEnabled
        };
    }
private:

    SocketOptionBuilder(const SocketOptionBuilder& copy) {
        this->_options = copy._options;
        this->_maxQueuedConnections = copy._maxQueuedConnections;
        this->_sslEnabled = copy._sslEnabled;
        this->_serverPort = copy._serverPort;
    };

    std::vector<int> _options;
    bool _sslEnabled = true;
    uint _maxQueuedConnections = 100;
    uint _serverPort = 443;
};

#endif //SOCKETOPTIONBUILDER_HPP
