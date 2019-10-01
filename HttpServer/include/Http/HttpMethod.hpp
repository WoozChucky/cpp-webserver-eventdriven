//
// Created by Nuno Levezinho Silva on 17/09/2019.
//

#ifndef HTTPMETHOD_HPP
#define HTTPMETHOD_HPP

enum HttpMethod {
    GET         = 0x00,
    HEAD        = 0x01,
    POST        = 0x02,
    PUT         = 0x03,
    DELETE      = 0x04,
    CONNECT     = 0x05,
    OPTIONS     = 0x06,
    TRACE       = 0x07,
    PATCH       = 0x08
};

#endif //HTTPMETHOD_HPP
