/* HTTP thread and functions */
#include "tx_api.h"
#include "nx_api.h"

//#include "nx_rz_driver.h"

#include "mjpeg_defs.h"

#ifdef FEATURE_NX_IPV6
#include "../../addons/http/nxd_http_server.h"
#else
#include "../../addons/http/nx_http_server.h"
#endif


#define MJPEG_PORT	8090
#define FRAME_PACKET_LENGTH	1456

/* If you want to see only one frame, comment out MJPEG_ENABLE define. */
#define MJPEG_ENABLE

/**********************************************************************************************************************
 Private global variables and functions
 *********************************************************************************************************************/

char header_string[] =
#ifdef MJPEG_ENABLE
{
    "HTTP/1.0 200 OK\r\n"
    "Cache-Control: no-cache\r\n"
    "Pragma: no-cache\r\n"
    "Connection: close\r\n"
    "Content-Type: multipart/x-mixed-replace; boundary=myboundary\r\n\r\n"
};
#else
{
    "HTTP/1.0 200 OK\r\n"
    "Cache-Control: no-cache\r\n"
    "Pragma: no-cache\r\n"
    "Connection: close\r\n"
    "Content-Type: image/jpeg\r\n"
    "Content-Length: %d\r\n\r\n"
};
#endif

char frame_string[] =
{
    "--myboundary\r\n"
    "Content-Type: image/jpeg\r\n"
    "Content-Length: %d\r\n\r\n"
};

char top_http_contents[] =
"<html><head><title>UVC camera test page</title></head>\r\n"
"<body>\r\n"
"<form method=post action=\"/?\">\r\n"
"<table border=0 cellspacing=0 cellpadding=0 width=100%%><tr bgcolor=#ccccff>\r\n"
"   <td width=50%%>\r\n"
"   <input type=hidden name=\"_Spec\" value=spec>\r\n"
"   <table border=0 cellspacing=0 cellpadding=1>\r\n"
"       <tr><td>Select camera parameter<br><br></td></tr>\r\n"
"       <tr><td nowrap>Parameter list</td>\r\n"
"           <td><select name=\"_Index\">\r\n";

char tail_http_contents[] =
"           </select></td>\r\n"
"       </tr>\r\n"
"       <tr><td></td><td><input type=submit name=Submit value=\"Set\"></td></tr>\r\n"
"   </table>\r\n"
"   </td></tr>\r\n"
"</table>\r\n"
"</form>\r\n"
"<img src=\"http://%d.%d.%d.%d:%d/\">\r\n"
"</body></html>\r\n";

/* HTML contents work area */
char buff_send[10*1024];
char tmp_buff_send[1024];

/* Define the NetX object control blocks...  */
NX_PACKET_POOL g_http_packet_pool0;

char demo_pool_area[10*1024];
char demo_ip_stack[4096];
char demo_arp_area[1024];

extern ULONG error_counter;

/* Set up the HTTP server global variables and prototypes */
NX_HTTP_SERVER  g_http_server0;
char demo_http_server_stack[4096];

UINT authentication_check(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, CHAR **name, CHAR **password, CHAR **realm);
UINT request_notify(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr);
void set_request_contents(NX_HTTP_SERVER *server_ptr);
void http_thread_entry(ULONG thread_input);
UINT stream_send(NX_TCP_SOCKET* socket, UCHAR* image, ULONG length);


/* authentication callback function */
UINT authentication_check(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, CHAR **name, CHAR **password,
        CHAR **realm)
{
    return(NX_SUCCESS);
}

/* request callback function */
UINT request_notify(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr)
{
    UINT len;
    char buff[64];
    UINT actual_size;

    char* p_index;
    int select;

    /* Look for the test resource. */
    if(strcmp(resource, "/")==0)
    {
        if(request_type==NX_HTTP_SERVER_GET_REQUEST)
        {
            set_request_contents(server_ptr);

            return(NX_HTTP_CALLBACK_COMPLETED);
        }
        else if(request_type == NX_HTTP_SERVER_POST_REQUEST)
        {
            len = nx_http_server_content_length_get(packet_ptr);
            nx_http_server_content_get(server_ptr, packet_ptr, 0, buff, sizeof(buff), &actual_size);
            buff[actual_size] = '\0';

            /* find selected index */
            if((p_index=strstr(buff, "_Index="))!=NULL)
            {
                select = atoi(&p_index[7]);
                uvc_camera_spec_set(select);
            }

            set_request_contents(server_ptr);

            return(NX_HTTP_CALLBACK_COMPLETED);
        }
    }

    /* find by FileX */
    return(NX_SUCCESS);
}

