//
// Created by Nuno Levezinho Silva on 18/09/2019.
//

#include <Console/Http/HttpRequest.hpp>
#include <Console/exports.h>
#include <utility>

HttpRequest::HttpRequest(std::string path, HttpProtocol protocol, HttpMethod method, HttpHeaders headers,
                         std::string body) {

    this->_path = std::move(path);
    this->_protocol = protocol;
    this->_method = method;
    this->_headers = std::move(headers);
    this->_body = std::move(body);
}

const HttpMethod &HttpRequest::GetMethod() {
    return _method;
}

const HttpProtocol &HttpRequest::GetProtocol() {
    return _protocol;
}

const HttpHeaders &HttpRequest::GetHeaders() {
    return _headers;
}

const HttpHeader &HttpRequest::GetHeader(const std::string& key) {
    for(auto& ref : _headers) {
        if (ref.GetKey() == key)
            return ref;
    }
    return HttpHeader("", "");
}

const std::string& HttpRequest::GetBody() {
    return _body;
}

const std::string &HttpRequest::GetPath() {
    return _path;
}

