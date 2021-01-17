#include <osl/osl.h>
#include "server.h"
#include <assert.h>

int main(int argc, char *argv[])
{
    chttpserver_server_t * server = chttpserver_init(chttpserver_new(), "0", 8080);
    assert(server != NULL);
    assert(chttpserver_start_async(server) == osl_true);
    assert(server->port > 0);

    printf("Listening Port: %d\n", server->port);
    fgetc(stdin);
    
    chttpserver_stop(server);
    assert(server->thread == NULL);
    chttpserver_free(server);
    
    return 0;
}
