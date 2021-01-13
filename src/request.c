#include "request.h"


chttpserver_request_t * chttpserver_request_new()
{
    chttpserver_request_t * req = (chttpserver_request_t*)malloc(sizeof(chttpserver_request_t));
    if (req == NULL) {
	/* TODO: exception */
	return NULL;
    }
    memset(req, 0, sizeof(chttpserver_request_t));
    return req;
}

void chttpserver_request_init(chttpserver_request_t * req, chttpserver_header_t * header, osl_socket remote_sock)
{
    req->header = header;
    req->remote_sock = remote_sock;
}

void chttpserver_request_free(chttpserver_request_t * req)
{
    chttpserver_header_free(req->header);
    osl_safe_free(req);
}

const char * chttpserver_request_get_method(chttpserver_request_t * req)
{
    return req->header->firstline->part1;
}

const char * chttpserver_request_get_uri(chttpserver_request_t * req)
{
    return req->header->firstline->part2;
}

const char * chttpserver_request_get_protocol(chttpserver_request_t * req)
{
    return req->header->firstline->part3;
}
