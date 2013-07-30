#ifndef PROTOCOL_H

	#define PROTOCOL_H

	void protocol_parse(const int client_id, const char *message, const size_t len);
	void protocol_get_clients(const int client_id);

#endif