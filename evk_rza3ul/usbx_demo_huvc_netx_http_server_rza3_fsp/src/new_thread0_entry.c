#include "new_thread0.h"

#ifdef FEATURE_NX_IPV6
#include "../../addons/http/nxd_http_server.h"
#else
#include "../../addons/http/nx_http_server.h"
#endif

#include "ux_api.h"
#include "ux_host_startup.h"

#include "bsp_api.h"

#include "ux_host_class_video.h"

#include "mjpeg_defs.h"

/* Function declarations*/
static void nx_common_init0(void);
static void packet_pool_init0(void);
static void ip_init0(void);

#define LINK_ENABLE_WAIT_TIME (1000U)


/* IP instance */
NX_IP g_ip0;

/* Stack memory for g_ip0. */
uint8_t g_ip0_stack_memory[G_IP0_TASK_STACK_SIZE];

/* ARP cache memory for g_ip0. */
uint8_t g_ip0_arp_cache_memory[G_IP0_ARP_CACHE_SIZE] BSP_ALIGN_VARIABLE(4);

/* Packet pool instance (If this is a Trustzone part, the memory must be placed in Non-secure memory). */
NX_PACKET_POOL g_packet_pool0;
uint8_t g_packet_pool0_pool_memory[G_PACKET_POOL0_PACKET_NUM * (G_PACKET_POOL0_PACKET_SIZE + sizeof(NX_PACKET))] BSP_ALIGN_VARIABLE(4);


#define OHCI_ADDRESS	(R_USB10_BASE+0x000)
#define EHCI_ADDRESS	(R_USB10_BASE+0x100)

#define EVENT_USB_PLUG_IN          (1UL << 0)
#define EVENT_USB_PLUG_OUT         (1UL << 1)
#define UX_FSP_DEVICE_INSERTION    (0x01U)
#define UX_FSP_DEVICE_REMOVAL      (0x02U)


/* device insert flag */
TX_EVENT_FLAGS_GROUP g_device_insert_eventflag;

/* data received semaphore */
TX_SEMAPHORE g_data_received_semaphore;

/* video class instance */
UX_HOST_CLASS_VIDEO* volatile video_host_class;

/* Definition of the process of adding a video buffer. */
/* Please enable either HIGH_BANDWIDTH_EHCI or NORMAL_BANDWIDTH_OHCI. */
#define HIGH_BANDWIDTH_EHCI 1
//#define NORMAL_BANDWIDTH_OHCI 1

#define VIDEO_BUFFER_SIZE   (4*1024)
UCHAR video_buffer[MAX_NUM_BUFFERS][VIDEO_BUFFER_SIZE] USB_BUFFER_PLACE_IN_SECTION;

FRAME_DATA frame_data[MAX_NUM_BUFFERS];
int frame_index = 0;

/* Camera spec data */
SPEC_OF_CAMERA camera_spec[200];
int camera_spec_count = 0;
int camera_spec_select = 0;

/* First select camera spec */
#define FIRST_SELECT_WIDTH  320
#define FIRST_SELECT_HEIGHT 240
#define FIRST_SELECT_INTERVAL   2000000

TX_THREAD http_thread;
extern void http_thread_entry(ULONG thread_input);
CHAR http_thread_stack[2048];

TX_THREAD mjpeg_thread;
extern void mjpeg_thread_entry(ULONG thread_input);
CHAR mjpeg_thread_stack[8*1024];


FX_FILE     g_file;
FX_MEDIA *  g_p_media       = UX_NULL;
ULONG       actual_flags    = 0;

ULONG   error_counter = 0;

/*******************************************************************************************************************//**
 * @brief     In this function, checks the USB device status and notifies to perform operation
 * @param[IN] event       updates usb events on attaching and dettaching device.
 * @param[IN] *host_class  pointer to host class device structure
 * @param[IN] instance
 * @retval    none
 **********************************************************************************************************************/
static UINT apl_change_function (ULONG event, UX_HOST_CLASS * host_class, VOID * instance)
{
    UINT                          status = UX_SUCCESS;

    if (UX_SUCCESS
            == _ux_utility_memory_compare (_ux_system_host_class_video_name, host_class,
                                           _ux_utility_string_length_get(_ux_system_host_class_video_name)))
    {
        if (UX_DEVICE_INSERTION == event) /* Check if there is a device insertion. */
        {
            video_host_class = instance;

            /* Set the event flag to let application know the device insertion. */
            tx_event_flags_set(&g_usb_plug_events, EVENT_USB_PLUG_IN, TX_OR);
        }
        else if(UX_DEVICE_REMOVAL == event)
        {
            /* Clear the event flag in case the camera was removed before the application could clear it. */
            tx_event_flags_set(&g_usb_plug_events, EVENT_USB_PLUG_OUT, TX_OR);

            video_host_class = NULL;
        }
    }

    return status;
}

