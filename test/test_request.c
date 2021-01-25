#include "request.h"
#include <assert.h>


int main()
{
    osl_socket sock = 0;
    chttpserver_connection_t * conn = chttpserver_connection_init(chttpserver_connection_new(), sock, OSL_FLAG_READ);
    chttpserver_request_t * request = chttpserver_request_init(chttpserver_request_new(), conn, CHTTPSERVER_HTTP_1_1);
    assert(request->header != NULL);
    assert(request->header->firstline != NULL);
    assert(chttpserver_request_get_protocol_version(request) == CHTTPSERVER_HTTP_1_1);
    chttpserver_request_free(request);
    chttpserver_connection_free(conn);
    
    return 0;
}
