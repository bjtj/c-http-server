#include "connection.h"

chttpserver_connection_t * chttpserver_connection_new(void)
{
    OSL_NEW(chttpserver_connection_t, conn);
    return conn;
}

chttpserver_connection_t * chttpserver_connection_init(chttpserver_connection_t * conn, osl_socket remote_sock, int flags)
{
    conn->remote_sock = remote_sock;
    conn->selector = osl_selector_init(osl_selector_new());
    osl_selector_register(conn->selector, remote_sock, flags);
    return conn;
}

void chttpserver_connection_free(chttpserver_connection_t * conn)
{
    osl_selector_free(conn->selector);
    osl_safe_free(conn);
}

void chttpserver_connection_close(chttpserver_connection_t * conn)
{
    osl_socket_close(conn->remote_sock);
}

int chttpserver_connection_select(chttpserver_connection_t * conn, int timeout)
{
    return osl_selector_select(conn->selector, timeout);
}

osl_bool chttpserver_connection_is_writable(chttpserver_connection_t * conn)
{
    return osl_selector_is_writable(conn->selector, conn->remote_sock);
}

osl_bool chttpserver_connection_is_readable(chttpserver_connection_t * conn)
{
    return osl_selector_is_readable(conn->selector, conn->remote_sock);
}

int chttpserver_connection_send(chttpserver_connection_t * conn, const void * data, int size)
{
    return osl_socket_send(conn->remote_sock, data, size, 0);
}

int chttpserver_connection_recv(chttpserver_connection_t * conn, void * data, int size)
{
    return osl_socket_recv(conn->remote_sock, data, size, 0);
}
