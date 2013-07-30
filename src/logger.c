/*
 * logger.c
 *
 * Used for logging to the standard output and/or log file. Supports different
 * levels of logging, as well as setting a minimum log priority.
 *
 */

#include <stdio.h>
#include <stdarg.h>

#include "logger.h"

/*
 * Add an entry to the log.
 *
 * Params:
 *	level		= Log level (LOGGER_DEBUG, LOGGER_INFO, etc.)
 *	format		= Formatted string (can contain specifiers, i.e. %d, %s, etc.)
 *	... 		= Relevant arguments for format specifiers
 *
 */
void log_entry(int level, const char *format, ...) {
	char buffer[LOGGER_BUF_SIZE];
	va_list args;

	va_start(args, format);
	vsprintf(buffer, format, args);

	switch(level) {
		case LOGGER_DEBUG:
			if(LOGGER_LEVEL <= LOGGER_DEBUG)
				_print_entry("DEBUG", buffer);
			break;
		case LOGGER_INFO:
			if(LOGGER_LEVEL <= LOGGER_INFO)
				_print_entry("INFO", buffer);
			break;
		case LOGGER_WARNING:
			if(LOGGER_LEVEL <= LOGGER_WARNING)
				_print_entry("WARNING", buffer);
			break;
		case LOGGER_ERROR:
			if(LOGGER_LEVEL <= LOGGER_ERROR)
				_print_entry("ERROR", buffer);
			break;
		default:
			_print_entry(NULL, buffer);
			break;
	}

	va_end(args);
}

/*
 * Used to print a log message to the standard output. Should only be used
 * internally by log_entry().
 *
 * Params:
 *	prefix		= Log output prefix (i.e. DEBUG, INFO, etc.)
 *	message		= Log message
 *
 */
void _print_entry(const char *prefix, const char *message) {
	if(prefix == NULL)
		printf("%s", message);
	else
		printf("[%s]: %s", prefix, message);
}

/*
 * File logging code will go here.
 *
 */
void _file_entry() {
	return;
}
