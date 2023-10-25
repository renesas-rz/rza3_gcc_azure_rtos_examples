#include "new_thread0.h"

#ifdef FEATURE_NX_IPV6
//#include "../../addons/http/nxd_http_server.h"
#else
#include "../../addons/http/nx_http_server.h"
#endif

#include "../../addons/dhcp/nxd_dhcp_client.h"

/* Function declarations*/
//static void nx_common_init0(void);
//static void packet_pool_init0(void);
//static void ip_init0(void);
static void print_ip(ULONG ip);

#define LINK_ENABLE_WAIT_TIME (1000U)



/* HTTP contents */
char test_contents[] =
"<HTML>\r\n"
"<HEAD><TITLE>NetX HTTP Test</TITLE></HEAD>\r\n"
"<BODY>\r\n"
"<H1>Another NetX Test Page!</H1>\r\n"
"</BODY>\r\n"
"</HTML>\r\n";

/* Set up the HTTP server global variables */
//NX_HTTP_SERVER  my_server;
char demo_http_server_stack[4096];

//UINT user_request_notify(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr);

NX_PACKET Sample_packet;
NX_PACKET *Sample_packet_ptr;
/* Startup Thread entry function */
void new_thread0_entry(void)
{
//    UINT i;
//    for(i=0;i<10;i++){
//        printf("i = %d\r\n",i);
//        tx_thread_sleep(1000);
//    }

    tx_application_define_user2(NULL);
}

/*******************************************************************************************************************//**
 * @brief     Initialization of NetX system.
 * @param[IN] None
 * @retval    None
 **********************************************************************************************************************/
//static void nx_common_init0(void)
//{
//    /* Initialize the NetX system. */
//    nx_system_initialize ();
//}




//UINT user_request_notify(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr)
//{
//    char buff[128];
//    int len;
//
//    /* Look for the test resource! */
//    if(strcmp(resource, "/") == 0)
//    {
//        if(request_type == NX_HTTP_SERVER_GET_REQUEST)
//        {
//            len = strlen(test_contents);
//            sprintf(buff, "HTTP/1.0 200 \r\nContent-Length: %d\r\nContent-Type: text/html\r\n\r\n", len);
//
//            nx_http_server_callback_data_send(server_ptr, buff, strlen(buff));
//            nx_http_server_callback_data_send(server_ptr, test_contents, len);
//
//            return(NX_HTTP_CALLBACK_COMPLETED);
//        }
//    }
//
//    /* Search for content by FileX. */
//    return(NX_SUCCESS);
//}
//static void print_ip(ULONG ip)
//{
//    ULONG bytes[4];
//    bytes[0] = ip & 0xFF;
//    bytes[1] = (ip >> 8) & 0xFF;
//    bytes[2] = (ip >> 16) & 0xFF;
//    bytes[3] = (ip >> 24) & 0xFF;
//    printf("%d.%d.%d.%d\r\n", bytes[3], bytes[2], bytes[1], bytes[0]);
//}
