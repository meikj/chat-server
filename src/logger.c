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
				printf("[DEBUG]: %s: %d\n", message, error_code);
			break;
		case LOGGER_INFO:
			if(LOGGER_LEVEL <= LOGGER_INFO)
				printf("[INFO]: %s: %d\n", message, error_code);
			break;
		case LOGGER_WARNING:
			if(LOGGER_LEVEL <= LOGGER_WARNING)
				printf("[WARNING]: %s: %d\n", message, error_code);
			break;
		case LOGGER_ERROR:
			if(LOGGER_LEVEL <= LOGGER_ERROR)
				printf("[ERROR]: %s: %d\n", message, error_code);
			break;
		default:
			printf("[MESSAGE]: %s: %d\n", message, error_code);
			break;
	}
}
