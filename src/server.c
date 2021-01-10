#include "server.h"
#include <osl/thread.h>


static osl_bool _bind(chttpserver_server_t * server, int port)
{
    int backlog = 5;
    /* TODO: bind to correct iface and port */
    osl_inet_address_t * addr = osl_inet_address_new(osl_inet4, "0.0.0.0", port);
    osl_socket sock = osl_socket_bind(addr, osl_true);
    osl_inet_address_free(addr);

    addr = osl_socket_get_inet_address(sock);
    server->port = addr->port;
    osl_inet_address_free(addr);

    if (osl_socket_listen(sock, backlog) != 0)
    {
	perror("listen() failed");
	return osl_false;
    }

    printf("Start HTTP Server (bind port: %d)\n", server->port);

    return osl_true;
}

static void _poll(chttpserver_server_t * server)
{
    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len = sizeof(remote_addr);
    memset(&remote_addr, 0, sizeof(remote_addr));
    osl_socket client = osl_socket_accept(server->sock, (struct sockaddr*)&remote_addr, &remote_addr_len);
    if (server->on_connect) {
	server->on_connect(client);
    } else {
	/* TODO: send dummy message before close */
	osl_socket_close(client);
    }
}

static void _run(chttpserver_server_t * server)
{
    while (!server->done) {
	_poll(server);
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

chttpserver_server_t * chttpserver_start(int port) {
    chttpserver_server_t * server = (chttpserver_server_t*)malloc(sizeof(chttpserver_server_t));
    if (server == NULL) {
	/* TODO: exception */
	return NULL;
    }
    memset(server, 0, sizeof(chttpserver_server_t));
    server->port = port;

    if (_bind(server, port)) {
	osl_safe_free(server);
	return NULL;
    }

    _run(server);
    
    return server;
}

chttpserver_server_t * chttpserver_start_async(int port) {
    chttpserver_server_t * server = (chttpserver_server_t*)malloc(sizeof(chttpserver_server_t));
    if (server == NULL) {
	/* TODO: exception */
	return NULL;
    }
    memset(server, 0, sizeof(chttpserver_server_t));

    if (!_bind(server, port)) {
	osl_safe_free(server);
	return NULL;
    }

    server->thread = osl_thread_new(_runner, (void*)server);
    osl_thread_start(server->thread);
    
    return server;
}

void chttpserver_stop(chttpserver_server_t * server)
{
    server->done = osl_true;
    if (server->thread) {
	osl_thread_join(server->thread);
	osl_thread_free(server->thread);
    }
    osl_safe_free(server);
}
