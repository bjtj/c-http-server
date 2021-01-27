#include "transfer.h"

static chttpserver_transfer_result_e _on_read_default(chttpserver_transfer_t * transfer, void * data, int size)
{
    int ret = chttpserver_connection_recv(transfer->connection, data, size);
    if (ret < 0) {
	return CHTTPSERVER_TRANSFER_RESULT_FAIL;
    }
    if (ret == size) {
	return CHTTPSERVER_TRANSFER_RESULT_OK;
    }
    return CHTTPSERVER_TRANSFER_RESULT_INCOMPLETE;
}

static chttpserver_transfer_result_e _on_write_default(chttpserver_transfer_t * transfer, void * data, int size)
{
    int ret = chttpserver_connection_send(transfer->connection, data, size);
    if (ret < 0) {
	return CHTTPSERVER_TRANSFER_RESULT_FAIL;
    }
    if (ret == size) {
	return CHTTPSERVER_TRANSFER_RESULT_OK;
    }
    return CHTTPSERVER_TRANSFER_RESULT_INCOMPLETE;
}

static chttpserver_transfer_result_e _on_read_chunked(chttpserver_transfer_t * transfer, void * data, int size)
{
    (void)transfer;
    (void)data;
    (void)size;
    /* TODO:  */
    return CHTTPSERVER_TRANSFER_RESULT_FAIL;
}

static chttpserver_transfer_result_e _on_write_chunked(chttpserver_transfer_t * transfer, void * data, int size)
{
    (void)transfer;
    (void)data;
    (void)size;
    /* TODO:  */
    return CHTTPSERVER_TRANSFER_RESULT_FAIL;
}

const char * chttpserver_transfer_type_to_str(chttpserver_transfer_type_e type)
{
    switch (type) {
    case CHTTPSERVER_TRANSFER_DEFAULT: {
	return "";
    }
    case CHTTPSERVER_TRANSFER_CHUNKED: {
	return "chunked";
    }
    case CHTTPSERVER_TRANSFER_COMPRESS: {
	return "compress";
    }
    case CHTTPSERVER_TRANSFER_DEFLATE: {
	return "deflate";
    }
    case CHTTPSERVER_TRANSFER_GZIP: {
	return "gzip";
    }
    case CHTTPSERVER_TRANSFER_IDENTITY: {
	return "identity";
    }
    default:
	return "(Unknown)";
    }
}

chttpserver_transfer_t * chttpserver_transfer_new(void)
{
    chttpserver_transfer_t * transfer = (chttpserver_transfer_t *)malloc(sizeof(chttpserver_transfer_t));
    if (transfer == NULL) {
	/* TODO: exception */
	return NULL;
    }
    memset(transfer, 0, sizeof(chttpserver_transfer_t));
    return transfer;
}

chttpserver_transfer_t * chttpserver_transfer_init(chttpserver_transfer_t * transfer, chttpserver_connection_t * connection, chttpserver_transfer_type_e type)
{
    transfer->type = type;
    transfer->connection = connection;
    switch (type) {
    case CHTTPSERVER_TRANSFER_DEFAULT: {
	transfer->on_read = _on_read_default;
	transfer->on_write = _on_write_default;
	break;
    }
    case CHTTPSERVER_TRANSFER_CHUNKED: {
	transfer->on_read = _on_read_chunked;
	transfer->on_write = _on_write_chunked;
	break;
    }
	/* TODO:  */
    default:
	break;
    }
    return transfer;
}

void chttpserver_transfer_free(chttpserver_transfer_t * transfer)
{
    osl_safe_free(transfer);
}

chttpserver_transfer_result_e chttpserver_transfer_on_read(chttpserver_transfer_t * transfer, void * data, int size)
{
    return transfer->on_read(transfer, data, size);
}

chttpserver_transfer_result_e chttpserver_transfer_on_write(chttpserver_transfer_t * transfer, void * data, int size)
{
    return transfer->on_write(transfer, data, size);
}
