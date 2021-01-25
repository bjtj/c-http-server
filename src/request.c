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

chttpserver_request_t * chttpserver_request_init(chttpserver_request_t * req, chttpserver_connection_t * connection, chttpserver_protocol_version_e protocol)
{
    req->header = chttpserver_header_init(chttpserver_header_new(), CHTTPSERVER_REQUEST_HEADER, protocol);;
    req->connection = connection;
    return req;
}

chttpserver_request_t * chttpserver_request_init_with_header(chttpserver_request_t * req, chttpserver_connection_t * connection, chttpserver_header_t * header)
{
    req->header = header;
    req->connection = connection;
    return req;
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


chttpserver_protocol_version_e chttpserver_request_get_protocol_version(chttpserver_request_t * req)
{
    return chttpserver_protocol_version_from_str(chttpserver_request_get_protocol(req));
}
