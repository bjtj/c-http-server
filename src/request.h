#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <osl/socket.h>
#include "header.h"

typedef struct _chttpserver_request_t {
    chttpserver_header_t * header;
    osl_socket remote_sock;
} chttpserver_request_t;

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT chttpserver_request_t * chttpserver_request_new();
    extern OSL_EXPORT void chttpserver_request_init(chttpserver_request_t *, chttpserver_header_t *, osl_socket);
    extern OSL_EXPORT void chttpserver_request_free(chttpserver_request_t *);

    extern OSL_EXPORT const char * chttpserver_request_get_method(chttpserver_request_t *);
    extern OSL_EXPORT const char * chttpserver_request_get_uri(chttpserver_request_t *);
    extern OSL_EXPORT const char * chttpserver_request_get_protocol(chttpserver_request_t *);
    

#ifdef __cplusplus
}
#endif

#endif /* __REQUEST_H__ */