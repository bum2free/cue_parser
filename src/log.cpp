#include "log.h"
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define FMT_BUF_SIZE    128
#define LOG_BUF_SIZE    2048

static int log_show_level = LEVEL_DEBUG;

const char* LEVEL_PREFIX[LEVEL_ERROR + 1] = {"DETAIL", "DEBUG", "INFO", "WARN", "ERROR"};
const char LEVEL_COLOR[LEVEL_ERROR + 1]   = {39, 39, 39, 33, 31};

static char n_fmt[FMT_BUF_SIZE];
static char buffer[LOG_BUF_SIZE];

void log(int level, const char* tag, const char* fmt, ...) {
    if(level < log_show_level)
        return;

    time_t t;
    time(&t);
    struct tm* lt = localtime(&t); 

    snprintf(n_fmt, FMT_BUF_SIZE, "\033[%02dm[%d-%02d-%02d %02d:%02d:%02d][%d] %s: [%s] %s\n\033[0m",
            LEVEL_COLOR[level],
            lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, (lt->tm_hour + 8) % 24, 
            lt->tm_min, lt->tm_sec, getpid(), LEVEL_PREFIX[level], tag, fmt);

    va_list argp;
    va_start(argp, fmt);
    int cnt = vsnprintf(buffer, LOG_BUF_SIZE, n_fmt, argp);
    write(1, buffer, cnt);
    va_end(argp);
}