static UINT usb_host_initialization(void)
{
    UINT status;

    /* ux host stack initialization */
    status =  ux_host_stack_initialize(apl_change_function);
    if (status != UX_SUCCESS)
    {
        printf("UX_HOST_STACK_INITIALIZE API FAILED..\r\n");
        return UX_ERROR;
    }

    /* Register all the USB host controllers available in this system */
    status =  ux_host_stack_hcd_register(_ux_system_host_hcd_ohci_name, ux_hcd_ohci_initialize, OHCI_ADDRESS, 0x0);
    if (status != UX_SUCCESS)
    {
        printf("OHCI regist FAILED..\r\n");
        return UX_ERROR;
    }

    status =  ux_host_stack_hcd_register(_ux_system_host_hcd_ehci_name, ux_hcd_ehci_initialize, EHCI_ADDRESS, 0x0);
    if (status != UX_SUCCESS)
    {
        printf("EHCI regist FAILED..\r\n");
        return UX_ERROR;
    }

    return UX_SUCCESS;
}

/* Video data received callback function. */ 
VOID uvc_transfer_request_done_callback(UX_TRANSFER * transfer_request)
{
    /* This is the callback function invoked by UVC class after a packet of 
       data is received. */

    /* The actual number of bytes being received into the data buffer is 
       recorded in transfer_request -> ux_transfer_request_actual_length. */ 

    /* Since this callback function executes in the USB host controller
       thread, a semaphore is released so the application can pick up the 
       video data in application thread. */

    frame_data[frame_index].length = transfer_request->ux_transfer_request_actual_length;

    frame_index++;
    if(MAX_NUM_BUFFERS<=frame_index)
    {
        frame_index = 0;
    }

    tx_semaphore_put(&g_data_received_semaphore); 
} 

/* Show the interval types */
VOID uvc_parameter_interval_list(UX_HOST_CLASS_VIDEO *video, int width, int height)
{
    UX_HOST_CLASS_VIDEO_FRAME_DESCRIPTOR frame_descriptor;

    ULONG min_frame_interval;
    ULONG max_frame_interval;
    ULONG frame_interval_step;
    int i;

    /* Make the descriptor machine independent.  */
    _ux_utility_descriptor_parse(video -> ux_host_class_video_current_frame_address,
                                 _ux_system_class_video_frame_descriptor_structure,
                                 UX_HOST_CLASS_VIDEO_FRAME_DESCRIPTOR_ENTRIES, (UCHAR *) &frame_descriptor);

    /* Check the frame interval type.  */
    if (frame_descriptor.bFrameIntervalType == 0)
    {
        if(camera_spec_count<(int)(sizeof(camera_spec)/sizeof(camera_spec[0])))
        {
            /* Frame interval type is continuous.  */
            min_frame_interval = _ux_utility_long_get(video -> ux_host_class_video_current_frame_address + 26);
            max_frame_interval = _ux_utility_long_get(video -> ux_host_class_video_current_frame_address + 30);
            frame_interval_step = _ux_utility_long_get(video -> ux_host_class_video_current_frame_address + 34);

            camera_spec[camera_spec_count].width = width;
            camera_spec[camera_spec_count].height = height;
            camera_spec[camera_spec_count].interval = (int)min_frame_interval;

            if(0<camera_spec_count)
            {
                /* select low-level spec */
                if(width<camera_spec[camera_spec_select].width)
                {
                    camera_spec_select = camera_spec_count;
                }
            }

            camera_spec_count++;
        }
    }
    else
    {
        /* Frame interval type is discrete.  */
        for(i = 0; i < (int)frame_descriptor.bFrameIntervalType; i++)
        {
            if(camera_spec_count<(int)(sizeof(camera_spec)/sizeof(camera_spec[0])))
            {
                camera_spec[camera_spec_count].width = width;
                camera_spec[camera_spec_count].height = height;
                camera_spec[camera_spec_count].interval = (int)_ux_utility_long_get(video -> ux_host_class_video_current_frame_address +
                                                            26 + (unsigned int)i * sizeof(ULONG));

                if(0<camera_spec_count)
                {
                    /* select low-level spec */
                    if(width<camera_spec[camera_spec_select].width)
                    {
                        camera_spec_select = camera_spec_count;
                    }
                }

                camera_spec_count++;
            }
            else
            {
                break;
            }
        }
    }
}

