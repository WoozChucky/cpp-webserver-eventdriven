//
// Created by Nuno on 3/25/2021.
//

#ifndef NOTFOUNDEXCEPTION_HPP
#define NOTFOUNDEXCEPTION_HPP

#include <exception>
#include <Http/Exceptions/HttpException.hpp>

class NotFoundException : public HttpException {

public:
    explicit NotFoundException() : HttpException("Resource not found") { }
    explicit NotFoundException(const std::string& reason) : HttpException(reason) { }

    inline HttpStatusCode GetStatusCode() override {
        return NOT_FOUND;
    }
};

#endif //NOTFOUNDEXCEPTION_HPP
