#include "transfer.h"


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

chttpserver_transfer_t * chttpserver_transfer_init(chttpserver_transfer_t * transfer, chttpserver_connection_t * connection)
{
    transfer->connection = connection;
    return transfer;
}

void chttpserver_transfer_free(chttpserver_transfer_t * transfer)
{
    osl_safe_free(transfer);
}

chttpserver_transfer_result_e chttpserver_transfer_on_read(chttpserver_transfer_t * transfer, void * data, int size)
{
    chttpserver_connection_recv(transfer->connection, data, size);
    /* TODO:  */
    return CHTTPSERVER_TRANSFER_RESULT_OK;
}

chttpserver_transfer_result_e chttpserver_transfer_on_write(chttpserver_transfer_t * transfer, void * data, int size)
{
    chttpserver_connection_send(transfer->connection, data, size);
    /* TODO:  */
    return CHTTPSERVER_TRANSFER_RESULT_OK;
}
