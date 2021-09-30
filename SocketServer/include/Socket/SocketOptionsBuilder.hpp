//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#ifndef SOCKETOPTIONSBUILDER_HPP
#define SOCKETOPTIONSBUILDER_HPP

#include <Socket/Sugar.hpp>

class SocketOptionBuilder {

public:
    SocketOptionBuilder() = default;
    ~SocketOptionBuilder() = default;

    SocketOptionBuilder* WithKeepAlive() {
        this->_options.push_back(SO_KEEPALIVE);
        return this;
    }

    SocketOptionBuilder* WithReuseAddress() {
        this->_options.push_back(SO_REUSEADDR);
        return this;
    }

    SocketOptionBuilder* WithReusePort() {
        this->_options.push_back(SO_REUSEPORT);
        return this;
    }

    SocketOptionBuilder* WithSocketOptions(U16 option) {
        this->_options.push_back(option);
        return this;
    }

    SocketOptionBuilder* WithServerPort(U16 port) {
        this->_serverPort = port;
        return this;
    }

    SocketOptionBuilder* WithSSL(bool enabled) {
        this->_sslEnabled = enabled;
        return this;
    }

    SocketOptionBuilder* WithMaxQueuedConnection(U16 connections) {
        this->_maxQueuedConnections = connections;
        return this;
    }

    SocketOptionBuilder* WithCertificate(std::string path) {
        this->_certificatePath = std::move(path);
        return this;
    }

    SocketOptionBuilder* WithPrivateKey(std::string path) {
        this->_privateKeyPath = std::move(path);
        return this;
    }

    ServerConfiguration* Build() {
        return new ServerConfiguration
                {
                        this->_options,
                        this->_serverPort,
                        this->_maxQueuedConnections,
                        this->_sslEnabled,
                        this->_certificatePath,
                        this->_privateKeyPath
                };
    }
private:

    SocketOptionBuilder(const SocketOptionBuilder& copy) {
        this->_options = copy._options;
        this->_maxQueuedConnections = copy._maxQueuedConnections;
        this->_sslEnabled = copy._sslEnabled;
        this->_serverPort = copy._serverPort;
        this->_certificatePath = copy._certificatePath;
        this->_privateKeyPath = copy._privateKeyPath;
    };

    std::vector<int> _options;
    bool _sslEnabled = false;
    U16 _maxQueuedConnections = 100;
    U16 _serverPort = 80; // default http port. 443 is for ssl
    std::string _certificatePath;
    std::string _privateKeyPath;
};

#endif //SOCKETOPTIONSBUILDER_HPP
