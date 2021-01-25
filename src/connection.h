#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <osl/socket.h>
#include <osl/selector.h>

typedef struct _chttpserver_connection_t
{
    osl_selector_t * selector;
    osl_socket remote_sock;
} chttpserver_connection_t;

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT chttpserver_connection_t * chttpserver_connection_new(void);
    extern OSL_EXPORT chttpserver_connection_t * chttpserver_connection_init(chttpserver_connection_t *, osl_socket, int);
    extern OSL_EXPORT void chttpserver_connection_free(chttpserver_connection_t *);
    extern OSL_EXPORT void chttpserver_connection_close(chttpserver_connection_t *);
    extern OSL_EXPORT int chttpserver_connection_select(chttpserver_connection_t *, int);
    extern OSL_EXPORT osl_bool chttpserver_connection_is_writable(chttpserver_connection_t *);
    extern OSL_EXPORT osl_bool chttpserver_connection_is_readable(chttpserver_connection_t *);
    extern OSL_EXPORT int chttpserver_connection_send(chttpserver_connection_t *, const void *, int);
    extern OSL_EXPORT int chttpserver_connection_recv(chttpserver_connection_t *, void *, int);


#ifdef __cplusplus
}
#endif

#endif /* __CONNECTION_H__ */