void set_request_contents(NX_HTTP_SERVER *server_ptr)
{
    ULONG ip_address;
    ULONG network_mask;
    ULONG len;
    UINT actual_size;
    UINT send_length;

    int count;
    SPEC_OF_CAMERA* p_camera_spec;
    int select;
    int i;

    uvc_camera_stop();

    sprintf(buff_send, top_http_contents);

    /* Make spec list */
    count = uvc_camera_spec_get(&p_camera_spec, &select);
    for(i=0; i<count; i++)
    {
        sprintf(tmp_buff_send, "<option value=%d %s>%d,%d  %d %s\r\n",
                i,
                i==select?"selected":"",
                p_camera_spec[i].width,
                p_camera_spec[i].height,
                p_camera_spec[i].interval,
                i==select?" *":"");
        strcat(buff_send, tmp_buff_send);
    }

    /* Make port address of image data */
    nx_ip_address_get(server_ptr->nx_http_server_ip_ptr, &ip_address, &network_mask);
    sprintf(tmp_buff_send, tail_http_contents,
        (ip_address>>24)&0x00ff,
        (ip_address>>16)&0x00ff,
        (ip_address>>8)&0x00ff,
        (ip_address)&0x00ff,
        MJPEG_PORT);

    strcat(buff_send, tmp_buff_send);

    len = (ULONG)strlen(buff_send);
    sprintf(tmp_buff_send, "HTTP/1.0 200 \r\nContent-Length: %ld\r\nContent-Type: text/html\r\n\r\n", len);

    nx_http_server_callback_data_send(server_ptr, tmp_buff_send, (ULONG)strlen(tmp_buff_send));

    send_length = 0;
    while(send_length<len)
    {
        actual_size = len - send_length;
        if(FRAME_PACKET_LENGTH<actual_size)
        {
            actual_size = FRAME_PACKET_LENGTH;
        }

        nx_http_server_callback_data_send(server_ptr, &buff_send[send_length], actual_size);
        send_length += actual_size;
    }
}

/* HTTP Thread entry function */
void http_thread_entry(ULONG thread_input)
{
    NX_IP *ip_ptr = (NX_IP*)thread_input;
    UINT status;
    NX_TCP_SOCKET mjpeg_socket;

    NX_PACKET* packet_ptr;
    NX_PACKET* send_packet;
    char* buffer;

    /* Create a packet pool.  */
    status =  nx_packet_pool_create(&g_http_packet_pool0, "NetX HTTP Packet Pool", 1540,
                                    demo_pool_area, sizeof(demo_pool_area));

    nx_ip_fragment_enable(ip_ptr);

    /* Create the HTTP Server.  */
    status = nx_http_server_create(&g_http_server0, "m-JPEG HTTP Server", ip_ptr, NX_NULL, 
                          demo_http_server_stack, sizeof(demo_http_server_stack), &g_http_packet_pool0, NX_NULL, request_notify);


    /* Start the HTTP Server. */
    nx_http_server_start(&g_http_server0);

    /* Start Motion JPEG over HTTP */
    status = nx_tcp_socket_create(ip_ptr, &mjpeg_socket, "Motion JPEG Socket", 
                                NX_IP_NORMAL, NX_FRAGMENT_OKAY, NX_IP_TIME_TO_LIVE, 1024,
                                NX_NULL, NX_NULL);

    status = nx_tcp_server_socket_listen(ip_ptr, MJPEG_PORT, &mjpeg_socket, 10, NX_NULL);

    while(1)
    {
        status = nx_tcp_server_socket_accept(&mjpeg_socket, NX_WAIT_FOREVER);

        if(nx_tcp_socket_receive(&mjpeg_socket, &packet_ptr, 1000)==NX_SUCCESS)
        {
            buffer =  (char*)packet_ptr->nx_packet_prepend_ptr;
            if(strstr(buffer, "GET / ")!=NULL)
            {
                nx_packet_release(packet_ptr);

#ifdef MJPEG_ENABLE
/* UVC process start */

                /* header send */
                nx_packet_allocate(&g_http_packet_pool0, &send_packet, NX_TCP_PACKET, NX_WAIT_FOREVER);
                nx_packet_data_append(send_packet, header_string, sizeof(header_string)-1, &g_http_packet_pool0, NX_WAIT_FOREVER);
                nx_tcp_socket_send(&mjpeg_socket, send_packet, 100);

                while(1)
                {
                    UCHAR* image;
                    ULONG length;

                    if(0<(length=display_image_get(&image)))
                    {
                        if(stream_send(&mjpeg_socket, image, length)!=NX_SUCCESS)
                        {
                            display_image_release();
                            break;
                        }

                        display_image_release();
                    }
                }

/* UVC process stop */
#else
                {
                    UCHAR* image;
                    ULONG length;

                    if(0<(length=display_image_get(&image)))
                    {
                        /* header send */
                        sprintf(buff_send, header_string, length);

                        nx_packet_allocate(&g_http_packet_pool0, &send_packet, NX_TCP_PACKET, NX_WAIT_FOREVER);
                        nx_packet_data_append(send_packet, buff_send, strlen(buff_send), &g_http_packet_pool0, NX_WAIT_FOREVER);
                        nx_tcp_socket_send(&mjpeg_socket, send_packet, 100);

                        if(stream_send(&mjpeg_socket, image, length)!=NX_SUCCESS)
                        {
                            display_image_release();
                        }
                        else
                        {
                            display_image_release();
                        }
                    }
                }
#endif
            }
            else
            {
                nx_packet_release(packet_ptr);
            }
        }

        nx_tcp_socket_disconnect(&mjpeg_socket, 100);
        nx_tcp_server_socket_unaccept(&mjpeg_socket);
        nx_tcp_server_socket_relisten(ip_ptr, MJPEG_PORT, &mjpeg_socket);
    }
}

