//
// Created by Nuno Levezinho Silva on 01/10/2019.
//

#include <Http/HttpRequest.hpp>
#include <utility>

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
    return "";
}

const std::string& HttpRequest::GetBody() const
{
    return _body;
}

const std::string &HttpRequest::GetPath() const
{
    return _path;
}
