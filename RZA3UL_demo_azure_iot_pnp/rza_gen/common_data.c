/* generated common source file - do not edit */
#include "common_data.h"

ether_phy_instance_ctrl_t g_gether_phy1_ctrl;

const ether_phy_instance_extend_t g_gether_phy1_extend_init =
{ .voltage = ETHER_PHY_VOLTAGE_18, };

const ether_phy_cfg_t g_gether_phy1_cfg =
{

.channel = 1,
  .phy_lsi_address = 7, .phy_reset_wait_time = 0x00020000, .mii_bit_access_wait_time = 8, .phy_lsi_type =
          ETHER_PHY_LSI_TYPE_KSZ9131RNXI,
  .flow_control = ETHER_PHY_FLOW_CONTROL_DISABLE, .mii_type = ETHER_PHY_MII_TYPE_RMII, .p_context = NULL, .p_extend =
          &g_gether_phy1_extend_init,

};
/* Instance structure to use this module. */
const ether_phy_instance_t g_gether_phy1 =
{ .p_ctrl = &g_gether_phy1_ctrl, .p_cfg = &g_gether_phy1_cfg, .p_api = &g_ether_phy_on_gether_phy };
ether_instance_ctrl_t g_gether1_ctrl;

uint8_t g_gether1_mac_address[6] =
{ 0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };

__attribute__((__aligned__(128))) gether_instance_tx_descriptor_t g_gether1_tx_descriptors[5 + 1] ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(128))) gether_instance_rx_descriptor_t g_gether1_rx_descriptors[10 + 1] ETHER_BUFFER_PLACE_IN_SECTION;

__attribute__((__aligned__(128)))uint8_t g_gether1_gether_buffer0[1536]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(128)))uint8_t g_gether1_gether_buffer1[1536]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(128)))uint8_t g_gether1_gether_buffer2[1536]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(128)))uint8_t g_gether1_gether_buffer3[1536]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(128)))uint8_t g_gether1_gether_buffer4[1536]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(128)))uint8_t g_gether1_gether_buffer5[1536]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(128)))uint8_t g_gether1_gether_buffer6[1536]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(128)))uint8_t g_gether1_gether_buffer7[1536]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(128)))uint8_t g_gether1_gether_buffer8[1536]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(128)))uint8_t g_gether1_gether_buffer9[1536]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(128)))uint8_t g_gether1_gether_buffer10[1536]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(128)))uint8_t g_gether1_gether_buffer11[1536]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(128)))uint8_t g_gether1_gether_buffer12[1536]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(128)))uint8_t g_gether1_gether_buffer13[1536]ETHER_BUFFER_PLACE_IN_SECTION;
__attribute__((__aligned__(128)))uint8_t g_gether1_gether_buffer14[1536]ETHER_BUFFER_PLACE_IN_SECTION;

uint8_t *pp_g_gether1_gether_buffers[15] =
{ (uint8_t*) &g_gether1_gether_buffer0[0],
  (uint8_t*) &g_gether1_gether_buffer1[0],
  (uint8_t*) &g_gether1_gether_buffer2[0],
  (uint8_t*) &g_gether1_gether_buffer3[0],
  (uint8_t*) &g_gether1_gether_buffer4[0],
  (uint8_t*) &g_gether1_gether_buffer5[0],
  (uint8_t*) &g_gether1_gether_buffer6[0],
  (uint8_t*) &g_gether1_gether_buffer7[0],
  (uint8_t*) &g_gether1_gether_buffer8[0],
  (uint8_t*) &g_gether1_gether_buffer9[0],
  (uint8_t*) &g_gether1_gether_buffer10[0],
  (uint8_t*) &g_gether1_gether_buffer11[0],
  (uint8_t*) &g_gether1_gether_buffer12[0],
  (uint8_t*) &g_gether1_gether_buffer13[0],
  (uint8_t*) &g_gether1_gether_buffer14[0], };

const ether_instance_extend_t g_gether1_extend_init =
{ .p_rx_descriptor = g_gether1_rx_descriptors, .p_tx_descriptor = g_gether1_tx_descriptors, };

