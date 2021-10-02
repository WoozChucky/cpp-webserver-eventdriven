//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <cstring>
#include <cstdarg>

#ifndef WINDOWS
#define TRACE(fmt, ...) \
        fprintf(stderr, "[DBG - %s] %s:%d:: " fmt "\n", __TIME__, __PRETTY_FUNCTION__, __LINE__,  __VA_ARGS__);
#else
#define TRACE(fmt, ...) \
        fprintf(stderr, "[DBG - %s] %s:%d:: " fmt "\n", __TIME__, __FUNCSIG__, __LINE__,  __VA_ARGS__);
#endif



#if DEBUG_ENABLED
// __PRETTY_FUNCTION__
#else
    #define TRACE(fmt, ...) \
     //
#endif

#endif //LOGGER_HPP
