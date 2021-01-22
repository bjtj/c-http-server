#include <osl/str.h>
#include "response.h"
#include "status_code.h"


chttpserver_response_t * chttpserver_response_new()
{
    chttpserver_response_t * res = (chttpserver_response_t*)malloc(sizeof(chttpserver_response_t));
    if (res == NULL) {
	/* TODO: exception */
	return NULL;
    }
    memset(res, 0, sizeof(chttpserver_response_t));
    return res;
}

chttpserver_response_t * chttpserver_response_init(chttpserver_response_t * res, osl_socket remote_sock, chttpserver_protocol_version_e protocol)
{
    res->header = chttpserver_header_init(chttpserver_header_new(), CHTTPSERVER_RESPONSE_HEADER, protocol);
    res->remote_sock = remote_sock;
    return res;
}

chttpserver_response_t * chttpserver_response_init_with_header(chttpserver_response_t * res, osl_socket remote_sock, chttpserver_header_t * header)
{
    res->header = header;
    res->remote_sock = remote_sock;
    return res;
}

void chttpserver_response_free(chttpserver_response_t * res)
{
    chttpserver_header_free(res->header);
    chttpserver_transfer_free(res->transfer);
    osl_safe_free(res->content);
    osl_safe_free(res);
}

const char * chttpserver_response_get_protocol(chttpserver_response_t * res)
{
    return res->header->firstline->part1;
}

chttpserver_protocol_version_e chttpserver_response_get_protocol_version(chttpserver_response_t * res)
{
    return chttpserver_protocol_version_from_str(chttpserver_response_get_protocol(res));
}

int chttpserver_response_get_status_code(chttpserver_response_t * res)
{
    return atoi(res->header->firstline->part2);
}

const char * chttpserver_response_get_status_message(chttpserver_response_t * res)
{
    return res->header->firstline->part3;
}

osl_bool chttpserver_response_set_protocol(chttpserver_response_t * res, chttpserver_protocol_version_e ver)
{
    if (res == NULL || res->header == NULL || res->header->firstline == NULL) {
	return osl_false;
    }
    chttpserver_header_firstline_set_part1(res->header->firstline, chttpserver_protocol_version_to_str(ver));
    return osl_true;
}

osl_bool chttpserver_response_set_status(chttpserver_response_t * res, int code)
{
    chttpserver_response_set_status_code(res, code);
    chttpserver_response_set_status_message(res, chttpserver_status_get_message(code));
    return osl_true;
}

osl_bool chttpserver_response_set_status_code(chttpserver_response_t * res, int code)
{
    char str[10] = {0,};
    if (res == NULL || res->header == NULL || res->header->firstline == NULL) {
	return osl_false;
    }
    snprintf(str, sizeof(str), "%d", code);
    chttpserver_header_firstline_set_part2(res->header->firstline, str);
    return osl_true;
}

osl_bool chttpserver_response_set_status_message(chttpserver_response_t * res, const char * msg)
{
    if (res == NULL || res->header == NULL || res->header->firstline == NULL) {
	return osl_false;
    }
    chttpserver_header_firstline_set_part3(res->header->firstline, msg);
    return osl_true;
}

osl_bool chttpserver_response_set_content(chttpserver_response_t * res, const char * type, const char * content)
{
    char len[10] = {0,};
    osl_safe_free(res->content);
    res->content = osl_strdup(content);
    chttpserver_header_set_field_value(res->header, "Content-Type", type);
    snprintf(len, sizeof(len), "%ld", strlen(content));
    chttpserver_header_set_field_value(res->header, "Content-Length", len);
    return osl_true;
}
