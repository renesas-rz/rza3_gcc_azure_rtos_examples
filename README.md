# RZ/A3UL Microsoft Azure SDK

This is the RZ/A3UL Software Development Kit with Microsoft Azure Support. The SDK contains the RZ/A3UL Board Support Package and drivers for the RZ/A3UL target Processors, the Asure RTOS ( ThreadX ), Azure middleware ( FileX, GUIX, NetX duo, USBX ). In driver GPIO code generation has been integrated with the RZ/A3UL Flexible Software Package (FSP).

## Microsoft Azure Libraries

The [Microsoft Azure software](https://azure.microsoft.com/en-us/free/search/?&ef_id=EAIaIQobChMIo6et7eiu8AIVAh-tBh2tygWBEAAYAiAAEgIHcPD_BwE:G:s&OCID=AID2100131_SEM_EAIaIQobChMIo6et7eiu8AIVAh-tBh2tygWBEAAYAiAAEgIHcPD_BwE:G:s&gclid=EAIaIQobChMIo6et7eiu8AIVAh-tBh2tygWBEAAYAiAAEgIHcPD_BwE) is supported by the Azure Microsoft Team.  This repository includes the library debug binaries. The full source code for the listed libraries can be found [here](https://github.com/azure-rtos/).

License

- **Threadx**  
  
  This is the Microsoft Azure RTOS.  
  To learn more about [Azure RTOS ThreadX](https://docs.microsoft.com/azure/rtos/threadx/).

- **USBx**  

  This is the USB middleware. It support both Host and Device. USB Class support include HID, MSC, CDC,   HUB, UVC, Audio, and printer.  
  To learn more about [Azure RTOS USBX](https://docs.microsoft.com/azure/rtos/usbx/).  

- **GUIx**  

  This the GUI middleware. It supports 16, 24 and 32 bit color space. Resolution is dependent on the amount of internal memory ( i.e. 4MB RZ/A3UL ) or external memory.  
  To learn more about [Azure RTOS GUIX](https://docs.microsoft.com/azure/rtos/guix/).  

- **FileX**  

  This is the RTOS file system. Supports RAM disk, USB and SD.  
  To learn more about [Azure RTOS FileX](https://docs.microsoft.com/azure/rtos/filex/).  

- **NetXDuo**  

  This middware supports the network for IP4, Ip6, TCP, UDP, HTTP, Secure Socket Layer, Crypto libraries, and many more.  
  To learn more about [Azure RTOS NetX Duo](https://docs.microsoft.com/azure/rtos/netx-duo/).  


- **Azure IoT**  

  As part of the NetxDuo the Azure IoT Cloud connectivity is included.  



## Getting Started
### Sample Projects
|No.|Program File|Descriptions|
|----|----|----|
|1|filex_demo_sdmmc_rza3_fsp_1.0|Sample project to check the function of FileX.
|2|guix_demo_usbx_hhid_mouse_rza3_fsp_1.1|Sample project to check the function of GUIX and USBX.
|3|netx_demo_http_server_rza3_fsp_1.0|Sample project to check the function of NetX duo.
|4|usbx_demo_hcdc_rza3_fsp_1.0|Sample project to check the function of USBX HCDC.
|5|usbx_demo_hhid_rza3_fsp_1.0|Sample project to check the function of USBX HHID.
|6|usbx_demo_hmsc_rza3_fsp_1.0|Sample project to check the function of USBX HMSC.
|7|usbx_demo_huvc_netx_http_server_rza3_fsp_1.0|Sample project to check the function of USBX HUVC.
|8|RZA3UL_demo_azure_iot_pnp_ 1.0|Sample project to check the function of Azure IoT Middleware.

### Development Environment  
|Software|Version|
|----|----|
|e<sup>2</sup> studio|2023 04 (23.4.0)<sup>[*1](#note1)</sup>|
|RZ/A FSP|2.0.1| 
<small id="note1">*1: It does not work with latest version of e2 studio (e 2 studio 2023 07 ). Please use e2studio 2023-04</small>  

For more information to set up your development environment, refer to the following document.   
- ["Getting Started with RZ/A Flexible Software Package"](https://www.renesas.com/us/en/document/qsg/getting-started-rza-flexible-software-package-v201)  

## Microsoft Azure Demos

- **filex_demo_sdmmc_rza3_fsp**  

  The "filex_demo_sdmmc_rza3_fsp" is a sample project of FileX.  
  This sample project outputs the status to the console while verifying SD card read/write.  
  ### How to Excute 

- **demo_usbx_hhid_mouse_rza3_fsp**

  The "demo_usbx_hhid_mouse_rza3_fsp" is a sample project of GUIX and USBX.  
  This sample project displays the GUI on the HDMI monitor and can be operated with the USB mouse.  

- **netx_demo_http_server_rza3_fsp**

  The "netx_demo_http_server_rza3_fsp" is a sample project of NetX duo.  
  This sample project works as an HTTP server and responds to HTTP requests from browser.  

- **usbx_demo_hcdc_rza3_fsp**

  The "usbx_demo_hcdc_rza3_fsp" is a sample project of USBX HCDC.  
  This sample project sends an AT command and displays the response in the log when connect a device that acts as a PCDC.  

- **usbx_demo_hhid_rza3_fsp**

  The "usbx_demo_hhid_rza3_fsp" is a sample project of USBX HHID.  
  This sample project detects the USB mouse and output the mouse position information.  

- **usbx_demo_hmsc_rza3_fsp**

  The "usbx_demo_hmsc_rza3_fsp" is a sample project of USBX HMSC.  
  This sample project creates, reads and writes files to the connected USB memory.  

- **usbx_demo_huvc_netx_http_server_rza3_fsp**

  The "usbx_demo_huvc_netx_http_server_rza3_fsp" is a sample project of USBX HUVC.  
  This sample project displays the camera image on the HTTP server in real time.  

- **RZA3UL_demo_azure_iot**  
  The "RZA3UL_demo_azure_iot" is a sample project to connect to Azure IoT Hub using Azure IoT Middleware for Azure RTOS.  
  This sample project supports IoT Plug and Play with multiple components.

  > Note: RZ/A3UL EVK is certified ["Azure Certified Device"](https://learn.microsoft.com/en-us/azure/certification/program-requirements-azure-certified-device) and ["IoT Plug and Play"](https://learn.microsoft.com/en-us/azure/certification/program-requirements-pnp) certifications, and is listed in ["Azure Certified Device catalog"](https://devicecatalog.azure.com/devices/5848d87b-5d3e-4c11-ad76-90612639b025).

  ![test](https://github.com/Riku-Kiyokawa/test/blob/main/WIN_20230613_16_34_02_Pro.jpg)