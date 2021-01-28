#include <osl/str.h>
#include <assert.h>
#include "request.h"



int main()
{
    osl_socket sock = 0;
    chttpserver_connection_t * conn = chttpserver_connection_init(chttpserver_connection_new(), sock, OSL_FLAG_READ);
    chttpserver_request_t * request = chttpserver_request_init(chttpserver_request_new(), conn, CHTTPSERVER_HTTP_1_1);
    assert(request->header != NULL);
    assert(request->header->firstline != NULL);
    assert(chttpserver_request_get_protocol_version(request) == CHTTPSERVER_HTTP_1_1);

    chttpserver_request_set_transfer_type(request, CHTTPSERVER_TRANSFER_CHUNKED);
    assert(osl_string_equals(chttpserver_request_get_transfer_value(request), "chunked"));

    {
	osl_list_t * types = NULL;
	chttpserver_transfer_type_e chunked = CHTTPSERVER_TRANSFER_CHUNKED;
	chttpserver_transfer_type_e gzip = CHTTPSERVER_TRANSFER_GZIP;
	
	types = osl_list_append(types, (void*)&chunked);
	types = osl_list_append(types, (void*)&gzip);

	chttpserver_request_set_transfer_types(request, types);
	assert(osl_string_equals(chttpserver_request_get_transfer_value(request), "chunked, gzip"));
	
	osl_list_free(types, NULL);
    }
    
    chttpserver_request_free(request);
    chttpserver_connection_free(conn);
    
    return 0;
}
