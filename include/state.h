/**
 ****************************************************************************************
 *
 * @file state.h
 *
 * @XYO Core library source code.
 *
 * @brief primary crypto routines for the XYO Core.
 *
 * Copyright (C) 2017 XY - The Findables Company. All Rights Reserved.
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */

#ifndef STATE_H
#include <stdint.h>
#include "xyobject.h"
#include "crypto.h"
#include "xyo.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct OriginChainState OriginChainState;

/*
 * STRUCTURES
 ****************************************************************************************
 */
 
struct OriginChainState {
  XYResult_t* (*newOriginBlock)(OriginChainState* self_OriginChainState, ByteArray_t* newHash);
  XYResult_t* (*addSigner)(OriginChainState* self_OriginChainState, Signer_t* newSigner);
  Signer_t* currentSigner;
  Signer_t* nextSigner;
  ByteArray_t* latestHash;
  uint32_t index;
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

XYResult_t* newOriginBlock(OriginChainState* self_OriginChainState, ByteArray_t* originBlockHash);
XYResult_t* addSigner(OriginChainState* self_OriginChainState, Signer_t* user_Signer);

#endif
