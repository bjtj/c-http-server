#ifndef __RESPONSE_H__
#define __RESPONSE_H__

#include <osl/socket.h>
#include "header.h"
#include "protocol.h"
#include "transfer.h"

typedef struct _chttpserver_response_t {
    chttpserver_header_t * header;
    osl_socket remote_sock;
    char * content;
    chttpserver_transfer_t * transfer;
} chttpserver_response_t;

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT chttpserver_response_t * chttpserver_response_new();
    extern OSL_EXPORT chttpserver_response_t * chttpserver_response_init(chttpserver_response_t *, chttpserver_header_t *, osl_socket);
    extern OSL_EXPORT void chttpserver_response_free(chttpserver_response_t *);

    extern OSL_EXPORT const char * chttpserver_response_get_protocol(chttpserver_response_t *);
    extern OSL_EXPORT int chttpserver_response_get_status_code(chttpserver_response_t *);
    extern OSL_EXPORT const char * chttpserver_response_get_status_message(chttpserver_response_t *);

    extern OSL_EXPORT osl_bool chttpserver_response_set_protocol(chttpserver_response_t *, chttpserver_protocol_version_e);
    extern OSL_EXPORT osl_bool chttpserver_response_set_status(chttpserver_response_t *, int);
    extern OSL_EXPORT osl_bool chttpserver_response_set_status_code(chttpserver_response_t *, int);
    extern OSL_EXPORT osl_bool chttpserver_response_set_status_message(chttpserver_response_t *, const char *);

    extern OSL_EXPORT osl_bool chttpserver_response_set_content(chttpserver_response_t *, const char *, const char *);
    

#ifdef __cplusplus
}
#endif

#endif /* __RESPONSE_H__ */
