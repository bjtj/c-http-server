#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#include <osl/socket.h>
#include "header.h"

typedef struct _chttpserver_response_t {
    chttpserver_header_t * header;
    osl_socket remote_sock;
} chttpserver_response_t;

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT chttpserver_response_t * chttpserver_response_new();
    extern OSL_EXPORT void chttpserver_response_init(chttpserver_response_t *, chttpserver_header_t *, osl_socket);
    extern OSL_EXPORT void chttpserver_response_free(chttpserver_response_t *);

    extern OSL_EXPORT const char * chttpserver_response_get_method(chttpserver_response_t *);
    extern OSL_EXPORT const char * chttpserver_response_get_uri(chttpserver_response_t *);
    extern OSL_EXPORT const char * chttpserver_response_get_protocol(chttpserver_response_t *);
    

#ifdef __cplusplus
}
#endif

#endif /* __RESPONSE_H__ */
