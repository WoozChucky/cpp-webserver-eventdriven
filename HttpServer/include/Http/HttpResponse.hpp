//
// Created by Nuno Levezinho Silva on 17/09/2019.
//

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <Socket/Sugar.hpp>
#include <Http/HttpStatusCode.hpp>
#include <Http/HttpHeader.hpp>
#include <Http/HttpProtocol.hpp>
#include <Http/HttpContext.hpp>


class HttpResponse {

public:

    explicit HttpResponse();

    U16 GetBufferSize();
    Memory GetBuffer();

    void SetBody(std::string body);
    void SetStatusCode(HttpStatusCode code);
    void SetProtocol(HttpProtocol protocol);
    void AddHeader(const HttpHeader& header);
private:

    HttpStatusCode _code;
    HttpProtocol _protocol;
    HttpHeaders _headers;
    HttpContext _context;
    std::string _body;
};

#endif //HTTPRESPONSE_HPP
