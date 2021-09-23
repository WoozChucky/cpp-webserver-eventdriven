//
// Created by nunol on 29/02/2020.
//

#include <Http/HttpResponse.hpp>
#include <Abstractions/Logger.hpp>

HttpResponse::HttpResponse() = default;

void HttpResponse::SetBody(std::string body) {
    this->_body = std::move(body);
}

void HttpResponse::SetStatusCode(HttpStatusCode code) {
    this->_code = code;
}

void HttpResponse::SetProtocol(HttpProtocol protocol) {
    this->_protocol = protocol;
}

void HttpResponse::AddHeader(const HttpHeader& header) {
    this->_headers.push_back(header);
}

std::string HttpResponse::GetBody() {
    return this->_body;
}

HttpStatusCode HttpResponse::GetStatusCode() {
    return this->_code;
}

HttpProtocol HttpResponse::GetProtocol() {
    return this->_protocol;
}

HttpHeaders HttpResponse::GetHeaders() {
    return this->_headers;
}

