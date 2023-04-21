#ifndef LOG_LOG_HPP
#define LOG_LOG_HPP

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>

void fatal_error(const char* tmp, ...);
void memcheck(void*, int);


enum {LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL, LOG_NL};
void log_log(int level, const char *file, int line, const char *fmt, ...);


#define log_trace(...) log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)
#define log_nl(...)    log_log(LOG_NL,      NULL,      0,     __VA_ARGS__)     


#define LOG_LOG_MAX_FILES 4
int log_log_add(FILE* file);
int log_log_remove(FILE* file);


#ifdef __NCURSES_H
#define LOG_LOG_MAX_WINDOWS 4
int log_log_add(WINDOW* window);
int log_log_remove(WINDOW* window);
#endif /* ifdef __NCURSES_H */


#include "log.cpp"

#endif /* ifndef LOG_LOG_HPP */
