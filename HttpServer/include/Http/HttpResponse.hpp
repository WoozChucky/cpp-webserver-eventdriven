//
// Created by Nuno Levezinho Silva on 17/09/2019.
//

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <Socket/Sugar.hpp>
#include <Http/HttpStatusCode.hpp>

class HttpResponse {

public:

    struct Memoria {
        Memory Get();
        U16 Size();
    };

    Memoria Memory;

private:

    HttpStatusCode _code;
};

#endif //HTTPRESPONSE_HPP
