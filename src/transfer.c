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

chttpserver_transfer_t * chttpserver_transfer_init(chttpserver_transfer_t * transfer)
{
    return transfer;
}

void chttpserver_transfer_free(chttpserver_transfer_t * transfer)
{
    osl_safe_free(transfer);
}