const ether_cfg_t g_gether1_cfg =
{ .channel = 1,
  .zerocopy = ETHER_ZEROCOPY_DISABLE,
  .multicast = ETHER_MULTICAST_ENABLE,
  .promiscuous = ETHER_PROMISCUOUS_DISABLE,
  .flow_control = ETHER_FLOW_CONTROL_DISABLE,
  .padding = ETHER_PADDING_DISABLE,
  .padding_offset = 14,
  .broadcast_filter = 0,
  .p_mac_address = g_gether1_mac_address,

  .num_tx_descriptors = 5,
  .num_rx_descriptors = 10,

  .pp_ether_buffers = pp_g_gether1_gether_buffers,

  .ether_buffer_size = 1536,

  .irq = ETH1_PIF_INT_N_IRQn,

  .interrupt_priority = 24,

  .p_callback = rm_netxduo_ether_callback,
  .p_ether_phy_instance = &g_gether_phy1,
  .p_context = &g_netxduo_ether_1_instance,
  .p_extend = &g_gether1_extend_init, };

/* Instance structure to use this module. */
const ether_instance_t g_gether1 =
{ .p_ctrl = &g_gether1_ctrl, .p_cfg = &g_gether1_cfg, .p_api = &g_ether_on_gether };
static NX_PACKET *g_netxduo_ether_1_tx_packets[5];
static NX_PACKET *g_netxduo_ether_1_rx_packets[10];

static rm_netxduo_ether_ctrl_t g_netxduo_ether_1_ctrl;
static rm_netxduo_ether_cfg_t g_netxduo_ether_1_cfg =
{ .p_ether_instance = &g_gether1, .mtu = 1500, .p_tx_packets = g_netxduo_ether_1_tx_packets, .p_rx_packets =
          g_netxduo_ether_1_rx_packets };

rm_netxduo_ether_instance_t g_netxduo_ether_1_instance =
{ .p_ctrl = &g_netxduo_ether_1_ctrl, .p_cfg = &g_netxduo_ether_1_cfg };

/*
 * NetX Duo Driver: g_netxduo_ether_1
 * Passes rm_netxduo_ether instance and driver request into the rm_netxduo_ether driver.
 */
void g_netxduo_ether_1(NX_IP_DRIVER *driver_req_ptr)
{
    rm_netxduo_ether (driver_req_ptr, &g_netxduo_ether_1_instance);
}
#ifndef NX_DISABLE_IPV6
NXD_ADDRESS g_ip0_ipv6_global_address =
{ .nxd_ip_version = NX_IP_VERSION_V6, .nxd_ip_address.v6 =
{ ((0x2001 << 16) | 0x0), ((0x0 << 16) | 0x0), ((0x0 << 16) | 0x0), ((0x0 << 16) | 0x1) } };

NXD_ADDRESS g_ip0_ipv6_link_local_address =
{ .nxd_ip_version = NX_IP_VERSION_V6, .nxd_ip_address.v6 =
{ ((0x0 << 16) | 0x0), ((0x0 << 16) | 0x0), ((0x0 << 16) | 0x0), ((0x0 << 16) | 0x0) } };

#endif
mmu_instance_ctrl_t g_mmu_ctrl;
const mmu_cfg_t g_mmu_cfg;
/* Instance structure to use this module. */
const mmu_instance_t g_mmu =
{ .p_api = &g_mmu_on_mmu, .p_ctrl = &g_mmu_ctrl, .p_cfg = &g_mmu_cfg, };
/** IOPORT interface configuration for event link **/

ioport_event_group_output_t g_port_group_output_cfg[] =
{
{ .pin_select = (uint8_t) (uintptr_t) NULL, .operation = (ioport_event_output_operation_t) NULL }, };

ioport_event_group_input_t g_port_group_input_cfg[] =
{
{ .event_control = (ioport_event_control_t) NULL,
  .edge_detection = (ioport_event_detection_t) NULL,
  .overwrite_control = (ioport_event_control_t) NULL,
  .pin_select = (uintptr_t) NULL,
  .buffer_init_value = (uintptr_t) NULL }, };

ioport_event_single_t g_single_port_cfg[] =
{
{ .event_control = (ioport_event_control_t) NULL, .direction = (ioport_event_direction_t) NULL, .port_num =
          (uintptr_t) NULL,
  .operation = (ioport_event_output_operation_t) NULL, .edge_detection = (ioport_event_detection_t) NULL }, };

const ioport_extend_cfg_t g_ioport_cfg_extend =
{ .p_port_group_output_cfg = &g_port_group_output_cfg[0],
  .p_port_group_input_cfg = &g_port_group_input_cfg[0],
  .p_single_port_cfg = &g_single_port_cfg[0] };

ioport_instance_ctrl_t g_ioport_ctrl;
const ioport_instance_t g_ioport =
{ .p_api = &g_ioport_on_ioport, .p_ctrl = &g_ioport_ctrl, .p_cfg = &g_bsp_pin_cfg, };

void g_common_init(void)
{
}
