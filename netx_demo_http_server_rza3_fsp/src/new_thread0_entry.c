#include "new_thread0.h"

#ifdef FEATURE_NX_IPV6
#include "../../addons/http/nxd_http_server.h"
#else
#include "../../addons/http/nx_http_server.h"
#endif

#include "../../addons/dhcp/nxd_dhcp_client.h"

/* Function declarations*/
static void nx_common_init0(void);
static void packet_pool_init0(void);
static void ip_init0(void);
static void print_ip(ULONG ip);

#define LINK_ENABLE_WAIT_TIME (1000U)


/* IP instance */
NX_IP g_ip0;

/* Stack memory for g_ip0. */
//uint8_t g_ip0_stack_memory[G_IP0_TASK_STACK_SIZE] BSP_PLACE_IN_SECTION(".stack.g_ip0") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
uint8_t g_ip0_stack_memory[G_IP0_TASK_STACK_SIZE];

/* ARP cache memory for g_ip0. */
uint8_t g_ip0_arp_cache_memory[G_IP0_ARP_CACHE_SIZE] BSP_ALIGN_VARIABLE(4);

/* Packet pool instance (If this is a Trustzone part, the memory must be placed in Non-secure memory). */
NX_PACKET_POOL g_packet_pool0;
uint8_t g_packet_pool0_pool_memory[G_PACKET_POOL0_PACKET_NUM * (G_PACKET_POOL0_PACKET_SIZE + sizeof(NX_PACKET))] BSP_ALIGN_VARIABLE(4);

void *packet_address_start = g_packet_pool0_pool_memory;
void *packet_address_end = &g_packet_pool0_pool_memory[sizeof(g_packet_pool0_pool_memory) - 1];

/* HTTP contents */
char test_contents[] =
"<HTML>\r\n"
"<HEAD><TITLE>NetX HTTP Test</TITLE></HEAD>\r\n"
"<BODY>\r\n"
"<H1>Another NetX Test Page!</H1>\r\n"
"</BODY>\r\n"
"</HTML>\r\n";

/* Set up the HTTP server global variables */
NX_HTTP_SERVER  my_server;
char demo_http_server_stack[4096];

UINT user_request_notify(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr);

NX_PACKET Sample_packet;
NX_PACKET *Sample_packet_ptr;
/* Startup Thread entry function */
void new_thread0_entry(void)
{
    UINT               status  = NX_SUCCESS;

    /* Initialize the NetX system.*/
    nx_common_init0();

    /* Initialize the packet pool.*/
    packet_pool_init0();

    /* create the ip instance.*/
    ip_init0();

    /* Create the HTTP Server.  */
    status = nx_http_server_create(&my_server, "My HTTP Server", &g_ip0, NX_NULL,
                          demo_http_server_stack, sizeof(demo_http_server_stack), &g_packet_pool0, NX_NULL, user_request_notify);
    if(NX_SUCCESS != status)
    {
        printf("SERVER_CREATE_FAILED ERRNUM=%x\n\r", status);
    }
    printf("SERVER_CREATE_FINISHED\n\r");

    /* OK to start the HTTP Server.   */
    status = nx_http_server_start(&my_server);
    if(NX_SUCCESS != status)
    {
        printf("SERVER_START_FAILED ERRNUM=%x\n\r", status);
    }
    printf("SERVER_START_FINISHED\n\r");
    /*Gets IP address*/
    ULONG client_ip_address = 0;
    ULONG network_mask = 0;
    status = nx_ip_address_get (&g_ip0, &client_ip_address, &network_mask);
    if(NX_SUCCESS != status)
    {
        printf("IP_ADDRESS_GET_FAILED\n\r");
    }
    printf("IP_ADDRESS: ");
    print_ip(client_ip_address);
    printf("PACKET_ADDRESS=%lx\n\r", packet_address_start);
    printf("PACKET_ADDRESS_END=%lx\n\r", packet_address_end);
}

/*******************************************************************************************************************//**
 * @brief     Initialization of NetX system.
 * @param[IN] None
 * @retval    None
 **********************************************************************************************************************/
static void nx_common_init0(void)
{
    /* Initialize the NetX system. */
    nx_system_initialize ();
}

/*******************************************************************************************************************//**
 * @brief     Create the packet pool.
 * @param[IN] None
 * @retval    None
 **********************************************************************************************************************/