UINT stream_send(NX_TCP_SOCKET* socket, UCHAR* image, ULONG length)
{
    UINT status = NX_SUCCESS;
    NX_PACKET* send_packet;
    char frame_message[128];
    UINT send_length = 0;
    UINT copy_length;

#ifdef MJPEG_ENABLE
    /* boundary header */
    status = nx_packet_allocate(&g_http_packet_pool0, &send_packet, NX_TCP_PACKET, NX_WAIT_FOREVER);
    if(status!=NX_SUCCESS)
    {
        return status;
    }
    sprintf(frame_message, frame_string, length);
    nx_packet_data_append(send_packet, frame_message, (ULONG)strlen(frame_message), &g_http_packet_pool0, NX_WAIT_FOREVER);
    status = nx_tcp_socket_send(socket, send_packet, NX_WAIT_FOREVER);
    if(status!=NX_SUCCESS)
    {
        nx_packet_release(send_packet);
        return status;
    }
#endif

    /* image data */
    while(send_length<(UINT)length)
    {
        status = nx_packet_allocate(&g_http_packet_pool0, &send_packet, NX_TCP_PACKET, NX_WAIT_FOREVER);
        if(status!=NX_SUCCESS)
        {
            return status;
        }
        copy_length = FRAME_PACKET_LENGTH;

        if(length<(send_length+copy_length))
        {
            copy_length = length - send_length;
        }

        nx_packet_data_append(send_packet,
                                &image[send_length],
                                copy_length,
                                &g_http_packet_pool0, NX_WAIT_FOREVER);
        status = nx_tcp_socket_send(socket, send_packet, NX_WAIT_FOREVER);
        if(status!=NX_SUCCESS)
        {
            nx_packet_release(send_packet);
            return status;
        }

        send_length += copy_length;

        /* wait for busy */
        tx_thread_sleep(1);
    }

#ifdef MJPEG_ENABLE
    status = nx_packet_allocate(&g_http_packet_pool0, &send_packet, NX_TCP_PACKET, NX_WAIT_FOREVER);
    if(status!=NX_SUCCESS)
    {
        return status;
    }
    nx_packet_data_append(send_packet, "\r\n\r\n", 4, &g_http_packet_pool0, NX_WAIT_FOREVER);
    status = nx_tcp_socket_send(socket, send_packet, NX_WAIT_FOREVER);
    if(status!=NX_SUCCESS)
    {
        nx_packet_release(send_packet);
    }
#endif

    return status;
}

