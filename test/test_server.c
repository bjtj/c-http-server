#include "server.h"
#include <assert.h>
#include <string.h>

chttpserver_server_t * server;

void start_server()
{
    server = chttpserver_new("127.0.0.1", 0);
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

int main()
{
    start_server();

    {
	const char * header = "GET / HTTP/1.1\r\n\r\n";
	osl_inet_address_t * addr = osl_inet_address_new(osl_inet_unspec, "127.0.0.1", server->port);
	osl_socket sock = osl_socket_connect(addr);
	assert(osl_socket_is_valid(sock));
	assert(osl_socket_send(sock, header, strlen(header), 0) == (int)strlen(header));
	assert(osl_socket_recv(sock, NULL, 0, 0) == 0);
	osl_inet_address_free(addr);
	osl_socket_close(sock);
    }

    stop_server();
    
    return 0;
}
