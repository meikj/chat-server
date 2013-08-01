#ifndef PROTOCOL_H

#define PROTOCOL_H

void proto_parse(int id, const char *data, size_t size);
void proto_get_clients(int id);

#endif
