#include <assert.h>
#include <string.h>
#include <osl/str.h>
#include "server.h"
#include "response.h"
#include "connection.h"

chttpserver_server_t * server;

static void _index(chttpserver_server_t * server, chttpserver_request_t * req, chttpserver_response_t * res)
{
    (void)server;
    (void)req;
    chttpserver_response_set_status(res, 200);
    chttpserver_response_set_content(res, "text/plain", "Hello");
}

void start_server()
{
    server = chttpserver_init(chttpserver_new(), "127.0.0.1", 0, 10);
    assert(server != NULL);
    assert(chttpserver_start_async(server) == osl_true);
    assert(osl_socket_is_valid(server->sock));
    assert(server->port > 0);

    chttpserver_router_register(server->router, "/", _index);
}

void stop_server()
{
    chttpserver_stop(server);
    assert(server->thread == NULL);
    chttpserver_free(server);
}

static void _request_helper(const char * header, const char * body, int ret_code)
{
    chttpserver_header_t * response_header;
    osl_inet_address_t * addr = osl_inet_address_init(osl_inet_address_new(), osl_inet_unspec, "127.0.0.1", server->port);
    osl_socket sock = osl_socket_connect(addr);
    chttpserver_connection_t * conn = chttpserver_connection_init(chttpserver_connection_new(), sock, OSL_FLAG_READ | OSL_FLAG_WRITE);
    chttpserver_response_t * res;
    assert(osl_socket_is_valid(sock));
    assert(osl_socket_send(sock, header, strlen(header), 0) == (int)strlen(header));
    if (body) {
	assert(chttpserver_connection_send(conn, body, strlen(body)) == (int)strlen(body));
    }
    response_header = chttpserver_read_header(server, conn);
    assert(response_header != NULL);
    assert(strcmp(response_header->firstline->part1, "HTTP/1.1") == 0);

    res = chttpserver_response_init_with_header(chttpserver_response_new(), conn, response_header);
    assert(chttpserver_response_get_status_code(res) == ret_code);
    chttpserver_response_free(res);
    osl_inet_address_free(addr);
    osl_socket_close(sock);
    chttpserver_connection_free(conn);
}



int main()
{
    osl_init_once();
    osl_ignore_sigpipe();
    start_server();

    {
	const char * header = "GET / HTTP/1.1\r\n\r\n";
	const char * body = NULL;
	_request_helper(header, body, 200);
    }

    {
	const char * header = "GET /qweqwe HTTP/1.1\r\n\r\n";
	const char * body = NULL;
	_request_helper(header, body, 404);
    }

    stop_server();
    osl_finish();
    return 0;
}
