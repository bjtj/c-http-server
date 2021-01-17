#include <osl/thread.h>
#include <osl/str.h>
#include <osl/string_buffer.h>
#include "server.h"
#include "header.h"
#include "request.h"
#include "response.h"

static chttpserver_header_t * _read_header(chttpserver_server_t * server, osl_socket sock)
{
    chttpserver_header_t * header = NULL;
    osl_string_buffer_t * sb = osl_string_buffer_init(osl_string_buffer_new());
    char ch;
    int len;

    (void)server;

    while (1) {
	len = osl_socket_recv(sock, &ch, 1, 0);
	if (len == 0) {
	    break;
	}
	osl_string_buffer_append_buffer(sb, &ch, 1);
	if (sb->len > 4 && osl_strcmp(sb->ptr + sb->len - 4, "\r\n\r\n") == 0) {
	    header = chttpserver_header_from_str(sb->ptr);
	    break;
	}
    }
    osl_string_buffer_free(sb);
    return header;
}

chttpserver_header_t * chttpserver_read_header(osl_socket sock)
{
    return _read_header(NULL, sock);
}

static chttpserver_response_t * _route(chttpserver_request_t * req)
{
    chttpserver_header_t * header = chttpserver_header_init(chttpserver_header_new(), NULL);
    chttpserver_response_t * res = chttpserver_response_init(chttpserver_response_new(), header, req->remote_sock);
    
    const char * uri = chttpserver_request_get_uri(req);
    /* TODO: response init - new header */
    
    if (osl_string_equals(uri, "/")) {
	chttpserver_response_set_status(res, 200);
	chttpserver_response_set_content(res, "text/plain", "Hello");
    } else {
	chttpserver_response_set_status(res, 404);
	chttpserver_response_set_content(res, "text/plain", "Not Found");
    }
    return res;
}

static void _on_connect(chttpserver_server_t * server, osl_socket client)
{
    /* read request header */
    /* route request */
    /* response header */
    /* response body */
    /* handling keep connect */

    chttpserver_header_t * header = _read_header(server, client);
    if (header) {
	chttpserver_response_t * res;
	chttpserver_request_t * req = chttpserver_request_init(chttpserver_request_new(), header, client);
	res = _route(req);
	if (res) {
	    char * header_str = chttpserver_header_to_str(res->header);
	    osl_socket_send(req->remote_sock, header_str, strlen(header_str), 0);
	    if (res->content) {
		char * content = res->content;
		osl_socket_send(req->remote_sock, content, strlen(content), 0);
	    }
	    osl_safe_free(header_str);
	    chttpserver_response_free(res);
	}
	chttpserver_request_free(req);
    }

    if (server->on_close) {
	server->on_close(server, client);
    }
    
    osl_socket_close(client);
}


static void _on_close(chttpserver_server_t * server, osl_socket client)
{
    /*  */
    (void)server;
    (void)client;
}


static osl_bool _bind(chttpserver_server_t * server, const char * host, int port)
{
    int backlog = 5;
    osl_inet_address_t * addr;
    if ((addr = osl_inet_address_init(osl_inet_address_new(), osl_inet_unspec, host, port)) == NULL) {
	/* TODO: exception */
	fprintf(stderr, "osl_inet_address_new() failed\n");
	return osl_false;
    }
    server->sock = osl_socket_bind(addr, osl_true);
    if (!osl_socket_is_valid(server->sock)) {
	fprintf(stderr, "invalid socket\n");
	return osl_false;
    }
    osl_inet_address_free(addr);

    addr = osl_socket_get_inet_address(server->sock);
    if (addr == NULL) {
	/* TODO: exception */
	fprintf(stderr, "osl_socket_get_inet_address() failed\n");
	return osl_false;
    }
    server->port = addr->port;
    osl_inet_address_free(addr);

    if (osl_socket_listen(server->sock, backlog) != 0)
    {
	perror("listen() failed");
	return osl_false;
    }
    return osl_true;
}

static osl_bool _poll(chttpserver_server_t * server)
{
    osl_socket client;
    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len = sizeof(remote_addr);
    memset(&remote_addr, 0, sizeof(remote_addr));
    client = osl_socket_accept(server->sock, (struct sockaddr*)&remote_addr, &remote_addr_len);
    if (!osl_socket_is_valid(client)) {
	/* TODO: exception */
	return osl_false;
    }
    if (server->on_connect) {
	server->on_connect(server, client);
    } else {
	/* TODO: send dummy message before close */
	osl_socket_close(client);
    }
    return osl_true;
}

static void _run(chttpserver_server_t * server)
{
    while (!server->done) {
	if (_poll(server) == osl_false) {
	    fprintf(stderr, "_poll() failed\n");
	    break;
	}
	osl_idle(10);
    }
    osl_socket_close(server->sock);
}

static void * _runner(void * arg)
{
    chttpserver_server_t * server = (chttpserver_server_t *)arg;
    if (server == NULL) {
	return NULL;
    }

    _run(server);

    return NULL;
}

static osl_bool _start_server(chttpserver_server_t * server)
{
    if (server == NULL) {
	return osl_false;
    }

    server->done = osl_false;

    if (_bind(server, server->host, server->port) == osl_false) {
	return osl_false;
    }

    return osl_true;
}

chttpserver_server_t * chttpserver_new(void)
{
    chttpserver_server_t * server = (chttpserver_server_t*)malloc(sizeof(chttpserver_server_t));
    if (server == NULL) {
	return NULL;
    }
    return server;
}

chttpserver_server_t * chttpserver_init(chttpserver_server_t * server, const char * host, int port)
{
    server->sock = OSL_INVALID_SOCKET;
    server->host = osl_safe_strdup(host);
    server->port = port;
    server->on_connect = _on_connect;
    server->on_close = _on_close;
    return server;
}

osl_bool chttpserver_start(chttpserver_server_t * server) {

    if (_start_server(server) == osl_false) {
	return osl_false;
    }

    _run(server);
    
    return osl_true;
}

osl_bool chttpserver_start_async(chttpserver_server_t * server) {

    if (_start_server(server) == osl_false) {
	return osl_false;
    }

    server->thread = osl_thread_init(osl_thread_new(), _runner, (void*)server);
    osl_thread_start(server->thread);
    
    return osl_true;
}

void chttpserver_stop(chttpserver_server_t * server)
{
    server->done = osl_true;
    if (server->thread) {
	osl_thread_join(server->thread);
	osl_thread_free(server->thread);
	server->thread = NULL;
    }
}


void chttpserver_free(chttpserver_server_t * server)
{
    if (server == NULL) {
	return;
    }
    osl_safe_free(server->host);
    osl_safe_free(server);
}
