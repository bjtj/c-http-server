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
