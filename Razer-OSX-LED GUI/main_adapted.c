//
//  main_adapted.c
//  Razer-OSX-LED GUI
//
//  Created by Roberto Sanchez on 06/05/2020.
//  Copyright © 2020 Reven Sanchez. All rights reserved.
//

#include "main_adapted.h"

char ret[256] = {0};

IOUSBDeviceInterface** getRazerUSBDeviceInterface() {
    CFMutableDictionaryRef matchingDict;
    matchingDict = IOServiceMatching(kIOUSBDeviceClassName);
    if (matchingDict == NULL) {
        return NULL;
    }
    
    io_iterator_t iter;
    kern_return_t kReturn =
        IOServiceGetMatchingServices(kIOMasterPortDefault, matchingDict, &iter);
    if (kReturn != kIOReturnSuccess) {
        return NULL;
    }

    io_service_t usbDevice;
    while ((usbDevice = IOIteratorNext(iter))) {
        IOCFPlugInInterface **plugInInterface = NULL;
        SInt32 score;
        
        kReturn = IOCreatePlugInInterfaceForService(
            usbDevice, kIOUSBDeviceUserClientTypeID, kIOCFPlugInInterfaceID, &plugInInterface, &score);
        
        IOObjectRelease(usbDevice);  // Not needed after plugin created
        if ((kReturn != kIOReturnSuccess) || plugInInterface == NULL) {
            printf("Unable to create plugin (0x%08x)\n", kReturn);
            continue;
        }
        
        IOUSBDeviceInterface **dev = NULL;
        HRESULT hResult = (*plugInInterface)->QueryInterface(
            plugInInterface, CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID), (LPVOID *)&dev);
        
        (*plugInInterface)->Release(plugInInterface);  // Not needed after device interface created
        if (hResult || !dev) {
            printf("Couldn’t create a device interface (0x%08x)\n", (int) hResult);
            continue;
        }
        
        if (!is_blade_laptop(dev)) {
            (*dev)->Release(dev);    // Not recognized as a Razer Blade Laptop device
            continue;
        }
        
        kReturn = (*dev)->USBDeviceOpen(dev);
        if (kReturn != kIOReturnSuccess)  {
            printf("Unable to open USB device: %08x\n", kReturn);
            (*dev)->Release(dev);
            continue;
        }
        
        // Success. We found the Razer USB device.
        // Caller is responsible for closing USB and release device.
        IOObjectRelease(iter);
        return dev;
    }
    
    IOObjectRelease(iter);
    return NULL;
}

