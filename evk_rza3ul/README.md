# RZ/A3UL Microsoft Azure SDK

This is the RZ/A3UL Software Development Kit with Microsoft Azure Support. The SDK contains the RZ/A3UL Board Support Package and drivers for the RZ/A3UL target Processors, the Asure RTOS ( ThreadX ), Azure middleware ( FileX, GUIX, NetX duo, USBX ). In driver GPIO code generation has been integrated with the RZ/A3UL Flexible Software Package (FSP).
> **Notice: Microsoft AzureRTOS to be transitioned to Eclipse**  
> See [LICENSE.md](https://github.com/renesas-rz/rza3_gcc_azure_rtos_examples/blob/main/LICENSE.md)


## Getting Started
### Sample Projects
|No.|Program File|Descriptions|
|----|----|----|
|1|filex_demo_sdmmc_rza3_fsp|Sample project to check the function of FileX.
|2|guix_demo_home_automation_rza3_fsp<br> guix_demo_washing_machine_rza3_fsp<br> guix_demo_widget_types_rza3_fsp|Sample project to check the function of GUIX and USBX.
|3|netx_demo_http_server_rza3_fsp|Sample project to check the function of NetX duo.
|4|usbx_demo_hcdc_rza3_fsp|Sample project to check the function of USBX HCDC.
|5|usbx_demo_hhid_rza3_fsp|Sample project to check the function of USBX HHID.
|6|usbx_demo_hmsc_rza3_fsp|Sample project to check the function of USBX HMSC.
|7|usbx_demo_huvc_netx_http_server_rza3_fsp|Sample project to check the function of USBX HUVC.
|8|usbx_demo_pcdc_rza3_fsp|Sample project to check the function of USBX PCDC.
|9|RZA3UL_demo_azure_iot_pnp|Sample project to check the function of Azure IoT Middleware.


### Development Environment  
|Hardware|Version|
|----|----|
|[RZ/A3UL-EVKIT](https://www.renesas.com/products/microcontrollers-microprocessors/rz-mpus/rza3ul-evkit-rza3ul-evaluation-board-kit)|QSPI Eddition|
|[Segger J-link](https://www.segger.com/products/debug-probes/j-link/)| - |
> Note: RZ/A3UL-EVKIT Octal-SPI Eddition is not supported.

|Software|Version|
|----|----|
|e<sup>2</sup> studio|2024-07 (24.7.0)|
|GCC ARM A-Profile|13.2.1.20231009|
|RZ/A FSP|3.2.0|  

For more information to set up your development environment, refer to the following document.   
- ["Getting Started with RZ/A Flexible Software Package"](https://www.renesas.com/us/en/document/qsg/getting-started-rza-flexible-software-package-v202)  

## Microsoft Azure Demos

- **filex_demo_sdmmc_rza3_fsp**  

  The "filex_demo_sdmmc_rza3_fsp" is a sample project of FileX.  
  This sample project outputs the status to the console while verifying SD card read/write.  
  #### How to Excute
  1. Set the SW1-2 on the board to ON.
  2. Start the terminal software.
  3. Download the project and start debugging.
  4. Insert micro SD card into CN2 connector on the board.
  5. Confirm the following message is displayed on the terminal.  
  ![BoardSetting_sample1](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/BoardSetting_sample1.png)  
  ![Output_sample1](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/Output_sample1.png)

- **guix_demo_home_automation_rza3_fsp<br> guix_demo_washing_machine_rza3_fsp<br> guix_demo_widget_types_rza3_fsp**

  The "guix_demo_xxx_rza3_fsp" is a sample project of GUIX and USBX.  
  This sample project displays the GUI on the HDMI monitor and can be operated with the USB mouse.  
  #### How to Excute
  1. Connect the HDMI monitor to CN5 connector on the board via Parallel to HDMI Conversion board.
  2. Download the project and start debugging.
  3. Connect the USB mouse to CN12 connector on the board.
  4. Confirm that the following screen is displayed on the HDMI monitor.  
  ![BoardSetting_sample2](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/BoardSetting_sample2.png)  
  ![Output_sample2](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/Output_sample2.png)

- **netx_demo_http_server_rza3_fsp**

  The "netx_demo_http_server_rza3_fsp" is a sample project of NetX duo.  
  This sample project works as an HTTP server and responds to HTTP requests from browser.  
  #### How to Excute
  1. Connect the Ethernet connector of the PC and CN8 connector on the board.
  2. Configure a static IP address for the board.
  3. Download the project and start debugging.
  4. Access the IP address “192.168.2.182” and confirm that following browser is displayed in the browser.  
  ![BoardSetting_sample3](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/BoardSetting_sample3.png)  
  ![Output_sample3](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/Output_sample3.png)

- **usbx_demo_hcdc_rza3_fsp**

  The "usbx_demo_hcdc_rza3_fsp" is a sample project of USBX HCDC.  
  This sample project sends an AT command and displays the response in the log when connect a device that acts as a PCDC.  
  #### How to Excute
  1. Run RZA3UL_USB_PCDC_2_0_Release project on one RZ/A3UL board to act as PCDC device.
  2. Start the terminal software.
  3. Download the project and start debugging.
  4. Connect the PCDC device to CN12 connector on the board.
  5. Confirm the following message is displayed on the terminal.  
  ![BoardSetting_sample4](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/BoardSetting_sample4.png)  
  ![Output_sample4](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/Output_sample4.png)

- **usbx_demo_hhid_rza3_fsp**

  The "usbx_demo_hhid_rza3_fsp" is a sample project of USBX HHID.  
  This sample project detects the USB mouse and output the mouse position information.  
  #### How to Excute
  1. Start the terminal software.
  2. Download the project and start debugging.
  3. Connect the USB mouse to CN12 connector on the board.
  4. Confirm the following mouse position information is displayed on the terminal.  
  ![BoardSetting_sample5](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/BoardSetting_sample5.png)  
  ![Output_sample5](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/Output_sample5.png)

- **usbx_demo_hmsc_rza3_fsp**

  The "usbx_demo_hmsc_rza3_fsp" is a sample project of USBX HMSC.  
  This sample project creates, reads and writes files to the connected USB memory.  
  #### How to Excute
  1. Download the project and start debugging.
  2. Connect the USB memory to CN12 connector on the board.
  3. Confirm that following information of the files in the connected USB memory is displayed on the terminal.  
  ![BoardSetting_sample6](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/BoardSetting_sample6.png)  
  ![Output_sample6](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/Output_sample6.png)

- **usbx_demo_huvc_netx_http_server_rza3_fsp**

  The "usbx_demo_huvc_netx_http_server_rza3_fsp" is a sample project of USBX HUVC.  
  This sample project displays the camera image on the HTTP server in real time.  
  #### How to Excute
  1. Connect the Ethernet connector of the PC and CN8 connector on the board.
  2. Connect the UVC camera to CN12 connector on the board.
  3. Configure a static IP address for the board.
  4. Download the project and start debugging.
  5. Access the IP address “192.168.2.182” and confirm that camera image is displayed in the browser.    
  ![BoardSetting_sample7](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/BoardSetting_sample7.png)  
  ![Output_sample7](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/Output_sample7.png)

- **usbx_demo_pcdc_rza3_fsp**

  The "usbx_demo_pcdc_rza3_fsp" is a sample project of USBX PCDC.  
  This sample project echoes back the input characters on the terminal.  
  #### How to Excute
  1. Connect the PC and CN11 connector on the board.
  2. Download the project and start debugging.
  3. Start the terminal software and connect to newly recognized COM port.
  4. Comfirm that input characters are echoed back on the terminal.  
  ![BoardSetting_sample8](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/BoardSetting_sample8.png)  
  ![Output_sample8](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/Output_sample8.png)

- **RZA3UL_demo_azure_iot**  
  The "RZA3UL_demo_azure_iot" is a sample project to connect to Azure IoT Hub using Azure IoT Middleware for Azure RTOS.  
  This sample project supports IoT Plug and Play with multiple components.

  > **Azure Certified Device is no longer in servise.**  
  ~~Note: RZ/A3UL EVK is certified ["Azure Certified Device"](https://learn.microsoft.com/en-us/azure/certification/program-requirements-azure-certified-device) and ["IoT Plug and Play"](https://learn.microsoft.com/en-us/azure/certification/program-requirements-pnp) certifications, and is listed in ["Azure Certified Device catalog"](https://devicecatalog.azure.com/devices/5848d87b-5d3e-4c11-ad76-90612639b025).~~

  #### How to Excute
  Please refer to [Get started guide](https://github.com/renesas-rz/rza3_gcc_azure_rtos_examples/blob/main/rza3ul_evk_plug-and-play.pdf).




