#include "response.h"

int main()
{
    osl_socket sock = 0;
    chttpserver_header_t * header = chttpserver_header_init(chttpserver_header_new(), NULL);
    chttpserver_response_t * response = chttpserver_response_init(chttpserver_response_new(), header, sock);
    chttpserver_response_free(response);
    
    return 0;
}
