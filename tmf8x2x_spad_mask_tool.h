/*
 *****************************************************************************
 * Copyright by ams OSRAM AG                                                       *
 * All rights are reserved.                                                  *
 *                                                                           *
 * IMPORTANT - PLEASE READ CAREFULLY BEFORE COPYING, INSTALLING OR USING     *
 * THE SOFTWARE.                                                             *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT         *
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS         *
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  *
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,     *
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT          *
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     *
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY     *
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT       *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE     *
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.      *
 *****************************************************************************
 */
/*
 *      PROJECT:   tmf8x2x SPAD mask conversion/checking tool
 *      $Revision: $
 *      LANGUAGE:  C99
 *
 */

/*! \file tmf8x2x_spad_mask_tool.h
 *  \brief tool to check masks in a "human-readable" format, and convert to HAL format and print on stdout.
 */

/*
 *****************************************************************************
 * INCLUDES
 *****************************************************************************
 */

#include <stdint.h>
#include "tmf8x2x_includes.h"

/*
 *****************************************************************************
 * DEFINES
 *****************************************************************************
 */

#ifndef TMF8X2X_SPAD_MASK_TOOL_H
#define TMF8X2X_SPAD_MASK_TOOL_H

/* constants for SPAD map checking */
#define TMF8X2X_ZONE_INVESTIGATE            0
#define TMF8X2X_ZONE_UNVERIFIED             1
#define TMF8X2X_ZONE_VERIFIED               2
#define TMF8X2X_CHECK_SCRATCH_BUFFER_SIZE   1024

/*
 *****************************************************************************
 * FUNCTIONS
 *****************************************************************************
 */

/**
 * @brief tmf8x2xCreateMainSpad fills the given config structure with the content of the human readable mask
 * @param config SPAD configuration in machine readable format (packed)
 * @param mask SPAD configuration in human readable format
 * @return pointer to SPAD configuration in machine readable format (packed), or 0 after errors
 */
tmf8x2xHalMainSpadConfig * tmf8x2xCreateMainSpad( tmf8x2xHalMainSpadConfig * config, const tmf8x2xSpadMask * mask );

/**
 * @brief tmf8x2xCheckMainSpadAssignment checks SPAD map size and if in each used channel, there are at least two adjacent SPADs (can be in any direction).
 * @param config configuration in machine readable format (packed)
 * @return TMF8X2X_SPAD_MAP_ERROR_CONFIG if checks found an error, TMF8X2X_SPAD_MAP_OK otherwise
 */
uint8_t tmf8x2xCheckMainSpadAssignment( const tmf8x2xHalMainSpadConfig * config );

/**
 * @brief tmf8x2xCheckMainSpadArea checks if the SPAD map with applied X/Y offset fits in the SPAD area
 * @param config configuration in machine readable format (packed)
 * @return TMF8X2X_SPAD_MAP_ERROR_CONFIG if checks found an error, TMF8X2X_SPAD_MAP_OK otherwise
 */
uint8_t tmf8x2xCheckMainSpadArea ( const tmf8x2xHalMainSpadConfig * config );

/**
 * @brief tmf8x2xCheckMainSpadChannelSetup checks if channel 0 was used in the SPAD map and if electrical calibration will pass
 * @param spadMap in human readable format
 * @param xSize SPAD map size in x direction
 * @param ySize SPAD map size in y direction
 * @return TMF8X2X_SPAD_MAP_ERROR_CONFIG if checks found an error, TMF8X2X_SPAD_MAP_OK otherwise
 */
uint8_t tmf8x2xCheckMainSpadChannelSetup ( const uint8_t * spadMap, const uint8_t xSize, const uint8_t ySize );

/**
 * @brief dumpMainSpadConfigAsCstruct dumps a SPAD setup in C code for use in custom TMF882x firmware
 * @param name of the custom SPAD setup
 * @param config configuration in machine readable format (packed)
 */
void dumpMainSpadConfigAsCstruct( const char * name, const tmf8x2xHalMainSpadConfig * config );

/**
 * @brief dumpMainSpadConfigAsI2Cstrings dumps a SPAD setup as I2C strings for setup via TMF882x I2C registers
 * @param name of the custom SPAD setup
 * @param config configuration in machine readable format (packed)
 */
void dumpMainSpadConfigAsI2Cstrings( const char * name, const tmf8x2xHalMainSpadConfig * config );

/* show the channel map in a human readable format */
/**
 * @brief dumpChannelMapAsText dumps the SPAD to TDC channel mapping in human readable format
 * @param spadMask channel mapping in human readable format
 */
void dumpChannelMapAsText( const tmf8x2xSpadMask * spadMask );

/**
 * @brief dumpMainSpadEnableBitsAsText shows the SPAD enable bits in a human readable format
 * @param config SPAD configuration in machine readable format (packed)
 */
void dumpMainSpadEnableBitsAsText( const tmf8x2xHalMainSpadConfig * config );


/**
 * @brief dumpString writes a string to stdout
 * @param dumpTxt string to dump
 */
void dumpString(const char* dumpTxt);

/**
 * @brief dumpUnsignedHex writes an unsigned hex number to stdout
 * @param number to dump
 */
void dumpUnsignedHex(const uint32_t number);

/**
 * @brief dumpSignedDecimal writes a signed decimal number to stdout
 * @param number to dump
 */
void dumpSignedDecimal(const int32_t number);

#endif /* TMF8X2X_SPAD_MASK_TOOL_H */

