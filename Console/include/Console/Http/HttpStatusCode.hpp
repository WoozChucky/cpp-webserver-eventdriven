//
// Created by Nuno Levezinho Silva on 17/09/2019.
//

#ifndef HTTPSTATUSCODE_HPP
#define HTTPSTATUSCODE_HPP

enum HttpStatusCode : int {

    /* 1xx Status Codes */
    CONTINUE = 100,
    SWITCHING_PROTOCOLS = 101,
    EARLY_HINTS = 103,

    /* 2xx Status Codes */
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NON_AUTHORITATIVE_INFORMATION = 203,
    NO_CONTENT = 204,
    RESET_CONTENT = 205,
    PARTIAL_CONTENT = 206,

    /* 3xx Status Codes */
    MULTIPLE_CHOICES = 300,

    /* 4xx Status Codes */
    BAD_REQUEST = 400,
    NOT_FOUND = 404

    /* 5xx Status Codes */
};

#endif //HTTPSTATUSCODE_HPP
