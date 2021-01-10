#include "server.h"
#include <assert.h>

int main()
{
    chttpserver_server_t * server = chttpserver_start_async(0);
    assert(server != NULL);
    chttpserver_stop(server);
    
    return 0;
}
