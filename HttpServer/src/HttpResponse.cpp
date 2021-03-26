//
// Created by nunol on 29/02/2020.
//

#include <Http/HttpResponse.hpp>
#include <Abstractions/Logger.hpp>

/*
static std::string temp = "HTTP/1.1 404 Not Found\r\n"
                   "Date: Mon, 16 Sep 2019 09:10:10 GMT\r\n"
                   "Connection: Keep-Alive\r\n"
                   "Content-Type: text/html\r\n"
                   "Content-Length: 0\r\n"
                   "\r\n";
*/

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



