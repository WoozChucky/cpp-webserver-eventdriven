//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#include <Http/HttpParser.hpp>
#include <deque>
#include <cstring>

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
    return method == ::POST;
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

Buffer *HttpParser::BufferFromResponse(HttpResponse* response) {

    std::string responseString =   "HTTP/1.1 200 OK\r\n"
                                   "Date: Mon, 16 Sep 2019 09:10:10 GMT\r\n"
                                   "Connection: Keep-Alive\r\n"
                                   "Content-Type: application/json\r\n"
                                   "Content-Length: 13\r\n"
                                   "\r\n"
                                   "{\"obj\": true}";

    auto size = responseString.size();

    auto buffer = new Buffer;

    buffer->Data = malloc(size);
    buffer->Size = size;

    memmove(buffer->Data, responseString.data(), size);

    return buffer;
}
