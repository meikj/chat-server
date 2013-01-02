#include <stdio.h>

#include "logger.h"

/*
 * Add an entry to the log. At the moment support is only present
 * for the standard output.
 */
void log_entry(const char *message, int error_code, int level) {
	switch(level) {
		case LOGGER_DEBUG:
			printf("[DEBUG]: %s: %d\n", message, error_code);
			break;
		case LOGGER_INFO:
			printf("[INFO]: %s: %d\n", message, error_code);
			break;
		case LOGGER_WARNING:
			printf("[WARNING]: %s: %d\n", message, error_code);
			break;
		case LOGGER_ERROR:
			printf("[ERROR]: %s: %d\n", message, error_code);
			break;
		default:
			printf("[MESSAGE]: %s: %d\n", message, error_code);
			break;
	}
}
