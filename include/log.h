#ifndef __LOG_H__
#define __LOG_H__

#define LEVEL_DETAIL 0
#define LEVEL_DEBUG 1
#define LEVEL_INFO 2
#define LEVEL_WARN 3
#define LEVEL_ERROR 4

void log(int level, const char* tag, const char* fmt, ...);
#endif
