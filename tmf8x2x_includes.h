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
 *      PROJECT:   tmf8x2x
 *      $Revision: $
 *      LANGUAGE:  C99
 *
 */

/*! \file tmf8x2x_includes.h
*/

#ifndef TMF8X2X_INCLUDES_H
#define TMF8X2X_INCLUDES_H

/*
 *****************************************************************************
 * INCLUDES
 *****************************************************************************
 */

#include <stdint.h>

/*
 *****************************************************************************
 * DEFINES
 *****************************************************************************
 */

/* number of TDC channels */
#define TMF8X2X_NUMBER_OF_CHANNELS 10

/* TDC channel numbers */
/* channel 0 and channel 1 not needed for checks according to the TMF882x data sheet */
#define CHANNEL_2 2
#define CHANNEL_3 3
#define CHANNEL_4 4
#define CHANNEL_5 5
#define CHANNEL_6 6
#define CHANNEL_7 7
#define CHANNEL_8 8
#define CHANNEL_9 9

/* number of SPADs in the SPAD array in X direction */
#define TMF8X2X_MAIN_SPAD_COLUMNS             34
/* number of SPADs in the SPAD array in Y direction */
#define TMF8X2X_MAIN_SPAD_ROWS                18

/* SPAD enable masks */
#define TDCIF_SPAD_ENABLE0__spad_enable_16_0__MASK   0x1ffff
#define TDCIF_SPAD_ENABLE0__spad_enable_16_0__WIDTH  17
#define TDCIF_SPAD_ENABLE1__spad_enable_33_17__MASK  0x1ffff

/* register adresses in the TMF882x I2C interface for custom SPAD maps / masks */
#define TMF8X2X_COM_SPAD_ENABLE_SPAD0_0       0x24
#define TMF8X2X_COM_SPAD_TDC_CHANNEL0_0       0x42
#define TMF8X2X_COM_SPAD_TDC_CHANNEL_SELECT_0 0x8a
#define TMF8X2X_COM_SPAD_X_OFFSET_2           0x8d
#define TMF8X2X_COM_SPAD_Y_OFFSET_2           0x8e
#define TMF8X2X_COM_SPAD_X_SIZE               0x8f
#define TMF8X2X_COM_SPAD_Y_SIZE               0x90

/* SPAD array center for even sized (x/y) SPAD maps, Q1 format */
#define X_CENTER_2_A 34
#define Y_CENTER_2_A 18
/* SPAD array center for odd sized (x/y) SPAD maps, Q1 format */
#define X_CENTER_2_B 33
#define Y_CENTER_2_B 17

/* size (x/y) of the custom SPAD map */
#define TMF8X2X_MAIN_SPAD_MAX_X_SIZE          18 /* maximum amount of SPADs selected in one row */
#define TMF8X2X_MAIN_SPAD_MAX_Y_SIZE          10 /* maximum amount of SPADs selected in one column */

/* size (x/y) of the SPAD area where screamers (defective SPADs) can be masked */
#define TMF8X2X_MAIN_SPAD_SCREAMER_MAX_X_SIZE 18
#define TMF8X2X_MAIN_SPAD_SCREAMER_MAX_Y_SIZE 12 /* the mask can be shifted for macro-mode in y-direction, therefore: +2 */

/* constants for SPAD map checking (function return values) */
#define TMF8X2X_SPAD_MAP_OK                 0
#define TMF8X2X_SPAD_MAP_ERROR_CONFIG       1

/*
 *****************************************************************************
 * MACROS
 *****************************************************************************
 */

/* necessary #defines for SPAD channel selection encoding in a 32-bit word */
#define TMF8X2X_MAIN_SPAD_VERTICAL_LSB_SHIFT ( 0 )
#define TMF8X2X_MAIN_SPAD_VERTICAL_MID_SHIFT ( 10 )
#define TMF8X2X_MAIN_SPAD_VERTICAL_MSB_SHIFT ( 20 )

/* to encode a single channel, we need to set 3 bits at specific positions in the 32-bit word */
#define TMF8X2X_MAIN_SPAD_ENCODE_CHANNEL( channel, yPosition )                                           \
( ( ( ( (channel) & 1 /*LSB*/ )      ) << ( (TMF8X2X_MAIN_SPAD_VERTICAL_LSB_SHIFT) + (yPosition) ) )     \
    | ( ( ( (channel) & 2 /*Mid*/ ) >> 1 ) << ( (TMF8X2X_MAIN_SPAD_VERTICAL_MID_SHIFT) + (yPosition) ) ) \
    | ( ( ( (channel) & 4 /*MSB*/ ) >> 2 ) << ( (TMF8X2X_MAIN_SPAD_VERTICAL_MSB_SHIFT) + (yPosition) ) ) \
    )

