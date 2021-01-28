#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <osl/socket.h>
#include "header.h"
#include "transfer.h"
#include "connection.h"

typedef struct _chttpserver_request_t {
    chttpserver_header_t * header;
    chttpserver_connection_t * connection;
    chttpserver_transfer_t * transfer;
} chttpserver_request_t;

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT chttpserver_request_t * chttpserver_request_new();
    extern OSL_EXPORT chttpserver_request_t * chttpserver_request_init(chttpserver_request_t *, chttpserver_connection_t *, chttpserver_protocol_version_e);
    extern OSL_EXPORT chttpserver_request_t * chttpserver_request_init_with_header(chttpserver_request_t *, chttpserver_connection_t *, chttpserver_header_t *);
    
    extern OSL_EXPORT void chttpserver_request_free(chttpserver_request_t *);

    extern OSL_EXPORT const char * chttpserver_request_get_method(chttpserver_request_t *);
    extern OSL_EXPORT void chttpserver_request_set_method(chttpserver_request_t *, const char *);
    extern OSL_EXPORT const char * chttpserver_request_get_uri(chttpserver_request_t *);
    extern OSL_EXPORT void chttpserver_request_set_uri(chttpserver_request_t *, const char *);
    extern OSL_EXPORT const char * chttpserver_request_get_protocol(chttpserver_request_t *);
    extern OSL_EXPORT void chttpserver_request_set_protocol(chttpserver_request_t *, const char *);
    extern OSL_EXPORT chttpserver_protocol_version_e chttpserver_request_get_protocol_version(chttpserver_request_t *);
    extern OSL_EXPORT const char * chttpserver_request_get_transfer_value(chttpserver_request_t *);
    extern OSL_EXPORT void chttpserver_request_set_transfer_value(chttpserver_request_t *, const char *);
    extern OSL_EXPORT void chttpserver_request_set_transfer_type(chttpserver_request_t *, chttpserver_transfer_type_e);
    extern OSL_EXPORT void chttpserver_request_set_transfer_types(chttpserver_request_t *, osl_list_t *);
    extern OSL_EXPORT const char * chttpserver_request_get_connection_value(chttpserver_request_t *);
    extern OSL_EXPORT void chttpserver_request_set_connection_value(chttpserver_request_t *, const char *);
    extern OSL_EXPORT osl_bool chttpserver_request_is_chunked_transfer(chttpserver_request_t *);
    extern OSL_EXPORT osl_bool chttpserver_request_is_keep_alive(chttpserver_request_t *);
    extern OSL_EXPORT void chttpserver_request_set_keep_alive(chttpserver_request_t *, osl_bool);

#ifdef __cplusplus
}
#endif

#endif /* __REQUEST_H__ */
