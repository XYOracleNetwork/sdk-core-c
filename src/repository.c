/**
 ****************************************************************************************
 *
 * @file repository.c
 *
 * @XYO Core library source code.
 *
 * @brief primary crypto routines for the XYO Core.
 *
 * Copyright (C) 2018 XY - The Findables Company
 *
 ****************************************************************************************
 */

/*
 * INCLUDES
 ****************************************************************************************
 */
#include "repository.h"
#include "BoundWitness.h"

/*----------------------------------------------------------------------------*
*  NAME
*      write
*
*  DESCRIPTION
*      Write block to the Repository Provider, in this case a ram disk.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult*                              [out]      bool   Returns OK if success
*----------------------------------------------------------------------------*/
XYResult* write(RepositoryProvider* self, ByteArray* value, uint offset, uint timeout) {
  /* First we check if Storage Provider has been initialized */
  if( self->repository == NULL ){
    XYResult* lookup_result = lookup((char*)IntStrongArray_id);
    if(lookup_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }
    ObjectProvider* ISA_Creator = lookup_result->result;

    XYResult* create_result = ISA_Creator->create((char*)IntStrongArray_id, NULL);
    if(create_result->error != OK){ RETURN_ERROR(ERR_CRITICAL); }

    IntStrongArray* repo = create_result->result;
    free(lookup_result);
    lookup_result = lookup((char*)BoundWitness_id);
    if(lookup_result->error != OK){ RETURN_ERROR(ERR_CRITICAL);}
    char* boundWitnessBytes = value->payload;
    ObjectProvider* boundWitness_Creator = lookup_result->result;
    XYResult* fromBytes_result = boundWitness_Creator->fromBytes(boundWitnessBytes);
    if(fromBytes_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }

    BoundWitness* boundWitness_raw = fromBytes_result->result;
    XYResult* newObject_result = newObject((char*)BoundWitness_id, boundWitness_raw);
    if(newObject_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }
    self->logicalEnd = 1;
    return repo->add(repo, newObject_result->result);
  } else {
    for(int i = 0; i<=offset; i++){
      XYResult* lookup_result = lookup(&self->repository[i]);
      if(lookup_result->error == OK){
        continue;
      } else {
        lookup_result = lookup((char*)BoundWitness_id);

        if(lookup_result->error != OK){ RETURN_ERROR(ERR_CRITICAL); }

        char* boundWitnessBytes = value->payload;
        ObjectProvider* boundWitness_Creator = lookup_result->result;

        XYResult* fromBytes_result = boundWitness_Creator->fromBytes(boundWitnessBytes);
        if(fromBytes_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }

        BoundWitness* boundWitness_raw = fromBytes_result->result;

        XYResult* newObject_result = newObject((char*)BoundWitness_id, boundWitness_raw);
        if(newObject_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }
        self->logicalEnd = i+1;
        return ((IntStrongArray*)self->repository)->add(self->repository, newObject_result->result);
      }
    }
    RETURN_ERROR(ERR_CRITICAL);
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      read
*
*  DESCRIPTION
*      Append block to the Repository Provider, in this case a ram disk.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult*                              [out]      bool   Returns OK if success
*----------------------------------------------------------------------------*/
XYResult* read(RepositoryProvider* self, uint offset, uint timeout){
  return ((IntStrongArray*)self->repository)->get(self->repository, offset);
}

/*----------------------------------------------------------------------------*
*  NAME
*      append
*
*  DESCRIPTION
*      Append block to the Repository Provider, in this case a ram disk.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self_OriginChainNavigator*
*     *ByteArray                               [in]       originBlockHash*
*
*  RETURNS
*      XYResult*                              [out]      bool   Returns OK if success
*----------------------------------------------------------------------------*/
XYResult* append(OriginChainProvider* self, ByteArray* value, uint timeout){
  return self->repository->write(self->repository, value, self->repository->logicalEnd-1, timeout);
}

/*----------------------------------------------------------------------------*
*  NAME
*      getChain
*
*  DESCRIPTION
*      Get the location of the origin chain on disk and return the array object.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self*
*
*  RETURNS
*      XYResult*                              [out]      XYObject*   IntStrongArray
*----------------------------------------------------------------------------*/
XYResult* getChain(OriginChainProvider* self){
  XYResult* lookup_result = lookup((char*)IntStrongArray_id);
  if(lookup_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }
  ObjectProvider* ISA_Creator = lookup_result->result;

  XYResult* newObject_result = newObject((char*)IntStrongArray_id, self->repository->repository);
  if(newObject_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }

  XYResult* toBytes_result = ISA_Creator->toBytes(newObject_result->result);
  if(newObject_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }

  free(lookup_result);
  free(newObject_result);

  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result){
    return_result->error = OK;
    return_result->result = toBytes_result->result;
    free(toBytes_result);
    return return_result;
  } else {
    free(toBytes_result);
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}

/*----------------------------------------------------------------------------*
*  NAME
*      deleteChain
*
*  DESCRIPTION
*      Nullify and clear the origin chain.
*
*  PARAMETERS
*     *OriginChainNavigator                    [in]       self*
*
*  RETURNS
*      XYResult*                              [out]      XYObject*   IntStrongArray
*----------------------------------------------------------------------------*/
XYResult* deleteChain(OriginChainProvider* self){
  /* format old chain */

  free(((IntStrongArray*)self->repository->repository)->payload);
  free(((IntStrongArray*)self->repository->repository));

  /* Create new IntStrongArray */

  XYResult* lookup_result = lookup((char*)IntStrongArray_id);
  if(lookup_result->error != OK) { RETURN_ERROR(ERR_CRITICAL); }

  ObjectProvider* ISA_Creator = lookup_result->result;
  XYResult* create_result = ISA_Creator->create((char*)IntStrongArray_id, NULL);
  if(create_result->error != OK){ RETURN_ERROR(ERR_CRITICAL); }
  IntStrongArray* repo = create_result->result;
  self->repository->repository = repo;
  ((OriginChainProvider*)self->repository)->repository->logicalEnd = 0;

  free(lookup_result);
  free(create_result);

  XYResult* return_result = malloc(sizeof(XYResult));
  if(return_result){
    return_result->error = OK;
    return_result->result = 0;
    return return_result;
  } else {
    RETURN_ERROR(ERR_INSUFFICIENT_MEMORY);
  }
}
