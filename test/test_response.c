#include "response.h"
#include <assert.h>

int main()
{
    osl_socket sock = 0;
    chttpserver_response_t * response = chttpserver_response_init(chttpserver_response_new(), sock, CHTTPSERVER_HTTP_1_1);
    assert(response->header != NULL);
    assert(response->header->firstline != NULL);
    assert(chttpserver_response_get_protocol_version(response) == CHTTPSERVER_HTTP_1_1);
    chttpserver_response_free(response);
    
    return 0;
}
