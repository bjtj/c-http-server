#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <osl/osl.h>

typedef enum _chttpserver_protocol_version_e {
    CHTTPSERVER_HTTP_1_0,
    CHTTPSERVER_HTTP_1_1,
} chttpserver_protocol_version_e;

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT const char * chttpserver_protocol_version_to_str(chttpserver_protocol_version_e);
    extern OSL_EXPORT chttpserver_protocol_version_e chttpserver_protocol_version_from_str(const char *);

#ifdef __cplusplus
}
#endif


#endif /* __PROTOCOL_H__ */
