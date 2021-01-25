#include "response.h"
#include <assert.h>

int main()
{
    osl_socket sock = 0;
    chttpserver_connection_t * conn = chttpserver_connection_init(chttpserver_connection_new(), sock, OSL_FLAG_READ);
    chttpserver_response_t * response = chttpserver_response_init(chttpserver_response_new(), conn, CHTTPSERVER_HTTP_1_1);
    assert(response->header != NULL);
    assert(response->header->firstline != NULL);
    assert(chttpserver_response_get_protocol_version(response) == CHTTPSERVER_HTTP_1_1);
    chttpserver_response_free(response);
    chttpserver_connection_free(conn);
    
    return 0;
}
