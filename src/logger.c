#include <stdio.h>

#include "logger.h"

void log_entry(char *message, int error_code, int level) {
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
			printf("[WARNING]: Invalid logger level passed\n");
			printf("%s: %d\n", message, error_code);
			break;
	}
}
