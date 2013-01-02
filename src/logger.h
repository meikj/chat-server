#ifndef LOGGER_H

	#define LOGGER_H
	
	#define LOGGER_DEBUG	1
	#define LOGGER_INFO	2
	#define LOGGER_WARNING	3
	#define LOGGER_ERROR	4
	
	#define LOGGER_LEVEL	LOGGER_DEBUG
	
	void log_entry(const char *message, int error_code, int level);
	void _print_entry(const char *prefix, const char *message, int error_code);
	void _file_entry();
	
#endif