// MARK: - pollDevice function
const unsigned char* pollDevice(const char cmdInput[]  ) {
    int init_size = strlen(cmdInput);
    char *argv[8];
    argv[0] = "self";
    int argc = 1;
    char delim[] = " ";
    char *ptr = strtok(cmdInput, delim);

    while(ptr != NULL)
    {
        argv[argc++] = ptr;
        ptr = strtok(NULL, delim);
    }
    char *cmd = argv[1];
    

    IOUSBDeviceInterface **dev = getRazerUSBDeviceInterface();
    if (dev == NULL) {
        return "error";  // Assume appropriate error message displayed during the lookup
    }
    
    if (strcmp("info", cmd) == 0) {
        char buf[256] = {0};
        razer_attr_read_device_type(dev, buf);
        strcpy(ret,buf);
        //printf("%s", buf);
        razer_attr_read_get_firmware_version(dev, buf);
        strcat(ret, "Firmware ");
        strcat(ret,buf);
        //printf("Firmware %s", buf);

    } else if (strcmp("static", cmd) == 0) {
        char buf[3] = {0,0,0};
        if (argc - 2 == 0) {
            // default to green
            buf[1] = 0xff;
        } else if (argc - 2 == 3) {
            buf[0] = strtol(argv[2], NULL, 10);
            buf[1] = strtol(argv[3], NULL, 10);
            buf[2] = strtol(argv[4], NULL, 10);
        } else if (argc - 2 == 1) {
            if (strcmp("red", argv[2]) == 0) {
                buf[0] = 0xff;
            } else if (strcmp("green", argv[2]) == 0) {
                buf[1] = 0xff;
            } else if (strcmp("blue", argv[2]) == 0) {
                buf[2] = 0xff;
            } else if (strcmp("white", argv[2]) == 0) {
                buf[0] = buf[1] = buf[2] = 0xff;
            }
        } else {
            printf("-- Incorrect number of args for command: %s\n\n", cmd);
            return "error";
        }
        razer_attr_write_mode_static(dev, buf, 3);
    } else if (strcmp("breathe", cmd) == 0) {
        char buf[6] = {0,0,0,0,0,0};
        int count = 1;
        if (argc - 2 == 0) {
            count = 1;
        } else if (argc - 2 == 3) {
            count = 3;
            buf[0] = strtol(argv[2], NULL, 10);
            buf[1] = strtol(argv[3], NULL, 10);
            buf[2] = strtol(argv[4], NULL, 10);
        } else if (argc - 2 == 6) {
            count = 6;
            buf[0] = strtol(argv[2], NULL, 10);
            buf[1] = strtol(argv[3], NULL, 10);
            buf[2] = strtol(argv[4], NULL, 10);
            buf[3] = strtol(argv[5], NULL, 10);
            buf[4] = strtol(argv[6], NULL, 10);
            buf[5] = strtol(argv[7], NULL, 10);
        } else {
            printf("-- Incorrect number of args for command: %s\n\n", cmd);
            return "error";
        }
        razer_attr_write_mode_breath(dev, buf, count);
    } else if (strcmp("starlight", cmd) == 0) {
        char buf[4] = {1,0,0xff,0};  // Speed, R, G, B
        if (argc - 2 == 0) {
            // Defaults are fine
        } else if (argc - 2 == 1) {
            buf[0] = strtol(argv[2], NULL, 10);
        } else if (argc - 2 == 4) {
            buf[0] = strtol(argv[2], NULL, 10);
            buf[1] = strtol(argv[3], NULL, 10);
            buf[2] = strtol(argv[4], NULL, 10);
            buf[3] = strtol(argv[5], NULL, 10);
        } else {
            printf("-- Incorrect number of args for command: %s\n\n", cmd);
            return "error";
        }
        razer_attr_write_mode_starlight(dev, buf, 0);
    } else if (strcmp("reactive", cmd) == 0) {
        char buf[4] = {1,0,0xff,0};  // Speed, R, G, B
        if (argc - 2 == 0) {
            // Defaults are fine
        } else if (argc - 2 == 4) {
            buf[0] = strtol(argv[2], NULL, 10);
            buf[1] = strtol(argv[3], NULL, 10);
            buf[2] = strtol(argv[4], NULL, 10);
            buf[3] = strtol(argv[5], NULL, 10);
        } else {
            printf("-- Incorrect number of args for command: %s\n\n", cmd);
            return "error";
        }
        razer_attr_write_mode_reactive(dev, buf, 4);
    } else if (strcmp("spectrum", cmd) == 0) {
        razer_attr_write_mode_spectrum(dev, "1", 1);
    } else if (strcmp("wave", cmd) == 0) {
        char *buf = "1";
        if (argc - 2 == 0) {
            // default is fine
        } else if (argc - 2 == 1) {
            if (strcmp("left", argv[2]) == 0) {
                buf = "1";
            } else if (strcmp("right", argv[2]) == 0) {
                buf = "2";
            } else {
                printf("-- Unrecognized argument for command: %s\n\n", argv[2]);
                return "error";
            }
        } else {
            printf("-- Incorrect number of args for command: %s\n\n", cmd);
            return "error";
        }
        razer_attr_write_mode_wave(dev, buf, 0);
    } else if (strcmp("logo", cmd) == 0) {
        if (argc - 2 == 0) {
            // Retrieve the current logo on/off state
            char buf[4] = {0};
            razer_attr_read_set_logo(dev, buf, 0);
             printf("%s", strcmp("0\n", buf) == 0 ? "off" : "on");
        } else if (argc - 2 == 1) {
            char *buf = "1";
            if (strcmp("on", argv[2]) == 0) {
                buf = "1";
            } else if (strcmp("off", argv[2]) == 0) {
                buf = "0";
            } else {
                printf("-- Unrecognized argument for command: %s\n\n", argv[2]);
                return "error";
            }
            razer_attr_write_set_logo(dev, buf, 0);
        } else {
            printf("-- Incorrect number of args for command: %s\n\n", cmd);
            return "error";
        }
    } else if (strcmp("fnkey", cmd) == 0) {
        if (argc - 2 == 1) {
            char *buf = "1";
            if (strcmp("on", argv[2]) == 0) {
                buf = "1";
            } else if (strcmp("off", argv[2]) == 0) {
                buf = "0";
            } else {
                printf("-- Unrecognized argument for command: %s\n\n", argv[2]);
                return "error";
            }
            razer_attr_write_set_fn_toggle(dev, buf, 0);
        } else {
            printf("-- Incorrect number of args for command: %s\n\n", cmd);
            return "error";
        }
    } else if (strcmp("gamemode", cmd) == 0) {
        if (argc - 2 == 1) {
            char *buf = "1";
            if (strcmp("on", argv[2]) == 0) {
                buf = "1";
            } else if (strcmp("off", argv[2]) == 0) {
                buf = "0";
            } else {
                printf("-- Unrecognized argument for command: %s\n\n", argv[2]);
                return "error";
            }
            razer_attr_write_set_gm_toggle(dev, buf, 0);
        } else {
            printf("-- Incorrect number of args for command: %s\n\n", cmd);
            return "error";
        }

    } else if (strcmp("brightness", cmd) == 0) {
        char buf[4] = {0};
        razer_attr_read_set_brightness(dev, buf);
        if (argc - 2 == 0) {
            // Display the current brightness
             strcpy(ret,buf);
        } else if (argc - 2 == 1) {
            // Changing the brightness
            if (strcmp(argv[2], "up") == 0 || strcmp(argv[2], "down") == 0) {
                bool up = strcmp(argv[2], "up") == 0;
                int val = strtol(buf, NULL, 10) + (up ? 16 : -16);
                if (val < 0) val = 0;
                if (val > 255) val = 255;
                sprintf(buf, "%d", val);
                razer_attr_write_set_brightness(dev, buf, 0);
            } else {
                razer_attr_write_set_brightness(dev, argv[2], 0);
            }
        } else {
            printf("-- Incorrect number of args for command: %s\n\n", cmd);
            return "error";
        }
    } else {
        printf("-- Unrecognized command: %s\n\n", cmd);
    }

    (*dev)->USBDeviceClose(dev);
    (*dev)->Release(dev);
    
    return ret;
}
