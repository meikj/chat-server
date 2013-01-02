/*
 * logger.c
 *
 * Used for logging to the standard output and/or files.
 * Contains different levels of logging which can be controlled
 * by editing the logger header file.
 *
 */

#include <stdio.h>

#include "logger.h"

/*
 * Add an entry to the log. At the moment support is only present
 * for the standard output.
 */
void log_entry(const char *message, int error_code, int level) {
	switch(level) {
		case LOGGER_DEBUG:
			if(LOGGER_LEVEL <= LOGGER_DEBUG)
				_print_entry("DEBUG", message, error_code);
			break;
		case LOGGER_INFO:
			if(LOGGER_LEVEL <= LOGGER_INFO)
				_print_entry("INFO", message, error_code);
			break;
		case LOGGER_WARNING:
			if(LOGGER_LEVEL <= LOGGER_WARNING)
				_print_entry("WARNING", message, error_code);
			break;
		case LOGGER_ERROR:
			if(LOGGER_LEVEL <= LOGGER_ERROR)
				_print_entry("ERROR", message, error_code);
			break;
		default:
			_print_entry("MESSAGE", message, error_code);
			break;
	}
}

/*
 * Used to print a log message to the standard output. Should only be used
 * internally by log_entry().
 */
void _print_entry(const char *prefix, const char *message, int error_code) {
	if(error_code == 0)
		printf("[%s]: %s\n", prefix, message);
	else
		printf("[%s]: %s: %d\n", prefix, message, error_code);
}

/*
 * File logging code will go here.
 */
void _file_entry() {
	return;
}
