#ifndef __TRANSFER_H__
#define __TRANSFER_H__

#include <osl/osl.h>
#include "connection.h"

typedef enum _chttpserver_transfer_type_e {
    CHTTPSERVER_TRANSFER_DEFAULT,
    CHTTPSERVER_TRANSFER_CHUNKED,
    CHTTPSERVER_TRANSFER_COMPRESS,
    CHTTPSERVER_TRANSFER_DEFLATE,
    CHTTPSERVER_TRANSFER_GZIP,
    CHTTPSERVER_TRANSFER_IDENTITY,
} chttpserver_transfer_type_e;

typedef enum _chttpserver_transfer_result_e{
    CHTTPSERVER_TRANSFER_RESULT_OK,
    CHTTPSERVER_TRANSFER_RESULT_INCOMPLETE,
    CHTTPSERVER_TRANSFER_RESULT_FAIL,
} chttpserver_transfer_result_e;

struct _chttpserver_transfer_t;

typedef chttpserver_transfer_result_e (*chttpserver_tranfser_on_read_cb)(struct _chttpserver_transfer_t *, void*, int);
typedef chttpserver_transfer_result_e (*chttpserver_tranfser_on_write_cb)(struct _chttpserver_transfer_t *, void*, int);

typedef struct _chttpserver_transfer_t
{
    chttpserver_transfer_type_e type;
    chttpserver_connection_t * connection;
    chttpserver_tranfser_on_read_cb on_read;
    chttpserver_tranfser_on_write_cb on_write;
} chttpserver_transfer_t;


#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT const char * chttpserver_transfer_type_to_str(chttpserver_transfer_type_e);
    extern OSL_EXPORT chttpserver_transfer_t * chttpserver_transfer_new(void);
    extern OSL_EXPORT chttpserver_transfer_t * chttpserver_transfer_init(chttpserver_transfer_t *, chttpserver_connection_t *, chttpserver_transfer_type_e);
    extern OSL_EXPORT void chttpserver_transfer_free(chttpserver_transfer_t *);
    extern OSL_EXPORT chttpserver_transfer_result_e chttpserver_transfer_on_read(chttpserver_transfer_t *, void *, int);
    extern OSL_EXPORT chttpserver_transfer_result_e chttpserver_transfer_on_write(chttpserver_transfer_t *, void *, int);

#ifdef __cplusplus
}
#endif

#endif /* __TRANSFER_H__ */
