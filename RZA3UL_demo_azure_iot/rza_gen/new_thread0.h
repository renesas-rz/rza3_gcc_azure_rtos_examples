/* generated thread header file - do not edit */
#ifndef NEW_THREAD0_H_
#define NEW_THREAD0_H_
#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void new_thread0_entry(void);
                #else
extern void new_thread0_entry(void);
#endif
#include "nxd_dhcp_client.h"
#include "nxd_sntp_client.h"
FSP_HEADER
void g_dhcp_client0_quick_setup();
#define G_SNTP_CLIENT0_INDEX_NETWORK_INTERFACE  0

void g_sntp_client0_quick_setup();
FSP_FOOTER
#endif /* NEW_THREAD0_H_ */
