#ifndef __SERVER_H__
#define __SERVER_H__

#include <osl/osl.h>
#include <osl/socket.h>
#include <osl/thread.h>
#include <osl/list.h>

struct _chttpserver_server_t;

typedef void (*chttpserver_server_on_connect_cb)(struct _chttpserver_server_t * server, osl_socket);
typedef void (*chttpserver_server_on_close_cb)(struct _chttpserver_server_t * server, osl_socket);

typedef struct _chttpserver_server_t {
    osl_bool done;
    osl_thread_t * thread;
    char * host;
    int port;
    osl_socket sock;
    chttpserver_server_on_connect_cb on_connect;
    chttpserver_server_on_close_cb on_close;
    osl_list_t * client_list;
} chttpserver_server_t;

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT chttpserver_server_t * chttpserver_new(const char * host, int port);
    extern OSL_EXPORT osl_bool chttpserver_start(chttpserver_server_t * server);
    extern OSL_EXPORT osl_bool chttpserver_start_async(chttpserver_server_t * server);
    extern OSL_EXPORT void chttpserver_stop(chttpserver_server_t * server);
    extern OSL_EXPORT void chttpserver_free(chttpserver_server_t * server);

#ifdef __cplusplus
}
#endif

#endif /* SERVER_H */
