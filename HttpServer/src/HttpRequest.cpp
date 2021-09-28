//
// Created by Nuno Levezinho Silva on 01/10/2019.
//

#include <Http/HttpRequest.hpp>
#include <utility>
#include <Abstractions/Format.hpp>
#include <Http/Exceptions/NotFoundException.hpp>

HttpRequest::HttpRequest(std::string path, HttpProtocol protocol, HttpMethod method, HttpHeaders headers,
                         std::string body) {

    this->_path = std::move(path);
    this->_protocol = protocol;
    this->_method = method;
    this->_headers = std::move(headers);
    this->_body = std::move(body);
}

const HttpMethod &HttpRequest::GetMethod() const
{
    return _method;
}

const HttpProtocol &HttpRequest::GetProtocol() const
{
    return _protocol;
}

[[maybe_unused]] const HttpHeaders &HttpRequest::GetHeaders() const
{
    return _headers;
}

std::string HttpRequest::GetHeader(const std::string& key) const
{
    for(auto& ref : _headers) {
        if (ref.GetKey() == key)
            return ref.GetValue();
    }
    throw NotFoundException(    // No handler has been found, return 404.
            Format::This(
                    "Header %s not found",
                    key.data()
            )
    );
}

const std::string& HttpRequest::GetBody() const
{
    return _body;
}

const std::string &HttpRequest::GetPath() const
{
    return _path;
}

bool HttpRequest::HasBody() const {
    return _method == POST || _method == PUT || _method == PATCH;
}
