#include "response.h"


chttpserver_response_t * chttpserver_response_new()
{
    chttpserver_response_t * req = (chttpserver_response_t*)malloc(sizeof(chttpserver_response_t));
    if (req == NULL) {
	/* TODO: exception */
	return NULL;
    }
    memset(req, 0, sizeof(chttpserver_response_t));
    return req;
}

void chttpserver_response_init(chttpserver_response_t * req, chttpserver_header_t * header, osl_socket remote_sock)
{
    req->header = header;
    req->remote_sock = remote_sock;
}

void chttpserver_response_free(chttpserver_response_t * req)
{
    chttpserver_header_free(req->header);
    osl_safe_free(req);
}

const char * chttpserver_response_get_method(chttpserver_response_t * req)
{
    return req->header->firstline->part1;
}

const char * chttpserver_response_get_uri(chttpserver_response_t * req)
{
    return req->header->firstline->part2;
}

const char * chttpserver_response_get_protocol(chttpserver_response_t * req)
{
    return req->header->firstline->part3;
}
