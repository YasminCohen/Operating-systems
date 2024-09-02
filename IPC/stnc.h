#if !defined(_XOPEN_SOURCE) && !defined(_POSIX_C_SOURCE)
#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif
#endif

#include <stdbool.h>

#define FILE_SIZE 104857600
#define CHUNK_SIZE 65536
#define UDP_CHUNK_SIZE 65507
#define UDS_NAME "/tmp/uds_socket_stnc.sock"
#define POLL_TIMEOUT 1000

char* md5hash(char *data);

void print_usage(char *name);

void client_chat(const char *ip, const char *port);

void server_chat(const char *port);

void client_performance(const char *ip, const char *port, const char *param1, const char *param2);

void server_performance(const char *port, bool quiet);