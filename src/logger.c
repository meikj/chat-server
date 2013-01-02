/*
 * logger.c
 *
 * Used for logging to the standard output and/or files.
 * Contains different levels of logging which can be controlled
 * by editing the logger header file.
 *
 */

#include <stdio.h>
#include <stdarg.h>

#include "logger.h"

/*
 * Add an entry to the log. At the moment support is only present
 * for the standard output.
 *
 * Params:
 *	level		= Log level (LOGGER_DEBUG, LOGGER_INFO, etc.)
 *	error_code	= Error code number, if applicable (0 for no error code)
 *	format		= Formatted string (can contain specifiers, i.e. %d, %s, etc.)
 *	... 		= Relevant arguments for format specifiers
 */
void log_entry(int level, int error_code, const char *format, ...) {
	char buffer[BUFFER_SIZE];
	va_list args;
	
	va_start(args, format);
	vsprintf(buffer, format, args);
	
	switch(level) {
		case LOGGER_DEBUG:
			if(LOGGER_LEVEL <= LOGGER_DEBUG)
				_print_entry("DEBUG", buffer, error_code);
			break;
		case LOGGER_INFO:
			if(LOGGER_LEVEL <= LOGGER_INFO)
				_print_entry("INFO", buffer, error_code);
			break;
		case LOGGER_WARNING:
			if(LOGGER_LEVEL <= LOGGER_WARNING)
				_print_entry("WARNING", buffer, error_code);
			break;
		case LOGGER_ERROR:
			if(LOGGER_LEVEL <= LOGGER_ERROR)
				_print_entry("ERROR", buffer, error_code);
			break;
		default:
			_print_entry("MESSAGE", buffer, error_code);
			break;
	}
	
	va_end(args);
}

/*
 * Used to print a log message to the standard output. Should only be used
 * internally by log_entry().
 *
 * Params:
 *		prefix		= Log output prefix (i.e. DEBUG, INFO, etc.)
 *		message		= Message to output to standard output
 *		error_code	= Error code of log, if applicable (0 for no error code)
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
