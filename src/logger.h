#ifndef LOGGER_H
	#define LOGGER_H
	
	#define LOGGER_DEBUG	1
	#define LOGGER_INFO		2
	#define LOGGER_WARNING	3
	#define LOGGER_ERROR	4
	
	void log_entry(const char *message, int error_code, int level);
	
#endif