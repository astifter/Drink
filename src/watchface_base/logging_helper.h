#pragma once

#ifdef LOG_TESTING
#define LOG_FUNC()
#else
#include <pebble.h>

#define LOG_NONE        0x00000000
#define LOG_ALL         0xFFFFFFFF
#define LOG_FUNCTIONS   0x00000001
#define LOG_FACEUPDATE  0x00000002
#define LOG_BATTERY     0x00000004
#define LOG_APPSYNC     0x00000008
#define LOG_STORAGE     0x00000030
#define LOG_STORAGE_SU  0x00000020
#define LOGENABLE       LOG_NONE

#define LOG_EXT(WHAT,FMT,ARGS...) \
    if (((LOGENABLE) & (WHAT)) == (WHAT)) { \
        app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "%s:" FMT, #WHAT, ARGS); \
    }
#define LOG(WHAT,TEXT) \
    if (((LOGENABLE) & (WHAT)) == (WHAT)) { \
        app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "%s:%s" , #WHAT, TEXT); \
    }

#define LOG_FUNC() \
    if (((LOGENABLE) & (LOG_FUNCTIONS)) == (LOG_FUNCTIONS)) { \
        app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "%s()", __PRETTY_FUNCTION__); \
    }
#define LOG_FUNC_END() \
    if (((LOGENABLE) & (LOG_FUNCTIONS)) == (LOG_FUNCTIONS)) { \
        app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, "%s() exiting", __PRETTY_FUNCTION__); \
    }
#endif

char* logging_error_to_text(int error);
