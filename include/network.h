/**
 ****************************************************************************************
 *
 * @file network.h
 *
 * @XY4 project source code.
 *
 * @brief primary network routines for the XY4 firmware.
 *
 * Copyright (C) 2017 XY - The Findables Company
 *
 * This computer program includes Confidential, Proprietary Information of XY. 
 * All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef NETWORK_H
#define NETWORK_H

/*
 * DEFINES
 ****************************************************************************************
 */

#include <stdint.h>

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct proactiveNetworkProvider proactiveNetworkProvider_t;
typedef struct reactiveNetworkProvider reactiveNetworkProvider_t;

/*
 * STRUCTURES
 ****************************************************************************************
 */

struct proactiveNetworkProvider{
  //XYResult_t* (*find)(int flags);
  
  // temporary placeholders
  uint32_t requestConnection;
  uint32_t sendData;
  uint32_t disconnect;
};

struct reactiveNetworkProvider{
  //XYResult_t* (*find)(int flags);
  
  // temporary placeholders
  uint32_t listen;
  uint32_t disconnect;
};

proactiveNetworkProvider_t* newProactiveNetworkProvider(void);
reactiveNetworkProvider_t* newReactiveNetworkProvider(void);

#endif

// end of file network.h