/* Show the frame resolutions */
UINT uvc_parameter_frame_list(UX_HOST_CLASS_VIDEO *video)
{
    ULONG frames_index;
    UX_HOST_CLASS_VIDEO_PARAMETER_FRAME_DATA frame_parameter;

    UINT status = UX_SUCCESS;

    /* frame resolutions */
    for (frames_index = 1; frames_index <= video -> ux_host_class_video_number_frames; frames_index++)
    {
        /* Get frame data for current frame index.  */
        frame_parameter.ux_host_class_video_parameter_frame_requested = frames_index;
        status = _ux_host_class_video_frame_data_get(video, &frame_parameter);
        if (status != UX_SUCCESS)
        {
            return(status);
        }

        /* Save the current frame index.  */
        video -> ux_host_class_video_current_frame = frames_index;

        uvc_parameter_interval_list(video,
            (int)frame_parameter.ux_host_class_video_parameter_frame_width,
            (int)frame_parameter.ux_host_class_video_parameter_frame_height);
    }

    return(status);
}


/* Show the device parameters */
VOID uvc_parameter_list(UX_HOST_CLASS_VIDEO *video, int format)
{
    ULONG format_index;
    UX_HOST_CLASS_VIDEO_PARAMETER_FORMAT_DATA format_parameter;

    UINT status = UX_SUCCESS;

    /* format types */
    for (format_index = 1; format_index <= video -> ux_host_class_video_number_formats; format_index++)
    {
        /* Get format data for current format index.  */
        format_parameter.ux_host_class_video_parameter_format_requested = format_index;
        status = _ux_host_class_video_format_data_get(video, &format_parameter);
        if (status == UX_SUCCESS)
        {
            if((int)format_parameter.ux_host_class_video_parameter_format_subtype==format)
            {
                /* Save number of frames in the video instance.  */
                video -> ux_host_class_video_number_frames = format_parameter.ux_host_class_video_parameter_number_frame_descriptors;

                uvc_parameter_frame_list(video);
                break;
            }
        }
    }
}

VOID uvc_frame_finish(UCHAR* p_frame)
{
    UINT old_threshold;

    tx_thread_preemption_change(tx_thread_identify(), 0, &old_threshold);
    if(video_host_class)
    {
        /* Add the buffer back for video transfer. */
        ux_host_class_video_transfer_buffer_add(video_host_class, p_frame); 
    }
    tx_thread_preemption_change(tx_thread_identify(), old_threshold, &old_threshold);
}

int uvc_camera_spec_get(SPEC_OF_CAMERA** p_camera_spec, int* p_select)
{
    if(p_camera_spec!=NULL)
    {
        *p_camera_spec = camera_spec;
    }
    if(p_select!=NULL)
    {
        *p_select = camera_spec_select;
    }

    return camera_spec_count;
}

void uvc_camera_spec_set(int select)
{
    if(select<camera_spec_count)
    {
        camera_spec_select = select;
    }
}

void uvc_camera_stop(void)
{
    if(video_host_class!=NULL)
    {
        /* Stop video transfer.  */
        ux_host_class_video_stop(video_host_class);
    }
}

