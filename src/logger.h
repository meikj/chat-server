#ifndef LOGGER_H

	#define LOGGER_H
	
	#define LOGGER_DEBUG	1
	#define LOGGER_INFO 	2
	#define LOGGER_WARNING	3
	#define LOGGER_ERROR	4
	
	#define LOGGER_LEVEL	LOGGER_DEBUG
	#define BUFFER_SIZE		256
	
	void log_entry(int level, int error_code, const char *format, ...);
	void _print_entry(const char *prefix, const char *message, int error_code);
	void _file_entry();
	
#endif