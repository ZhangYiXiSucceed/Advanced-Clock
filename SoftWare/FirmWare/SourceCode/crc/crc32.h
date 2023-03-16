/**********************************************************************************************************/
/*                                                                                                        */
/*                                Copyright (c) 2021-2022 by Pizer.Fan                                    */
/*                                                                                                        */
/*  This software is copyrighted by and is the sole property of Pizer.Fan. All rights, title, ownership,  */
/*  or other interests in the software remain the property of Pizer.Fan. Any unauthorized use,            */
/*  duplication, transmission, distribution, or disclosure of this software is expressly forbidden.       */
/*  This Copyright notice may not be removed or modified without prior written consent of Pizer.Fan       */
/*                                                                                                        */
/**********************************************************************************************************/

/**********************************************************************************************************/
/*  Packet Tool CLI                                                                                       */
/*                                                                                                        */
/*    Arguments                                                                                           */
/*                                                                                                        */
/**********************************************************************************************************/

/**********************************************************************************************************/
/*  Release History                                                                                       */
/*                                                                                                        */
/*    Date              Name                      Description                                             */
/*                                                                                                        */
/*  2022/09/13      Pizer.Fan                   Initial Version                                           */
/*  2022/11/11      Guoqing.zhang               Modify Version                                            */
/*                                                                                                        */
/**********************************************************************************************************/

#ifndef _CRC_H_
#define _CRC_H_

#include <stdint.h>

//! @addtogroup crc32
//! @{

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! @brief State information for the CRC32 algorithm.
typedef struct Crc32Data
{
    uint32_t currentcrc;   //!< Current CRC value.
    uint32_t byteCountCrc; //!< Number of bytes processed.
} crc32_data_t;

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////

#if __cplusplus
extern "C" {
#endif

//! @name CRC32
//@{

//! @brief Initializes the parameters of the crc function, must be called first
//!
//! @param crc32Config Instantiation of the data structure of type crc32_data_t
//! @retval kStatus_Success
void crc32_init(crc32_data_t *crc32Config);

//! @brief A "running" crc calculator that updates the crc value after each call
//!
//! @param crc32Config Instantiation of the data structure of type crc32_data_t
//! @param src Pointer to the source buffer of data
//! @param lengthInBytes The length, given in bytes (not words or long-words)
//! @retval kStatus_Success
void crc32_update(crc32_data_t *crc32Config, const uint8_t *src, uint32_t lengthInBytes);

//! @brief Calculates the final crc value, padding with zeros if necessary, must be called last
//!
//! @param crc32Config Instantiation of the data structure of type crc32_data_t
//! @param hash Pointer to the value returned for the final calculated crc value
//! @retval kStatus_Success
void crc32_finalize(crc32_data_t *crc32Config, uint32_t *hash);

//@}

#if __cplusplus
}
#endif

//! @}

#endif
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