void uvc_camera_start(void)
{
    UINT status;

    /* Maximum buffer requirement reported by the video device. */ 
    ULONG max_buffer_size; 

    UX_HOST_CLASS_VIDEO_PARAMETER_CHANNEL channel;

    /* Index variable keeping track of the current buffer being used by the video device. */ 
    ULONG buffer_index; 

    if(video_host_class!=NULL)
    {
        /* Set video parameters. This setting value is a dummy. 
           Depending on the application, set the necessary parameters. */
        if(camera_spec_select<camera_spec_count)
        {
            status = ux_host_class_video_frame_parameters_set(video_host_class,
                        UX_HOST_CLASS_VIDEO_VS_FORMAT_MJPEG,
                        (ULONG)camera_spec[camera_spec_select].width, camera_spec[camera_spec_select].height,
                        (ULONG)camera_spec[camera_spec_select].interval);
        }
        else
        {
            status = ux_host_class_video_frame_parameters_set(video_host_class,
                        UX_HOST_CLASS_VIDEO_VS_FORMAT_MJPEG,
                        FIRST_SELECT_WIDTH, FIRST_SELECT_HEIGHT,
                        FIRST_SELECT_INTERVAL);
        }

        /* Set the user callback function of video class. */
        ux_host_class_video_transfer_callback_set(video_host_class, uvc_transfer_request_done_callback);

        /* Find out the maximum memory buffer size for the video configuration
           set above. */
        max_buffer_size = ux_host_class_video_max_payload_get(video_host_class);

        /* Clear semaphore to zero */
        while (1)
        {
            if(tx_semaphore_get(&g_data_received_semaphore, 0)==TX_NO_INSTANCE)
            {
                break;
            }
        }

        /* Start video transfer.  */
        status = ux_host_class_video_start(video_host_class);
        if(status!=UX_SUCCESS)
        {
            /* Setting these to zero is a hack since we're mixing old and new APIs (new API does this and is required for reads). */
            video_host_class -> ux_host_class_video_transfer_request_start_index = 0;
            video_host_class -> ux_host_class_video_transfer_request_end_index = 0;

            channel.ux_host_class_video_parameter_format_requested = video_host_class -> ux_host_class_video_current_format;
            channel.ux_host_class_video_parameter_frame_requested = video_host_class -> ux_host_class_video_current_frame;
            channel.ux_host_class_video_parameter_frame_interval_requested = video_host_class -> ux_host_class_video_current_frame_interval;
            channel.ux_host_class_video_parameter_channel_bandwidth_selection = 1024;

            status = ux_host_class_video_ioctl(video_host_class, UX_HOST_CLASS_VIDEO_IOCTL_CHANNEL_START, &channel);
        }

#if HIGH_BANDWIDTH_EHCI /* Driver HCD must support adding requests list.  */
        {
        UCHAR* video_buffers[MAX_NUM_BUFFERS];

        for(buffer_index = 0; buffer_index < MAX_NUM_BUFFERS; buffer_index++)
        {
            frame_data[buffer_index].p_frame = (ULONG)video_buffer[buffer_index];
            video_buffers[buffer_index] = (UCHAR*)frame_data[buffer_index].p_frame;
        }

        /* Issue transfer request list to start streaming.  */
        status = ux_host_class_video_transfer_buffers_add(video_host_class, video_buffers, MAX_NUM_BUFFERS);
        }
#elif NORMAL_BANDWIDTH_OHCI /* Driver adds request one by one.  */
        /* Allocate space for video buffer. */     
        for(buffer_index = 0; buffer_index < MAX_NUM_BUFFERS; buffer_index++)
        {
            frame_data[buffer_index].p_frame = (ULONG)video_buffer[buffer_index];

            /* Add buffer to the video device for video streaming data. */
            ux_host_class_video_transfer_buffer_add(video_host_class, frame_data[buffer_index].p_frame);
        }
#endif
    }

    frame_index = 0;
}


