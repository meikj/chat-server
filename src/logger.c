#include <stdio.h>

#include "logger.h"

void log_entry(char *message, int level) {
	switch(level) {
		case LOGGER_DEBUG:
			printf("[DEBUG]: %s\n", message);
			break;
		case LOGGER_INFO:
			printf("[INFO]: %s\n", message);
			break;
		case LOGGER_WARNING:
			printf("[WARNING]: %s\n", message);
			break;
		case LOGGER_ERROR:
			printf("[ERROR]: %s\n", message);
			break;
		default:
			printf("[WARNING]: Invalid logger level passed\n");
			printf("%s\n", message);
			break;
	}
}
