#ifndef __ROUTER_H__
#define __ROUTER_H__

#include <osl/osl.h>
#include <osl/list.h>
#include "request.h"
#include "response.h"

struct _chttpserver_server_t;

typedef void (*chttpserver_route_handler_cb)(struct _chttpserver_server_t *, chttpserver_request_t *, chttpserver_response_t *);

typedef struct _chttpserver_router_node_t
{
    char * pattern;
    chttpserver_route_handler_cb handler;
} chttpserver_router_node_t;

typedef struct _chttpserver_router_t
{
    osl_list_t * table;
} chttpserver_router_t;


#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT chttpserver_router_node_t * chttpserver_router_node_new(void);
    extern OSL_EXPORT chttpserver_router_node_t * chttpserver_router_node_init(chttpserver_router_node_t *, const char *, chttpserver_route_handler_cb);
    extern OSL_EXPORT void chttpserver_router_node_free(chttpserver_router_node_t *);
    
    extern OSL_EXPORT chttpserver_router_t * chttpserver_router_new(void);
    extern OSL_EXPORT chttpserver_router_t * chttpserver_router_init(chttpserver_router_t *);
    extern OSL_EXPORT void chttpserver_router_free(chttpserver_router_t *);
    extern OSL_EXPORT osl_bool chttpserver_router_register(chttpserver_router_t *, const char *, chttpserver_route_handler_cb);
    extern OSL_EXPORT osl_bool chttpserver_router_unregister(chttpserver_router_t *, const char *);
    extern OSL_EXPORT chttpserver_route_handler_cb chttpserver_router_get_handler(chttpserver_router_t *, const char *);
    

#ifdef __cplusplus
}
#endif

#endif /* __ROUTER_H__ */
