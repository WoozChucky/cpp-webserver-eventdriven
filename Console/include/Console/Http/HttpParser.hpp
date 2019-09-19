//
// Created by Nuno Levezinho Silva on 18/09/2019.
//

#ifndef HTTPPARSER_HPP
#define HTTPPARSER_HPP

#include <Console/exports.h>
#include <unordered_map>
#include "HttpRequest.hpp"
#include "HttpProtocol.hpp"

class HttpParser {

public:

    explicit HttpParser(HttpProtocol protocol);

    HttpRequest RequestFromBuffer(const std::string& buffer);

private:

    HttpProtocol _protocol;
    std::unordered_map<std::string, HttpMethod> _methodMapping;

};

#endif //HTTPPARSER_HPP
