//
// Created by nunol on 29/02/2020.
//

#include <Http/HttpResponse.hpp>
#include <Abstractions/Logger.hpp>

static std::string temp = "HTTP/1.1 404 Not Found\r\n"
                   "Date: Mon, 16 Sep 2019 09:10:10 GMT\r\n"
                   "Connection: Keep-Alive\r\n"
                   "Content-Type: text/html\r\n"
                   "Content-Length: 0\r\n"
                   "\r\n";

HttpResponse::HttpResponse() {

}

Memory HttpResponse::GetBuffer() {

    // TODO(Levezinho): Auto-calculate the content length based on the body .size()
    // response->AddHeader(HttpHeader("Content-Length", "13"));

    std::string responseString =   "HTTP/1.1 200 OK\r\n"
                                   "Date: Mon, 16 Sep 2020 09:10:10 GMT\r\n"
                                   "Connection: Keep-Alive\r\n"
                                   "Content-Type: application/json\r\n"
                                   "Content-Length: 13\r\n"
                                   "\r\n"
                                   "{\"obj\": true}";

    TRACE("%s", "Using implicit operator.");

    return (Memory) responseString.c_str();
}

U16 HttpResponse::GetBufferSize() {

    std::string responseString =   "HTTP/1.1 200 OK\r\n"
                                   "Date: Mon, 16 Sep 2020 09:10:10 GMT\r\n"
                                   "Connection: Keep-Alive\r\n"
                                   "Content-Type: application/json\r\n"
                                   "Content-Length: 13\r\n"
                                   "\r\n"
                                   "{\"obj\": true}";

    TRACE("%s", "Getting buffer size.");

    return responseString.size();
}

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