static void packet_pool_init0(void)
{
    /* Create the packet pool. */
    UINT status = nx_packet_pool_create(&g_packet_pool0,
                                        "g_packet_pool0 Packet Pool",
                                        G_PACKET_POOL0_PACKET_SIZE,
                                        &g_packet_pool0_pool_memory[0],
                                        sizeof(g_packet_pool0_pool_memory));
    if(NX_SUCCESS != status)
    {
		;
    }
}
TX_EVENT_FLAGS_GROUP my_event_flags_group;
/*******************************************************************************************************************//**
 * @brief     Create the ip instance and enables ARP,UDP,ICMP.
 * @param[IN] None
 * @retval    None
 **********************************************************************************************************************/
static void ip_init0(void)
{
    UINT status = NX_SUCCESS;

    /* Create the ip instance. */
    status = nx_ip_create(&g_ip0,
                          "g_ip0 IP Instance",
                          G_IP0_ADDRESS,
                          G_IP0_SUBNET_MASK,
                          &g_packet_pool0,
                          g_netxduo_ether_1,
                          &g_ip0_stack_memory[0],
                          G_IP0_TASK_STACK_SIZE,
                          G_IP0_TASK_PRIORITY);
    if(NX_SUCCESS != status)
    {
		printf("IP_CREATE_FAILED ERRNUM=%x\n\r", status);
    }
    printf("IP_CREATE_FINISHED\n\r");

    /* Set the gateway address if it is configured. */
    if (IP_ADDRESS(0, 0, 0, 0) != G_IP0_GATEWAY_ADDRESS)
    {
        status = nx_ip_gateway_address_set (&g_ip0, G_IP0_GATEWAY_ADDRESS);
        if(NX_SUCCESS != status)
        {
            printf("GATEWAY_SET_FAILED ERRNUM=%x\n\r", status);
        }
        printf("GATEWAY_SET_FINISHED\n\r");
    }

    /* Enable IP fragment. */
    status = nx_ip_fragment_enable(&g_ip0);
    if(NX_SUCCESS != status)
    {
        printf("IP_FRAGMENT_ENABLE_FAILED ERRNUM=%x\n\r", status);
    }
    printf("IP_FRAGMENT_ENABLE_FINISHED\n\r");

    /* Enables Address Resolution Protocol (ARP).*/
    status = nx_arp_enable(&g_ip0, &g_ip0_arp_cache_memory[0], G_IP0_ARP_CACHE_SIZE);
    if(NX_SUCCESS != status)
    {
        printf("ARP_ENABLE_FAILED ERRNUM=%x\n\r", status);
    }
    printf("ARP_ENABLE_FINISHED\n\r");

    /* Enable udp.*/
    status = nx_udp_enable(&g_ip0);
    if(NX_SUCCESS != status)
    {
        printf("UDP_ENABLE_FAILED ERRNUM=%x\n\r", status);
    }
    printf("UDP_ENABLE_FINISHED\n\r");

    /* Enable tcp.*/
    status = nx_tcp_enable(&g_ip0);
    if(NX_SUCCESS != status)
    {
        printf("TCP_ENABLE_FAILED ERRNUM=%x\n\r", status);
    }
    printf("TCP_ENABLE_FINISHED\n\r");

    /* Enable icmp.*/
    status = nx_icmp_enable(&g_ip0);
    if(NX_SUCCESS != status)
    {
        printf("ICMP_ENABLE_FAILED ERRNUM=%x\n\r", status);
    }
    printf("ICMP_ENABLE_FINISHED\n\r");

    /* Wait for the link to be enabled. */
    ULONG current_state;
    status = nx_ip_status_check(&g_ip0, NX_IP_LINK_ENABLED, &current_state, LINK_ENABLE_WAIT_TIME);
    if(NX_SUCCESS != status)
    {
        printf("STATUS_CHECK_FAILED ERRNUM=%x\n\r", status);
    }
    printf("STATUS_CHECK_FINISHED\n\r");

}

UINT user_request_notify(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr)
{
    char buff[128];
    int len;

    /* Look for the test resource! */
    if(strcmp(resource, "/") == 0)
    {
        if(request_type == NX_HTTP_SERVER_GET_REQUEST)
        {
            len = strlen(test_contents);
            sprintf(buff, "HTTP/1.0 200 \r\nContent-Length: %d\r\nContent-Type: text/html\r\n\r\n", len);

            nx_http_server_callback_data_send(server_ptr, buff, strlen(buff));
            nx_http_server_callback_data_send(server_ptr, test_contents, len);

            return(NX_HTTP_CALLBACK_COMPLETED);
        }
    }

    /* Search for content by FileX. */
    return(NX_SUCCESS);
}
static void print_ip(ULONG ip)
{
    ULONG bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;
    printf("%d.%d.%d.%d\r\n", bytes[3], bytes[2], bytes[1], bytes[0]);
}
