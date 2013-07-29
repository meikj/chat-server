#ifndef LOGGER_H

	#define LOGGER_H

	#define LOGGER_DEBUG	1
	#define LOGGER_INFO 	2
	#define LOGGER_WARNING	3
	#define LOGGER_ERROR	4

	#define LOGGER_LEVEL	LOGGER_DEBUG // Change to set minimum logger level
	#define LOGGER_BUF_SIZE	256

	void log_entry(int level, const char *format, ...);
	void _print_entry(const char *prefix, const char *message);
	void _file_entry();

	// Kind of wrapper functions to simplify logging
	#define log(message, args...) log_entry(0, message, args)
	#define log_debug(message, args...) log_entry(LOGGER_DEBUG, message, args)
	#define log_info(message, args...) log_entry(LOGGER_INFO, message, args)
	#define log_warn(message, args...) log_entry(LOGGER_WARNING, message, args)
	#define log_error(message, args...) log_entry(LOGGER_ERROR, message, args)

#endif
