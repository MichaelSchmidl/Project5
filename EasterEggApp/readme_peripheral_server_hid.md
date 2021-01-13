Keyboard Peripheral Device with Server Sample Code
====================================================

NOTE: If you use this sample application for your own purposes, follow
      the licensing agreement specified in `Software_Use_Agreement.rtf`
      in the home directory of the installed RSL10 Software
      Development Kit (SDK).

Overview
--------
This project demonstrates the functionality of a wireless input device
(keyboard/mouse) by making use of the BLE HID OVER GATT Profile (HOGP).
Two entities are defined in the Human Interface Device (HID) protocol: the
*host*, and the *device*. The device is the entity that directly interacts 
with a human, such as a keyboard or mouse. The most common example of a host 
is a PC or a tablet. The host communicates with the device by receiving input 
reports (data flows from the device to the host) and sending output reports 
(data flows from the host to the device). Additionally, a feature report is 
exchanged between the device and host wherein various configuration 
information is specified.

In addition to the HID service, this project generates a battery service
and a device information service. The device information service is added
to the kernel and database. The RSL10 ADC is used to read the battery level
value every 200 ms when there is a kernel timer event. The average for 16 
reads is calculated, and if this average value changes, a flag is set to send 
the battery level notification. When the device is bonded to a peer device `(VALID_BOND_INFO(ble_env[i].bond_info.STATE) is true)`, it can start sending 
the battery level notifications.

When this sample code is executed, the device begins advertising in the
undirected connectable mode. For this sample project, any central device can
scan, connect, pair/bond, perform service discovery, receive battery value
notifications, and receive/send input/output reports from the HID device. This
project primarily showcases how to handle the input, output, and feature
reports of the HOGP BLE profile to interface with BLE HID peripheral devices.

By default, this application allows the peripheral device to pair with only
one central device. This number can be increased by adjusting the 
`NUM_MASTERS` define, which determines the number of central devices that can 
pair with the RSL10. However, only the first device that is paired with the 
RSL10 in the connection sequence will be able to interact with the HID 
service. When a central device sends the pairing request, various pre-defined
connection/configuration parameters are exchanged intially. The
`SECURE_CONNECTION` definition, used in the `GAPC_BondReqInd` function, can be
defined/undefined to achieve the desired security level and to specify which
keys will be exchanged. The `GAPC_BondReqInd` function is also responsible for
generating the LTK to be sent to the central device. Note that in 
`GAPC_BondInd`, the remote IRK and CSRK are received while the local IRK and 
CSRK are read from `NVR3` as default device security information. The 
generated LTK, remote IRK and remote CSRK values are saved into non-volatile 
record two (`NVR2`) in flash.

Depending on the device address type selected by the application
(in `ble_std.h`), either a public or private address is used for the device. 
When the public address type is selected, one of the following addresses is 
used:

1.  A public address provided by the application. 
2.  If a public address is not provided by the application, the device's
    public address that is available in `DEVICE_INFO_BLUETOOTH_ADDR` (located 
    in `NVR3`) is used. 
3.  If `NVR3` does not have a public address, a default public address that is
    predefined in the stack is used. 

When the private address type is selected, the private address provided by the
application is used.

This sample project passes through several states before all services are
enabled:

1.  `APPM_INIT` (initialization)
    Application initializing and is configured into an idle state. 
2.  `APPM_CREATE_DB` (create database)
    Application has configured the Bluetooth stack, including GAP, according 
    to the required role and features. It is now adding services, handlers, 
    and characteristics for all services that it will provide.
3.  `APPM_READY` (ready)
    Generated for each peripheral instance. Application has added the desired
    standard and custom services or profiles into the Bluetooth GATT database
    and handlers.
4.  `APPM_ADVERTISING` (advertising)
    Generated for each peripheral instance. The device starts advertising 
    based on the sample project.
5.  `APPM_CONNECTED` (connected)
    Generated for each peripheral instance. Connection is now established with
    another compatible device.

After the connection is established, the `BLE_SetServiceState` function is 
called, in which, for any profiles/services that need to be enabled, an enable 
request message is sent to the corresponding profile of the Bluetooth stack. 
Once a response is received each of those profiles, a flag is set to indicate 
to the application that the corresponding profile is enabled and ready to use.

