#include <assert.h>
#include <string.h>
#include <osl/str.h>
#include "server.h"
#include "response.h"

chttpserver_server_t * server;

void start_server()
{
    server = chttpserver_init(chttpserver_new(), "127.0.0.1", 0);
    assert(server != NULL);
    assert(chttpserver_start_async(server) == osl_true);
    assert(osl_socket_is_valid(server->sock));
    assert(server->port > 0);
}

void stop_server()
{
    chttpserver_stop(server);
    assert(server->thread == NULL);
    chttpserver_free(server);
}

void request(const char * header, const char * body)
{
    chttpserver_header_t * response_header;
    osl_inet_address_t * addr = osl_inet_address_init(osl_inet_address_new(), osl_inet_unspec, "127.0.0.1", server->port);
    osl_socket sock = osl_socket_connect(addr);
    chttpserver_response_t * res;
    assert(osl_socket_is_valid(sock));
    assert(osl_socket_send(sock, header, strlen(header), 0) == (int)strlen(header));
    if (body) {
	assert(osl_socket_send(sock, body, strlen(header), 0) == (int)strlen(body));
    }
    response_header = chttpserver_read_header(sock);
    assert(response_header != NULL);

    res = chttpserver_response_init(chttpserver_response_new(), response_header, sock);
    assert(chttpserver_response_get_status_code(res) == 200);
    chttpserver_response_free(res);
    osl_inet_address_free(addr);
    osl_socket_close(sock);
}


int main()
{
    start_server();

    {
	const char * header = "GET / HTTP/1.1\r\n\r\n";
	const char * body = NULL;
	request(header, body);
    }

    stop_server();
    
    return 0;
}
