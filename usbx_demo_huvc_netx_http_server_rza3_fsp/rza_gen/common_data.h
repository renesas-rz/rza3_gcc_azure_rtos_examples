/* generated common header file - do not edit */
#ifndef COMMON_DATA_H_
#define COMMON_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "tx_api.h"
#include "r_usb_basic.h"
#include "r_usb_basic_api.h"
#include "r_sdhi.h"
#include "r_sdmmc_api.h"
#include "rm_block_media_sdmmc.h"
#include "rm_block_media_api.h"
#include "rm_filex_block_media.h"
#include "fx_api.h"
#include "nx_api.h"
#include "r_gether_phy.h"
#include "r_ether_phy_api.h"
#include "r_gether.h"
#include "r_ether_api.h"
#include "rm_netxduo_ether.h"
#include "r_mmu.h"
#include "r_ioport.h"
#include "bsp_pin_cfg.h"
FSP_HEADER
/* Basic on USB Instance. */
extern const usb_instance_t g_basic0;

/** Access the USB instance using these structures when calling API functions directly (::p_api is not used). */
extern usb_instance_ctrl_t g_basic0_ctrl;
extern const usb_cfg_t g_basic0_cfg;

#ifndef NULL
void NULL(void*);
#endif

#if 2 == BSP_CFG_RTOS
#ifndef NULL
void NULL(usb_event_info_t *, usb_hdl_t, usb_onoff_t);
#endif
#endif
/** SDMMC on SDMMC Instance. */
extern const sdmmc_instance_t g_sdmmc0;

/** Access the SDMMC instance using these structures when calling API functions directly (::p_api is not used). */
extern sdhi_instance_ctrl_t g_sdmmc0_ctrl;
extern sdmmc_cfg_t g_sdmmc0_cfg;

#ifndef rm_block_media_sdmmc_callback
void rm_block_media_sdmmc_callback(sdmmc_callback_args_t *p_args);
#endif
extern const rm_block_media_instance_t g_rm_block_media0;

/** Access the SDMMC instance using these structures when calling API functions directly (::p_api is not used). */
extern rm_block_media_sdmmc_instance_ctrl_t g_rm_block_media0_ctrl;
extern const rm_block_media_cfg_t g_rm_block_media0_cfg;

#ifndef rm_filex_block_media_memory_callback
void rm_filex_block_media_memory_callback(rm_block_media_callback_args_t *p_args);
#endif
extern const rm_filex_block_media_instance_t g_rm_filex_block_media_0_instance;

/** Access the FileX Block Media instance using these structures when calling API functions directly (::p_api is not used). */
extern rm_filex_block_media_instance_ctrl_t g_rm_filex_block_media_0_ctrl;
extern const rm_filex_block_media_cfg_t g_rm_filex_block_media_0_cfg;

#ifndef g_rm_filex_block_media_0_callback
void g_rm_filex_block_media_0_callback(rm_filex_block_media_callback_args_t *p_args);
#endif
#define G_FX_MEDIA0_MEDIA_MEMORY_SIZE (512)
#define G_FX_MEDIA0_VOLUME_NAME ("Volume 1")
#define G_FX_MEDIA0_NUMBER_OF_FATS (1)
#define G_FX_MEDIA0_DIRECTORY_ENTRIES (256)
#define G_FX_MEDIA0_HIDDEN_SECTORS (0)
#define G_FX_MEDIA0_TOTAL_SECTORS (3751936)
#define G_FX_MEDIA0_BYTES_PER_SECTOR (512)
#define G_FX_MEDIA0_SECTORS_PER_CLUSTER (1)
#define G_FX_MEDIA0_VOLUME_SERIAL_NUMBER (12345)
#define G_FX_MEDIA0_BOUNDARY_UNIT (128)
#ifndef ETHER_PHY_LSI_TYPE_KIT_COMPONENT
#define ETHER_PHY_LSI_TYPE_KIT_COMPONENT ETHER_PHY_LSI_TYPE_DEFAULT
#endif

/** ether_phy on ether_phy Instance. */
extern const ether_phy_instance_t g_gether_phy1;

/** Access the Ethernet PHY instance using these structures when calling API functions directly (::p_api is not used). */
extern ether_phy_instance_ctrl_t g_gether_phy1_ctrl;
extern const ether_phy_cfg_t g_gether_phy1_cfg;
#define ETHER_BUFFER_PLACE_IN_SECTION __attribute__((section("UNCACHED_BSS")))

/** ether on ether Instance. */
extern const ether_instance_t g_gether1;

/** Access the Ethernet instance using these structures when calling API functions directly (::p_api is not used). */
extern ether_instance_ctrl_t g_gether1_ctrl;
extern const ether_cfg_t g_gether1_cfg;

#ifndef rm_netxduo_ether_callback
void rm_netxduo_ether_callback(ether_callback_args_t *p_args);
#endif
/* NetX Duo Ethernet Driver */
void g_netxduo_ether_1(NX_IP_DRIVER *driver_req_ptr);

/* Instance for storing state information for the Ethernet Driver. */
extern rm_netxduo_ether_instance_t g_netxduo_ether_1_instance;
#define G_PACKET_POOL0_PACKET_SIZE (1568)
#define G_PACKET_POOL0_PACKET_NUM  (16)
extern NX_PACKET_POOL g_packet_pool0;
extern uint8_t g_packet_pool0_pool_memory[(G_PACKET_POOL0_PACKET_NUM * (G_PACKET_POOL0_PACKET_SIZE + sizeof(NX_PACKET)))];

/* Quick Setup for g_packet_pool0 instance. */
void g_packet_pool0_quick_setup();
#define G_IP0_ADDRESS      (IP_ADDRESS(192,168,2,182))
#define G_IP0_SUBNET_MASK     (IP_ADDRESS(255,255,255,0))
#define G_IP0_GATEWAY_ADDRESS (IP_ADDRESS(0,0,0,0))
#define G_IP0_TASK_STACK_SIZE (2048)
#define G_IP0_TASK_PRIORITY   (3)
#define G_IP0_ARP_CACHE_SIZE (520 * 1)

#define RA_NOT_DEFINED 0xFFFFFFFF
#if (RA_NOT_DEFINED != g_netxduo_ether_1)
#define G_IP0_NETWORK_DRIVER g_netxduo_ether_1

#else
#define G_IP0_NETWORK_DRIVER nx_driver_ewf_adapter
#endif
#undef RA_NOT_DEFINED

void g_ip0_quick_setup();
void g_ip0_error_handler(UINT status);

#ifndef NX_DISABLE_IPV6
extern NXD_ADDRESS g_ip0_ipv6_global_address;
extern NXD_ADDRESS g_ip0_ipv6_link_local_address;
#endif
/* MMU Instance */
extern const mmu_instance_t g_mmu;

/* MMU control structure. */
extern mmu_instance_ctrl_t g_mmu_ctrl;
extern const mmu_cfg_t g_mmu_cfg;
/* IOPORT Instance */
extern const ioport_instance_t g_ioport;

/* IOPORT control structure. */
extern ioport_instance_ctrl_t g_ioport_ctrl;
extern TX_EVENT_FLAGS_GROUP g_usb_plug_events;
void g_common_init(void);
FSP_FOOTER
#endif /* COMMON_DATA_H_ */
