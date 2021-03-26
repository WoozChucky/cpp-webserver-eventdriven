//
// Created by Nuno on 3/25/2021.
//


#include <exception>

#ifndef HTTPEXCEPTION_HPP
#define HTTPEXCEPTION_HPP

class HttpException : public std::runtime_error {

public:

    virtual HttpStatusCode GetStatusCode() = 0;

protected:
    explicit HttpException(): std::runtime_error("") {}
    explicit HttpException(const std::string& reason): std::runtime_error(reason) {};

};

#endif //HTTPEXCEPTION_HPP
