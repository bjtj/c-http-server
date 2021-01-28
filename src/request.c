#include <osl/str.h>
#include <osl/string_buffer.h>
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

void chttpserver_request_set_method(chttpserver_request_t * req, const char * method)
{
    chttpserver_header_firstline_set_part1(req->header->firstline, method);
}

const char * chttpserver_request_get_uri(chttpserver_request_t * req)
{
    return req->header->firstline->part2;
}

void chttpserver_request_set_uri(chttpserver_request_t * req, const char * uri)
{
    chttpserver_header_firstline_set_part2(req->header->firstline, uri);
}

const char * chttpserver_request_get_protocol(chttpserver_request_t * req)
{
    return req->header->firstline->part3;
}

void chttpserver_request_set_protocol(chttpserver_request_t * req, const char * protocol)
{
    chttpserver_header_firstline_set_part3(req->header->firstline, protocol);
}

chttpserver_protocol_version_e chttpserver_request_get_protocol_version(chttpserver_request_t * req)
{
    return chttpserver_protocol_version_from_str(chttpserver_request_get_protocol(req));
}

const char * chttpserver_request_get_transfer_value(chttpserver_request_t * req)
{
    return chttpserver_header_get_field_value(req->header, "Transfer-Encoding");
}

void chttpserver_request_set_transfer_value(chttpserver_request_t * req, const char * transfer)
{
    chttpserver_header_set_field_value(req->header, "Transfer-Encoding", transfer);
}

void chttpserver_request_set_transfer_type(chttpserver_request_t * req, chttpserver_transfer_type_e type)
{
    chttpserver_request_set_transfer_value(req, chttpserver_transfer_type_to_str(type));
}

static void _iter_join_transfer_types(int idx, void * data, void * user)
{
    osl_string_buffer_t * sb = (osl_string_buffer_t *)user;
    chttpserver_transfer_type_e * type = (chttpserver_transfer_type_e *)data;
    const char * type_str = chttpserver_transfer_type_to_str(*type);
    if (idx > 0) {
	osl_string_buffer_append(sb, ", ");
    }
    osl_string_buffer_append(sb, type_str);
}

void chttpserver_request_set_transfer_types(chttpserver_request_t * req, osl_list_t * types)
{
    char * value;
    osl_string_buffer_t * sb = osl_string_buffer_init(osl_string_buffer_new());
    osl_list_each(types, _iter_join_transfer_types, sb);
    value = osl_string_buffer_to_str_and_free(sb);
    chttpserver_request_set_transfer_value(req, value);
    osl_safe_free(value);
}

const char * chttpserver_request_get_connection_value(chttpserver_request_t * req)
{
    return chttpserver_header_get_field_value(req->header, "Connection");
}

void chttpserver_request_set_connection_value(chttpserver_request_t * req, const char * connection)
{
    if (connection == NULL) {
	chttpserver_header_remove_field(req->header, "Connection");
    } else {
	chttpserver_header_set_field_value(req->header, "Connection", connection);
    }
}

osl_bool chttpserver_request_is_chunked_transfer(chttpserver_request_t * req)
{
    const char * transfer = chttpserver_request_get_transfer_value(req);
    if (transfer == NULL) {
	return osl_false;
    }
    return osl_string_equals(transfer, "chunked");
}

osl_bool chttpserver_request_is_keep_alive(chttpserver_request_t * req)
{
    const char * connection = chttpserver_request_get_connection_value(req);
    if (connection == NULL) {
	return osl_false;
    }
    return osl_string_equals(connection, "keep-alive");
}

void chttpserver_request_set_keep_alive(chttpserver_request_t * req, osl_bool set)
{
    chttpserver_request_set_connection_value(req, set ? "keep-alive" : NULL);
}
