//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#include <Http/HttpParser.hpp>
#include <deque>
#include <cstring>
#include <sstream>

HttpParser::HttpParser(HttpProtocol protocol) {
    this->_protocol = protocol;

    this->_methodMapping["GET"]     = HttpMethod::GET;
    this->_methodMapping["POST"]    = HttpMethod::POST;
    this->_methodMapping["HEAD"]    = HttpMethod::HEAD;
    this->_methodMapping["PUT"]     = HttpMethod::PUT;
    this->_methodMapping["PATCH"]   = HttpMethod::PATCH;
    this->_methodMapping["DELETE"]  = HttpMethod::DELETE;
    this->_methodMapping["CONNECT"] = HttpMethod::CONNECT;
    this->_methodMapping["OPTIONS"] = HttpMethod::OPTIONS;
    this->_methodMapping["TRACE"]   = HttpMethod::TRACE;
}

std::deque<std::string> split(const std::string& str, const std::string& delim)
{
    std::deque<std::string> tokens;
    size_t prev = 0, pos = 0;

    do  {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());

    return tokens;
}

std::string get_right_of_delim(std::string const& str, std::string const& delim){
    return str.substr(str.find(delim) + delim.size() + 1);
}

std::string get_left_of_delim(std::string const& str, std::string const& delim){
    return str.substr(0,str.find(delim));
}

bool RequestHasBody(HttpMethod method) {
    return method == ::POST || method == ::PATCH || method == ::PUT;
}

HttpRequest* HttpParser::RequestFromBuffer(const std::string& buffer) {

    auto lines = split(buffer, "\r\n");

    auto startLine = lines[0];

    auto startLineSplit = split(startLine, " ");

    auto stringMethod = startLineSplit[0];

    auto httpMethod = _methodMapping[stringMethod];

    auto path = startLineSplit[1];

    lines.pop_front();

    HttpHeaders headers;

    auto hasBody = RequestHasBody(httpMethod);

    auto headerCount = 0;

    auto totalHeaders = lines.size() - (hasBody ? 1 : 0);

    for(unsigned long idx = 0; idx < totalHeaders; idx++) {
        auto key = get_left_of_delim(lines[idx], ":");
        auto value = get_right_of_delim(lines[idx], ":");

        headers.push_back(HttpHeader(key, value));
        headerCount++;
    }

    for(auto i = 0; i < headerCount; i++)
        lines.pop_front();

    auto body = lines.empty() ? "" :  lines[0];

    return new HttpRequest(path, HttpProtocol::V1_1, httpMethod, headers, body);
}

/*
static std::string temp = "HTTP/1.1 404 Not Found\r\n"
                   "Date: Mon, 16 Sep 2019 09:10:10 GMT\r\n"
                   "Connection: Keep-Alive\r\n"
                   "Content-Type: text/html\r\n"
                   "Content-Length: 0\r\n"
                   "\r\n";
*/

std::string GetReadableProtocol(HttpProtocol protocol) {

    switch (protocol) {
        case V1_0:
            return "1.0";
        default:
        case V1_1:
            return "1.1";
        case V2:
            return "2.0";
    }

}

std::string GetReadableStatusCode(HttpStatusCode code) {
    switch (code) {
        case OK:
            return "200 OK";
        default:
        case NOT_FOUND:
            return "404 NotFound";
        case INTERNAL_SERVER_ERROR:
            return "500 InternalServerError";
    }
}

Buffer *HttpParser::BufferFromResponse(HttpResponse* response) {

    std::stringstream ss;

    auto body = response->GetBody();

    ss << "HTTP/" << GetReadableProtocol(response->GetProtocol()) << " " << GetReadableStatusCode(response->GetStatusCode()) << "\r\n";
    ss << "Date: Mon, 16 Sep 2019 09:10:10 GMT\r\n"
		  "Server: cpp-webserver-eventdriven-1.0\r\n"
          "Connection: Keep-Alive\r\n"
          "Content-Type: application/json\r\n"
          "Content-Length: "<< body.length() <<"\r\n"
          "\r\n"
          << body;

    std::string responseString =   "HTTP/1.1 200 OK\r\n"
                                   "Date: Mon, 16 Sep 2019 09:10:10 GMT\r\n"
                                   "Connection: Keep-Alive\r\n"
                                   "Content-Type: application/json\r\n"
                                   "Content-Length: 13\r\n"
                                   "\r\n"
                                   "{\"obj\": true}";

    auto size = ss.str().size();

    auto buffer = new Buffer;

    buffer->Data = malloc(size);
    buffer->Size = size;

    memmove(buffer->Data, ss.str().data(), size);

    return buffer;
}
