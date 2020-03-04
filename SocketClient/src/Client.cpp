//
// Created by NB22477 on 04/03/2020.
//

#include <Socket/Client.hpp>
#include <Socket/TlsDecorator.hpp>

bool Client::Connect(const std::string &ipAddress) {
    TlsDecorator decorator;
    decorator.Connect("google.pt");
    return false;
}