/* to decode a single channel, we need to get 3 bits at specific positions in the 32-bit word */
#define TMF8X2X_MAIN_SPAD_DECODE_CHANNEL( config, yPosition )                                           \
( ( ( ( (config) >> ( (TMF8X2X_MAIN_SPAD_VERTICAL_LSB_SHIFT) + (yPosition) ) )      ) & 1 /*LSB*/ )     \
    | ( ( ( (config) >> ( (TMF8X2X_MAIN_SPAD_VERTICAL_MID_SHIFT) + (yPosition) ) ) << 1 ) & 2 /*Mid*/ ) \
    | ( ( ( (config) >> ( (TMF8X2X_MAIN_SPAD_VERTICAL_MSB_SHIFT) + (yPosition) ) ) << 2 ) & 4 /*MSB*/ ) \
)

/* check if a SPAD is enabled in a custom SPAD mask */
#define TMF8X2X_APP_IS_MAIN_SPAD_ENABLED( cfg, x, y )    ( (cfg)->enableSpad[ (y) ] & ( 1 << (x) ) )

/*
 *****************************************************************************
 * STRUCTURES
 *****************************************************************************
 */

/* SPAD selection structure, assign each SPAD in the FOV to one channel */
/* this FOV center is typically read from the fuses. If there is no center in the fuses, you can configure it through a request,
 * otherwise it defaults to the absolute center: in this case it is: xCenter_2 = (34/2)*2 - 1 = 33, yCenter_2 = (18/2)*2 - 1= 17
 *
 * xLowerLeftCorner  = (xCenter_2 + xOffset_2 - xSize ) / 2
 * yLowerLeftCorner  = (yCenter_2 + yOffset_2 - ySize ) / 2
 * xUpperRightCorner = (xLowerLeftCorner + xSize ) -1
 * yUpperRightCorner = (yLowerLeftCorner + ySize ) -1
 *
 */
typedef struct _tmf8x2xHalMainSpadFovCenter
{
    uint8_t xCenter_2;    /*!< x-value of the center of FOV multiplied by 2 to accomodate for a center that is not on top of 1 spad - so we know in which direction to favor for even x-sized masks */
    uint8_t yCenter_2;    /*!< y-value of the center of FOV muliplied by 2 to accomodate for a center that is not on top of 1 spad - so we know in which direction to favor even y-sized masks */
} tmf8x2xHalMainSpadFovCenter;

/* this structure holds all information for a custom SPAD map in a machine readable format */
/* this data is sent to the TMF882x via I2C registers */
typedef struct _tmf8x2xHalMainSpadConfig
{

    uint32_t enableSpad[ TMF8X2X_MAIN_SPAD_MAX_Y_SIZE ]; /* have each bit set for a spad in the FOV that you want to enable */
    uint32_t tdcChannel[ TMF8X2X_MAIN_SPAD_MAX_X_SIZE ]; /* encoded in one 32-bit word are 3x10bits that encode one column of the selected SPAD */
    uint32_t tdcChannelSelect;  /* each row can have selected tdc channels 0/1 or tdc channels 8/9 */
    int8_t xOffset_2;    /* center offset in Q1 to FOV center (range is -67..+67) == -(2*34-1) .. + (2*34-1) */
    int8_t yOffset_2;    /* center offset in Q1 to FOV center (range is -35..+35) == -(2*18-1) .. + (2*18-1) */
    uint8_t xSize;       /* width in x-dimension (range is 1..18) */
    uint8_t ySize;       /* width in y-dimension (range is 1..10) */
} tmf8x2xHalMainSpadConfig;

/* this structure helps define a custom SPAD map in a human readable format */
typedef struct _tmf8x2xSpadMask
{
    const uint32_t * enable;
    const uint8_t  * channels;
    uint8_t id;
    int8_t xOffset_2;
    int8_t yOffset_2;
    uint8_t xSize;
    uint8_t ySize;
} tmf8x2xSpadMask;

#endif /* TMF8X2X_INCLUDES_H */

