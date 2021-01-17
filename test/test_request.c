#include "request.h"

int main()
{
    osl_socket sock = 0;
    chttpserver_header_t * header = chttpserver_header_init(chttpserver_header_new(), NULL);
    chttpserver_request_t * request = chttpserver_request_init(chttpserver_request_new(), header, sock);
    chttpserver_request_free(request);
    
    return 0;
}
