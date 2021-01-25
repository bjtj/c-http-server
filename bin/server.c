#include <osl/osl.h>
#include <assert.h>
#include "server.h"

static void _index(chttpserver_server_t * server, chttpserver_request_t * req, chttpserver_response_t * res)
{
    (void)server;
    (void)req;
    chttpserver_response_set_status(res, 200);
    chttpserver_response_set_content(res, "text/plain", "Hello");
}

int main(int argc, char *argv[])
{
    chttpserver_server_t * server = chttpserver_init(chttpserver_new(), "0", 8080, 200);
    assert(server != NULL);
    assert(chttpserver_start_async(server) == osl_true);
    assert(server->port > 0);

    chttpserver_router_register(server->router, "/", _index);

    printf("Listening Port: %d\n", server->port);
    fgetc(stdin);
    
    chttpserver_stop(server);
    assert(server->thread == NULL);
    chttpserver_free(server);
    
    return 0;
}
