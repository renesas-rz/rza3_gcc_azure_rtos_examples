/* generated configuration header file - do not edit */
#ifndef RM_USBX_PORT_CFG_H_
#define RM_USBX_PORT_CFG_H_
/* definition of USBX user application pool size */
#define USB_HOST_POOL_SIZE 131072
#define USB_HOST_UNCACHED_POOL_SIZE 131072

/* definition of USBX host classes property */
#if (FSP_NOT_DEFINED != FSP_NOT_DEFINED)
#define USB_HOST_STORAGE_CLASS_REGISTER
#endif
#if (FSP_NOT_DEFINED != FSP_NOT_DEFINED)
#define USB_HOST_HID_CLASS_REGISTER
#include "r_usb_hhid_cfg.h"
#endif
#if (FSP_NOT_DEFINED != FSP_NOT_DEFINED)
#define USB_HOST_CDC_ACM_CLASS_REGISTER
#include "r_usb_hcdc_cfg.h"
#endif
#if (1 != FSP_NOT_DEFINED)
#define USB_HOST_VIDEO_CLASS_REGISTER
#include "r_usb_huvc_cfg.h"
#endif
#define USB_HOST_HUB_CLASS_REGISTER
#endif /* RM_USBX_PORT_CFG_H_ */
