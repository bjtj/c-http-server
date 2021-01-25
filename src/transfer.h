#ifndef __TRANSFER_H__
#define __TRANSFER_H__

#include <osl/osl.h>
#include "connection.h"

typedef enum _chttpserver_tranfser_type_e {
    CHTTPSERVER_TRANSFER_DEFAULT,
    CHTTPSERVER_TRANSFER_CHUNKED,
} chttpserver_tranfser_type_e;

typedef struct _chttpserver_transfer_t
{
    chttpserver_tranfser_type_e type;
    chttpserver_connection_t * connection;
} chttpserver_transfer_t;

typedef enum _chttpserver_transfer_result_e{
    CHTTPSERVER_TRANSFER_RESULT_OK,
    CHTTPSERVER_TRANSFER_RESULT_INCOMPLETE,
} chttpserver_transfer_result_e;


#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT chttpserver_transfer_t * chttpserver_transfer_new(void);
    extern OSL_EXPORT chttpserver_transfer_t * chttpserver_transfer_init(chttpserver_transfer_t *, chttpserver_connection_t *);
    extern OSL_EXPORT void chttpserver_transfer_free(chttpserver_transfer_t *);
    extern OSL_EXPORT chttpserver_transfer_result_e chttpserver_transfer_on_read(chttpserver_transfer_t *, void *, int);
    extern OSL_EXPORT chttpserver_transfer_result_e chttpserver_transfer_on_write(chttpserver_transfer_t *, void *, int);

#ifdef __cplusplus
}
#endif

#endif /* __TRANSFER_H__ */