/* USB Thread entry function */
void new_thread0_entry(void)
{
    UINT status;
    int     loop_count;

    /* Create the flags and the semaphore for video control.  */
    status = tx_event_flags_create (&g_device_insert_eventflag, (CHAR *)"Device Insert Event Flags");
    if(status!=TX_SUCCESS)
    {
        printf("UVC event flags create FAILED..\r\n");
        return;
    }

    status = tx_semaphore_create (&g_data_received_semaphore, (CHAR *) "Data Receive Semaphore", 0);
    if(status!=TX_SUCCESS)
    {
        printf("UVC semaphore create FAILED..\r\n");
        return;
    }

#if 1
    /* Initialize the NetX system.*/
    nx_common_init0();

    /* Initialize the packet pool.*/
    packet_pool_init0();

    /* create the ip instance.*/
    ip_init0();

    /* Create the http thread.  */
    status = tx_thread_create (&http_thread, (CHAR *) "HTTP Thread", http_thread_entry, (ULONG)&g_ip0,
                        &http_thread_stack, sizeof(http_thread_stack), 30, 30, 1, TX_AUTO_START);
    if (status != UX_SUCCESS)
    {
        printf("http thread create FAILED..\r\n");
        return;
    }
#endif

    status = ux_host_startup(&g_basic0_ctrl, &g_basic0_cfg, usb_host_initialization);
    if (status != UX_SUCCESS)
    {
        printf("ux_host_startup API FAILED..\r\n");
        return;
    }

    printf("Init completed!\r\n\r\n");

    printf("Please insert USB camera for work.\r\n");

    /*  Wait until device inserted.*/
    tx_event_flags_get (&g_usb_plug_events, EVENT_USB_PLUG_IN, TX_AND_CLEAR, &actual_flags, TX_WAIT_FOREVER);
    if(EVENT_USB_PLUG_IN == actual_flags)
    {
        printf("USB camera is inserted\r\n");
    }

    /* Reset the event flag */
    actual_flags = 0;
    loop_count = 0;

    /* List parameters */
    camera_spec_count = 0;
    camera_spec_select = 0;
    uvc_parameter_list(video_host_class, UX_HOST_CLASS_VIDEO_VS_FORMAT_MJPEG);

    /* Create the mjpeg thread.  */
    status = tx_thread_create (&mjpeg_thread, (CHAR *) "MJPEG Thread", mjpeg_thread_entry, (ULONG)NULL,
                        &mjpeg_thread_stack, sizeof(mjpeg_thread_stack), 22, 22, 0, TX_AUTO_START);
    if(status!=TX_SUCCESS)
    {
        printf("MJPEG thread failed.\r\n");
    }

    /* This delay is required for now to get valid ISO IN UX_ENDPOINT instance. */
    tx_thread_sleep(100);

    while (true)
    {
        loop_count++;

        /*  Check if USB is plugged out.*/
        tx_event_flags_get (&g_usb_plug_events, EVENT_USB_PLUG_OUT, TX_AND_CLEAR, &actual_flags, TX_NO_WAIT);
        if(EVENT_USB_PLUG_OUT == actual_flags)
        {
            printf("\r\nPlease insert USB camera for work.\r\n\r\n");
            actual_flags = 0;

            /*  Wait until device inserted.*/
            while(EVENT_USB_PLUG_IN != actual_flags)
            {
                tx_event_flags_get (&g_usb_plug_events, EVENT_USB_PLUG_IN, TX_AND_CLEAR, &actual_flags, TX_WAIT_FOREVER);
                if(EVENT_USB_PLUG_IN == actual_flags)
                {
                    printf("USB camera is inserted\r\n");
                    loop_count = 0;

                    /* This delay is required for now to get valid ISO IN UX_ENDPOINT instance. */
                    tx_thread_sleep(100);

                    /* List parameters */
                    camera_spec_count = 0;
                    camera_spec_select = 0;
                    uvc_parameter_list(video_host_class, UX_HOST_CLASS_VIDEO_VS_FORMAT_MJPEG);
                }
            }
        }
        else
        {
            tx_thread_sleep(100);
        }
    }
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
		;
    }

    /* Set the gateway address if it is configured. */
    if (IP_ADDRESS(0, 0, 0, 0) != G_IP0_GATEWAY_ADDRESS)
    {
        status = nx_ip_gateway_address_set (&g_ip0, G_IP0_GATEWAY_ADDRESS);
        if(NX_SUCCESS != status)
        {
			;
        }
    }

    /* Enable IP fragment. */
    status = nx_ip_fragment_enable(&g_ip0);
    if(NX_SUCCESS != status)
    {
		;
    }

    /* Enables Address Resolution Protocol (ARP).*/
    status = nx_arp_enable(&g_ip0, &g_ip0_arp_cache_memory[0], G_IP0_ARP_CACHE_SIZE);
    if(NX_SUCCESS != status)
    {
		;
    }

    /* Enable udp.*/
    status = nx_udp_enable(&g_ip0);
    if(NX_SUCCESS != status)
    {
		;
    }

    /* Enable tcp.*/
    status = nx_tcp_enable(&g_ip0);
    if(NX_SUCCESS != status)
    {
		;
    }

    /* Enable icmp.*/
    status = nx_icmp_enable(&g_ip0);
    if(NX_SUCCESS != status)
    {
		;
    }

    /* Wait for the link to be enabled. */
    ULONG current_state;
    status = nx_ip_status_check(&g_ip0, NX_IP_LINK_ENABLED, &current_state, LINK_ENABLE_WAIT_TIME);
    if(NX_SUCCESS != status)
    {
		;
    }

}