**This application is structured as follows:** The source code exists in a 
`code` folder, all application-related include header files are in the 
`include` folder and the main() function `app.c` is located in the parent 
directory.

When this application starts, it creates a bondlist using the bonding 
information stored in `NVR2`.

Erasing `NVR2` will lead to an empty bondlist. If the desired behavior of this
application or test configuration is to start from a new bondlist at startup, 
the following steps can be undertaken:

1.  Connect DIO12 to ground.
2.  Press the RESET button (this restarts the application, which will
    pause at the start of its initialization routine).

Note that this will erase the flash, but only if there is anything in the 
`NVR2` drive.

**Note:**
If peer (central) device address type is not public during connection 
establishment, it is not added to the white list, since it needs to be 
resolved using IRK which is not implemented in this sample code.

Code
----
    app_init.c    - All initialization functions are called here, but the
                    implementation is in the respective C files
    app_process.c - Message handlers for application
    ble_bass.c    - Support functions and message handlers pertaining to 
                    Battery Service Server
    ble_diss.c    - Support functions and message handlers pertaining to 
                    Device Information Service Server
    ble_hogpd.c   - Support functions and message handlers pertaining to HID
                    Over GATT Profile Service Server
    ble_sec.c     - Support functions and message handlers pertaining to
                    Bluetooth low energy technology security. And support
                    functions for erasing and writing into the NVR2 section.
    ble_std.c     - Support functions and message handlers pertaining to
                    Bluetooth low energy technology

Include
-------
    app.h        - Overall application header file
    ble_bass.h   - Header file for Battery Service Server
    ble_diss.h   - Header file for Device Information Service Server
    ble_hogpd.h  - Header file for HID Over GATT Profile Service Server
    ble_sec.h    - Header file for Bluetooth low energy security
    ble_std.h    - Header file for Bluetooth low energy standard

Hardware Requirements
---------------------
This application can be executed on any RSL10 Evaluation and Development Board 
with no external connections required.

Importing a Project
-------------------
To import the sample code into your IDE workspace, refer to the 
*Getting Started Guide* for your IDE for more information.

Select the project configuration according to the required optimization level. 
Use **Debug** configuration for optimization level **None** or **Release**
configuration for optimization level **More** or **O2**.

Verification
------------
To verify that this application is functioning correctly, connect and pair the
RSL10 with a Bluetooth Low Energy compatible phone/computer. Open a text 
editor on the phone/computer and repeatedly press button SW1 connected to DIO5
on the RSL10. The phone/computer should output 
**RSL10 ON Semiconductor HID Keyboard Demo** on-screen. Also, use RSL10 or another third-party central 
device application to establish a connection and execute pairing and bonding functionality. In addition to establishing a connection, this application can 
be used to read/write characteristics and receive notifications.

To show how an application can send notifications, if bonded, for every 30 
timer expirations (6 seconds), a notification request flag is set and the 
application sends an incremental value of the first attribute to a peer 
device.

Alternatively, you can observe the behavior of the LED on the RSL10 Evaluation 
and Development Board (DIO6). The LED behavior is controlled by the 
`LED_Timer` function (`app_process.c`) and can be one of the following:

    - If the device has not started advertising: the LED is off.
    - If the device is advertising but it has not connected to any peer: the 
      LED blinks every 200 ms.
    - If the device is advertising and it is connecting to fewer than
      NUM_MASTERS peers: the LED blinks every 2 seconds according to the
      number of connected peers (i.e., blinks once if one peer is connected,
      twice if two peers are connected, etc.).
    - If the device is connecting to NUM_MASTERS peers (it stopped 
      advertising): the LED is steady on.
      
Notes
-----
Sometimes the firmware in RSL10 cannot be successfully re-flashed, due to the
application going into Sleep Mode or resetting continuously (either by design 
or due to programming error). To circumvent this scenario, a software recovery
mode using DIO12 can be implemented with the following steps:

1.  Connect DIO12 to ground.
2.  Press the RESET button (this restarts the application, which
    pauses at the start of its initialization routine).
3.  Re-flash RSL10. After successful re-flashing, disconnect DIO12 from
    ground, and press the RESET button so that the application can work
    properly.
    
**Note:** The use of this method will also reset the `NVR2` memory.
      
***
Copyright (c) 2017 Semiconductor Components Industries, LLC
(d/b/a ON Semiconductor).
