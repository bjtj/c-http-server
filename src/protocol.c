#include <osl/str.h>
#include "protocol.h"


const char * chttpserver_protocol_version_to_str(chttpserver_protocol_version_e version)
{
    switch (version) {
    case CHTTPSERVER_HTTP_1_0: return "HTTP/1.0";
    case CHTTPSERVER_HTTP_1_1: return "HTTP/1.1";
    default:
	return "(UNKNOWN)";
    }
}

chttpserver_protocol_version_e chttpserver_protocol_version_from_str(const char * protocol)
{
    if (osl_string_equals(protocol, "HTTP/1.0")) {
	return CHTTPSERVER_HTTP_1_0;
    }

    if (osl_string_equals(protocol, "HTTP/1.1")) {
	return CHTTPSERVER_HTTP_1_1;
    }

    return CHTTPSERVER_HTTP_UNKNOWN;
}
