//
//  main_adapted.h
//  Razer-OSX-LED GUI
//
//  Created by Roberto Sanchez on 06/05/2020.
//  Copyright © 2020 AppCoda. All rights reserved.
//

#ifndef main_adapted_h
#define main_adapted_h

#import <IOKit/IOKitLib.h>
#import <IOKit/IOCFPlugIn.h>
#import <stdio.h>
#import <string.h>
#import <stdlib.h>

#include <stdio.h>

#import "razerkbd_driver.h"

char clamp_u8(unsigned char value, unsigned char min, unsigned char max);
const unsigned char* pollDevice(const char cmdInput[]);
IOUSBDeviceInterface** getRazerUSBDeviceInterface();
#endif /* main_adapted_h */
