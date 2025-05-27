# RZ/A3M Microsoft Azure SDK

This is the RZ/A3M Software Development Kit with Microsoft Azure Support. The SDK contains the RZ/A3M Board Support Package and drivers for the RZ/A3M target Processors, the Asure RTOS ( ThreadX ), Azure middleware ( FileX, GUIX, USBX ). In driver GPIO code generation has been integrated with the RZ/A3M Flexible Software Package (FSP).
> **Notice: Microsoft AzureRTOS to be transitioned to Eclipse**  
> See [LICENSE.md](https://github.com/renesas-rz/rza3_gcc_azure_rtos_examples/blob/main/LICENSE.md)
> - [Azure RTOS Is Now Eclipse ThreadX | The Eclipse Foundation](https://threadx.io/)


## Getting Started
### Sample Projects
|No.|Program File|Descriptions|
|----|----|----|
|1|filex_demo_sdmmc_ek_rza3m|Sample project to check the function of FileX SDMMC.
|2|guix_demo_home_automation_ek_rza3m<br> guix_demo_washing_machine_ek_rza3m<br> guix_demo_widget_types_ek_rza3m|Sample project to check the function of GUIX.
|3|usbx_demo_hmsc_ek_rza3m|Sample project to check the function of USBX HMSC.


### Development Environment  
|Hardware|
|----|
|EK-RZA3M|  

|Software|Version|
|----|----|
|e<sup>2</sup> studio|2025-01 (25.1.0)|
|GCC ARM A-Profile|13.2.1.20231009|
|RZ/A FSP|3.5.0|  



For more information to set up your development environment, refer to the following document.   
- [Getting Started with RZ/A Flexible Software Package](https://www.renesas.com/us/en/document/qsg/getting-started-rza-flexible-software-package)
- [EK-RZ/A3M User's Manual](https://www.renesas.com/ja/document/mah/rza3m-group-users-manual-hardware?r=25572905)  

## Microsoft Azure Demos

- **filex_demo_sdmmc_ek_rza3m**  

  The "filex_demo_sdmmc_ek_rza3m" is a sample project of FileX.  
  This sample project outputs the status to the console while verifying SD card read/write.  
  #### How to Excute
  1. Connect JP9 2-3.
  2. Start the terminal software.
  3. Download the project and start debugging.
  4. Insert micro SD card into J11 connector on the board.
  5. Confirm the following message is displayed on the terminal.  
  ![Output_sample1](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/Output_sample1.png)

- **guix_demo_home_automation_ek_rza3m<br> guix_demo_washing_machine_ek_rza3m<br> guix_demo_widget_types_ek_rza3m**

  The "guix_demo_xxx_ek_rza3m" is a sample project of GUIX.  
  This sample project displays the GUI on the MIPI Graphics Expansion Board and can be operated by touch panel.  
  #### How to Excute
  1. Connect the MIPI Graphics Expansion Board to J32 connector on the board.
  2. Download the project and start debugging.
  3. Confirm that the following screen is displayed on the display and can be operated by touch panel.   
  ![Output_sample2](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/Output_sample2.png)


- **usbx_demo_hmsc_ek_rza3m**

  The "usbx_demo_hmsc_ek_rza3m" is a sample project of USBX HMSC.  
  This sample project creates, reads and writes files to the connected USB memory.  
  #### How to Excute
  1. Download the project and start debugging.
  2. Connect the USB memory to J7 connector on the board via USB Type-C Hub.
  3. Confirm that following information of the files in the connected USB memory is displayed on the terminal.   
  ![Output_sample6](https://github.com/Riku-Kiyokawa/images/blob/main/rza3_gcc_azure_rtos_examples/Output_sample6.png)





