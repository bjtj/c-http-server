#include <osl/str.h>
#include "router.h"


static osl_bool _compare_node_pattern(chttpserver_router_node_t * node, const char * pattern)
{
    return osl_string_equals(node->pattern, pattern);
}

chttpserver_router_node_t * chttpserver_router_node_new(void)
{
    OSL_NEW(chttpserver_router_node_t, node);
    return node;
}

chttpserver_router_node_t * chttpserver_router_node_init(chttpserver_router_node_t * node, const char * pattern, chttpserver_route_handler_cb handler)
{
    node->pattern = osl_strdup(pattern);
    node->handler = handler;
    return node;
}

void chttpserver_router_node_free(chttpserver_router_node_t * node)
{
    osl_safe_free(node->pattern);
    osl_safe_free(node);
}


chttpserver_router_t * chttpserver_router_new(void)
{
    OSL_NEW(chttpserver_router_t, router);
    return router;
}

chttpserver_router_t * chttpserver_router_init(chttpserver_router_t * router)
{
    return router;
}

void chttpserver_router_free(chttpserver_router_t * router)
{
    osl_list_free(router->table, (osl_free_cb)chttpserver_router_node_free);
    osl_safe_free(router);
}

osl_bool chttpserver_router_register(chttpserver_router_t * router, const char * pattern, chttpserver_route_handler_cb handler)
{
    if (chttpserver_router_get_handler(router, pattern) != NULL) {
	return osl_false;
    }
    router->table = osl_list_append(router->table, chttpserver_router_node_init(chttpserver_router_node_new(), pattern, handler));
    return osl_true;
}

osl_bool chttpserver_router_unregister(chttpserver_router_t * router, const char * pattern)
{
    if (chttpserver_router_get_handler(router, pattern) == NULL) {
	return osl_false;
    }
    router->table = osl_list_remove_if(router->table, (osl_compare_cb)_compare_node_pattern, (void*)pattern, (osl_free_cb)chttpserver_router_node_free);
    return osl_true;
}

chttpserver_route_handler_cb chttpserver_router_get_handler(chttpserver_router_t * router, const char * pattern)
{
    chttpserver_router_node_t * node = (chttpserver_router_node_t*)osl_list_find(router->table, (osl_compare_cb)_compare_node_pattern, (void*)pattern);
    return node ? node->handler : NULL;
}

