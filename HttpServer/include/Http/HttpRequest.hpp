//
// Created by Nuno Levezinho Silva on 17/09/2019.
//

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <Http/HttpMethod.hpp>
#include <Http/HttpHeader.hpp>
#include <Http/HttpProtocol.hpp>
#include <Http/HttpContext.hpp>
#include <vector>

class HttpRequest {

public:

    explicit HttpRequest(std::string path,
            HttpProtocol protocol,
            HttpMethod method,
            HttpHeaders headers,
            std::string body);

    [[nodiscard]] const HttpMethod& GetMethod() const;
    [[nodiscard]] const HttpProtocol& GetProtocol() const;
    [[maybe_unused]] [[nodiscard]] const HttpHeaders& GetHeaders() const;
    [[nodiscard]] std::string GetHeader(const std::string& key) const;
    [[nodiscard]] const std::string& GetBody() const;
    [[nodiscard]] const std::string& GetPath() const;
    bool HasBody() const;

private:

    std::string _path;
    HttpProtocol _protocol;
    HttpMethod _method;
    HttpHeaders _headers;
    HttpContext _context;
    std::string _body;

};

#endif //HTTPREQUEST_HPP
