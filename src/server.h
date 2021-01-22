#ifndef __SERVER_H__
#define __SERVER_H__

#include <osl/osl.h>
#include <osl/socket.h>
#include <osl/thread.h>
#include <osl/list.h>
#include <osl/thread_pool.h>
#include <osl/selector.h>
#include "header.h"

struct _chttpserver_server_t;
struct _chttpserver_connection_t;

typedef void (*chttpserver_server_on_connect_cb)(struct _chttpserver_connection_t *);
typedef void (*chttpserver_server_on_close_cb)(struct _chttpserver_connection_t *);

typedef struct _chttpserver_server_t {
    osl_bool done;
    osl_thread_t * thread;
    char * host;
    int port;
    osl_socket sock;
    chttpserver_server_on_connect_cb on_connect;
    chttpserver_server_on_close_cb on_close;
    osl_list_t * client_list;
    osl_thread_pool_t * pool;
    osl_selector_t * selector;
} chttpserver_server_t;

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT chttpserver_header_t * chttpserver_read_header(osl_socket);
    extern OSL_EXPORT chttpserver_server_t * chttpserver_new(void);
    extern OSL_EXPORT chttpserver_server_t * chttpserver_init(chttpserver_server_t *, const char *, int, int);
    extern OSL_EXPORT osl_bool chttpserver_start(chttpserver_server_t *);
    extern OSL_EXPORT osl_bool chttpserver_start_async(chttpserver_server_t *);
    extern OSL_EXPORT void chttpserver_stop(chttpserver_server_t *);
    extern OSL_EXPORT void chttpserver_free(chttpserver_server_t *);

#ifdef __cplusplus
}
#endif

#endif /* SERVER_H */
