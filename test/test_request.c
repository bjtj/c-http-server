#include "request.h"
#include <assert.h>


int main()
{
    osl_socket sock = 0;
    chttpserver_request_t * request = chttpserver_request_init(chttpserver_request_new(), sock, CHTTPSERVER_HTTP_1_1);
    assert(request->header != NULL);
    assert(request->header->firstline != NULL);
    assert(chttpserver_request_get_protocol_version(request) == CHTTPSERVER_HTTP_1_1);
    chttpserver_request_free(request);
    
    return 0;
}
