//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP

#define TRACE(fmt, ...) \
    fprintf(stderr, "[DBG - %s] %s:%d:: " fmt "\n", __TIME__, __PRETTY_FUNCTION__, __LINE__,  __VA_ARGS__)

#endif //LOGGER_HPP
