#include <osl/thread.h>
#include <osl/str.h>
#include <osl/string_buffer.h>
#include "server.h"
#include "header.h"
#include "request.h"
#include "response.h"
#include "connection.h"
#include "router.h"


struct _on_connect_param_t {
    chttpserver_server_t * server;
    chttpserver_connection_t * conn;
};


static chttpserver_header_t * _read_header(chttpserver_server_t * server, chttpserver_connection_t * conn)
{
    chttpserver_header_t * header = NULL;
    osl_string_buffer_t * sb = osl_string_buffer_init(osl_string_buffer_new());
    char ch;
    int len;

    while (!server->done) {
	int count = chttpserver_connection_select(conn, 100);
	if (count < 0) {
	    /* TODO: error */
	    fprintf(stderr, "error\n");
	    break;
	}
	
	if (count == 0) {
	    continue;
	}

	if (chttpserver_connection_is_readable(conn)) {
	    len = chttpserver_connection_recv(conn, &ch, 1);
	    if (len == 0) {
		fprintf(stdout, "closed\n");
		break;
	    }
	    osl_string_buffer_append_buffer(sb, &ch, 1);
	    if (sb->len > 4 && osl_strcmp(sb->ptr + sb->len - 4, "\r\n\r\n") == 0) {
		header = chttpserver_header_from_str(sb->ptr);
		break;
	    }
	}
    }
    osl_string_buffer_free(sb);
    return header;
}

chttpserver_header_t * chttpserver_read_header(chttpserver_server_t * server, chttpserver_connection_t * conn)
{
    return _read_header(server, conn);
}

static void _notfound(chttpserver_server_t * server, chttpserver_request_t * req, chttpserver_response_t * res)
{
    (void)server;
    (void)req;
    chttpserver_response_set_status(res, 404);
    chttpserver_response_set_content(res, "text/plain", "Not Found");
}

static struct _on_connect_param_t * _make_handler_param(chttpserver_server_t * server, chttpserver_connection_t * conn)
{
    OSL_NEW(struct _on_connect_param_t, param);
    param->server = server;
    param->conn = conn;
    return param;
}

static void _on_connect_wrapper(struct _on_connect_param_t * param)
{
    if (param->server->on_connect) {
	param->server->on_connect(param->server, param->conn);
    }
    osl_safe_free(param);
}

static void _send_response(chttpserver_response_t * res)
{
    char * header_str = chttpserver_header_to_str(res->header);
    chttpserver_connection_send(res->connection, header_str, strlen(header_str));
    if (res->content) {
	chttpserver_connection_send(res->connection, res->content, strlen(res->content));
    }
    osl_safe_free(header_str);
}

static void _handle_request_header(chttpserver_server_t * server, chttpserver_request_t * req, chttpserver_response_t * res)
{
    chttpserver_route_handler_cb handler = chttpserver_router_get_handler(server->router, chttpserver_request_get_uri(req));
    if (handler == NULL) {
	printf("NOT FOUND\n");
	_notfound(server, req, res);
    } else {
	printf("HANDLE - '%s'\n", chttpserver_request_get_uri(req));
	handler(server, req, res);
    }

    _send_response(res);
}

static void _on_connect(chttpserver_server_t * server, chttpserver_connection_t * conn)
{
    chttpserver_header_t * header;
    osl_bool keep_alive = osl_false;
    
    do
    {
	chttpserver_request_t * req;
	chttpserver_response_t * res;
	
	header = _read_header(server, conn);
	if (header == NULL)
	{
	    break;
	}
	
	req = chttpserver_request_init_with_header(chttpserver_request_new(), conn, header);
	res = chttpserver_response_init(chttpserver_response_new(), req->connection, chttpserver_request_get_protocol_version(req));
	
	_handle_request_header(server, req, res);

	keep_alive = chttpserver_request_is_keep_alive(req);
	printf("keep alive: %s\n", keep_alive ? "Y" : "N");

	chttpserver_request_free(req);
	chttpserver_response_free(res);
	
    } while (keep_alive && !server->done);

    if (server->on_close) {
	server->on_close(server, conn);
    }

    chttpserver_connection_close(conn);
    chttpserver_connection_free(conn);
}


static void _on_close(chttpserver_server_t * server, chttpserver_connection_t * conn)
{
    /*  */
    (void)server;
    (void)conn;
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

    if (osl_selector_select(server->selector, 100) <= 0) {
	return osl_true;
    }
    
    client = osl_socket_accept(server->sock, (struct sockaddr*)&remote_addr, &remote_addr_len);
    if (!osl_socket_is_valid(client)) {
	/* TODO: exception */
	return osl_false;
    }
    if (server->on_connect) {
	chttpserver_connection_t * conn = chttpserver_connection_init(chttpserver_connection_new(), client, OSL_FLAG_READ | OSL_FLAG_WRITE);
	struct _on_connect_param_t * param =  _make_handler_param(server, conn);
	osl_thread_pool_call(server->pool, (osl_thread_pool_func)_on_connect_wrapper, param);
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

    osl_selector_register(server->selector, server->sock, OSL_FLAG_READ);

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

chttpserver_server_t * chttpserver_init(chttpserver_server_t * server, const char * host, int port, int pool_size)
{
    server->sock = OSL_INVALID_SOCKET;
    server->host = osl_safe_strdup(host);
    server->port = port;
    server->on_connect = _on_connect;
    server->on_close = _on_close;
    server->pool = osl_thread_pool_init(osl_thread_pool_new(), pool_size);
    server->selector = osl_selector_init(osl_selector_new());
    server->router = chttpserver_router_init(chttpserver_router_new());
    
    return server;
}

osl_bool chttpserver_start(chttpserver_server_t * server) {

    if (_start_server(server) == osl_false) {
	return osl_false;
    }

    osl_thread_pool_start(server->pool);

    _run(server);
    
    return osl_true;
}

osl_bool chttpserver_start_async(chttpserver_server_t * server) {

    if (_start_server(server) == osl_false) {
	return osl_false;
    }

    osl_thread_pool_start(server->pool);

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
    osl_thread_pool_stop(server->pool);
    osl_selector_free(server->selector);
}


void chttpserver_free(chttpserver_server_t * server)
{
    if (server == NULL) {
	return;
    }
    osl_thread_pool_free(server->pool);
    chttpserver_router_free(server->router);
    osl_safe_free(server->host);
    osl_safe_free(server);
}
