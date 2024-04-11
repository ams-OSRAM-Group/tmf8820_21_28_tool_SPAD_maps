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
 *      PROJECT:   tmf8x2x SPAD sample selection masks
 *      $Revision: $
 *      LANGUAGE:  C99
 *
 */

/*! \file tmf8x2x_spad_mask_tool.c
 *  \brief constant masks to select zones and SPAD
*/

/*
 *****************************************************************************
 * INCLUDES
 *****************************************************************************
 */

#include <stdio.h>
#include <math.h>
#include "tmf8x2x_includes.h"
#include "tmf8x2x_spad_mask_tool.h"

/*
 *****************************************************************************
 * LOCAL PROTOTYPES
 *****************************************************************************
 */
/**
 * @brief i2c8 converts an 8 bit number into a two character hex number representation
 * @param value to be converted
 */
static void i2c8( uint8_t value);
/**
 * @brief i2c24 converts an 24 bit number into a six character hex number representation
 * @param value to be converted
 */
static void i2c24( uint32_t value );
/**
 * @brief i2c24 converts an 24 bit number into an eight character hex number representation
 * @param value to be converted
 */
static void i2c32( uint32_t value );

/**
 * @brief mainSpadLlc computes the lower left corner (x or y, one function for both axes) of a SPAD map in the SPAD array
 * @param center_2 center in x/y direction in Q1 format
 * @param offset_2 offset from the center in x/y direction in Q1 format
 * @param size of the SPAD array in x/y direction
 * @return x/y position in the SPAD array
 */
static int mainSpadLlc( uint8_t center_2, int8_t offset_2, uint8_t size );
/**
 * @brief mainSpadUrc computes the upper right corner (x or y, one function for both axes) of a SPAD map in the SPAD array
 * @param llc lower left corner in x/y direction in the SPAD array *
 * @param size of the SPAD array in x/y direction
 * @return x/y position in the SPAD array
 */
static int mainSpadUrc( uint8_t llc, uint8_t size );

/*
 *****************************************************************************
 * UTILITY FUNCTIONS
 *****************************************************************************
 */

static int mainSpadLlc ( uint8_t center_2, int8_t offset_2, uint8_t size )
{
    int llc = center_2;
    llc = ( llc + offset_2 - size ) / 2;
    return llc;
}

static int mainSpadUrc ( uint8_t llc, uint8_t size )
{
    return (llc + size - 1);
}

/*
 *****************************************************************************
 * SPAD MAP CREATION
 *****************************************************************************
 */

tmf8x2xHalMainSpadConfig * tmf8x2xCreateMainSpad ( tmf8x2xHalMainSpadConfig * config, const tmf8x2xSpadMask * mask )
{
    int32_t x;
    int32_t y;
    int llcX;
    int llcY;
    int urcX;
    int urcY;
    uint16_t lineSelect = 0; /* default all have channel 0/1 selected */
    if (  ! config
        || ! mask
        || ( mask->xSize > TMF8X2X_MAIN_SPAD_MAX_X_SIZE )
        || ( mask->ySize > TMF8X2X_MAIN_SPAD_MAX_Y_SIZE )
        )
    {
        return 0;
    }

    llcX = mainSpadLlc( X_CENTER_2_A, mask->xOffset_2, mask->xSize );
    llcY = mainSpadLlc( Y_CENTER_2_A, mask->yOffset_2, mask->ySize );
    urcX = mainSpadUrc( llcX, mask->xSize );
    urcY = mainSpadUrc( llcY, mask->ySize );

    if (  ( ( urcX - llcX ) >= TMF8X2X_MAIN_SPAD_SCREAMER_MAX_X_SIZE )
        || ( ( urcY - llcY ) >= TMF8X2X_MAIN_SPAD_SCREAMER_MAX_Y_SIZE )
        )
    {
        return 0;
    }

    llcX = mainSpadLlc( X_CENTER_2_B, mask->xOffset_2, mask->xSize );
    llcY = mainSpadLlc( Y_CENTER_2_B, mask->yOffset_2, mask->ySize );
    urcX = mainSpadUrc( llcX, mask->xSize );
    urcY = mainSpadUrc( llcY, mask->ySize );

    if (  ( ( urcX - llcX ) >= TMF8X2X_MAIN_SPAD_SCREAMER_MAX_X_SIZE )
        || ( ( urcY - llcY ) >= TMF8X2X_MAIN_SPAD_SCREAMER_MAX_Y_SIZE )
        )
    {
        return 0;
    }
    /* clear all first */
    for ( y = 0; y < TMF8X2X_MAIN_SPAD_MAX_Y_SIZE; y++ )
    {
        config->enableSpad[ y ] = 0;
    }
    for ( y = 0; y < mask->ySize; y++ ) /* copy enable SPAD */
    {
        config->enableSpad[ y ] = mask->enable[ mask->ySize - 1 - y ];  /* reverse as the mask->enable is order-reversed */
    }
    /* check for correct channel selection (no 0/1 in same row as 8/9) */
    for ( y = 0; y < mask->ySize; y++ )
    {
        int is89 = 0; /* so far we do not know */
        int is89Set = 0; /* so far we have neither 0 nor 1 selected */
        const uint8_t * row = mask->channels + ( ( mask->ySize - 1 - y ) * mask->xSize ) ; /* step through linear array row wise. So as start-point for inner loop we have to go through lines */
        for ( x = 0; x < mask->xSize; x++ )
        {
            if ( row[ x ] == 0 || row[ x ] == 1 )
            {
                if ( !is89Set ) /* never set for this row - so set it now that we have 0/1 and may not have 8/9 */
                {
                    is89Set = 1;
                    is89 = 0;
                }
                else if ( is89 )
                {
                    return 0;
                }
            }
            else if ( row[ x ] == 8 || row[ x ] == 9 )
            {
                if ( !is89Set ) /* never set for this row - so set it now that we have 8/9 and may not have 0/1 */
                {
                    is89Set = 1;
                    is89 = 1;
                    lineSelect |= ( 1 << y ); /* this row has channels 8/9 selected */
                }
                else if ( ! is89 )
                {
                    return 0;
                }
            }
        }
    }
    /* clear all first */
    for ( x = 0; x < TMF8X2X_MAIN_SPAD_MAX_X_SIZE; x++ )
    {
        config->tdcChannel[ x ]  = 0;
    }

    /* now we can go on to pack the bits per column */
    for ( x = 0; x < mask->xSize; x++ )
    {
        uint32_t encode = 0; /* default is tdc channel 0 for all */
        uint8_t yIdx; /* now we go from bottom up in table, we need to invert the accessing index for y here. If ySize is 0, we won't enter below loop anyhow. */
        for ( y = 0, yIdx = mask->ySize - 1; y < mask->ySize; y++, yIdx-- )
        {
            uint8_t ch = mask->channels[ x + ( yIdx * mask->xSize ) ];    /* in linear array we need to start from bottom */
            encode |= TMF8X2X_MAIN_SPAD_ENCODE_CHANNEL( ch, y );/* an 8 is masked out by macro to a 0, and a 9 is masked out by macro to a 1 */
        }
        config->tdcChannel[ x ] = encode;
    }

    /* finally set the boundaries correctly in config record, and the row channel line select bits */
    config->xOffset_2 = mask->xOffset_2;
    config->yOffset_2 = mask->yOffset_2;
    config->xSize = mask->xSize;
    config->ySize = mask->ySize;
    config->tdcChannelSelect = lineSelect;

    return config;
}

/*
 *****************************************************************************
 * OUTPUT FUNCTIONS
 *****************************************************************************
 */

static void i2c8 ( uint8_t value)
{
    fprintf(stdout,"%02x ",value);
}

static void i2c24 ( uint32_t value )
{
    fprintf(stdout,"%02x %02x %02x ", value & UINT8_MAX, (value >> 8) & UINT8_MAX, (value >> 16) & UINT8_MAX );
}

static void i2c32 ( uint32_t value )
{
    fprintf(stdout,"%02x %02x %02x %02x ", value & UINT8_MAX, (value >> 8) & UINT8_MAX, (value >> 16) & UINT8_MAX, (value >> 24) & UINT8_MAX );
}

void dumpString (const char* dumpTxt)
{
    fprintf(stdout,"%s",dumpTxt);
}

void dumpUnsignedHex (const uint32_t number)
{
    fprintf(stdout,"%x",number);
}

void dumpSignedDecimal (const int32_t number)
{
    fprintf(stdout,"%d",number);
}

void dumpChannelMapAsText ( const tmf8x2xSpadMask * mask )
{
    dumpString("/* SPAD Map Assignment between Zones and TDCs */\n");
    dumpString( "xOffset_2=" );
    dumpSignedDecimal( mask->xOffset_2 );
    dumpString( " yOffset_2=" );
    dumpSignedDecimal( mask->yOffset_2 );
    dumpString( " xSize=" );
    dumpSignedDecimal( mask->xSize );
    dumpString( " ySize=" );
    dumpSignedDecimal( mask->ySize );
    dumpString( "\n" );
    dumpString("/* x =     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7  */\n\n");
    for ( int32_t y = 0; y < mask->ySize; y++ )
    {
        int32_t idx = y * mask->xSize; /* point at the beginning of the corresponding row */
        dumpString( "/* y=" );
        dumpSignedDecimal( mask->ySize - 1 - y );
        dumpString( " */  " );
        for ( int32_t x = 0; x < mask->xSize; x++ )
        {
            dumpSignedDecimal( mask->channels[ idx + x ] );
            dumpString( " " );
        }
        dumpString( "\n" );
    }
}

static void dumpEnabledBitsLineHead( uint32_t* line )
{
    fprintf(stdout,"/* y=%2u */ ", *line);
    --(*line);
}

static void dumpEnableBits( uint32_t bits )
{
    uint32_t currentBitNr = 0;

    while ( currentBitNr < TMF8X2X_MAIN_SPAD_SCREAMER_MAX_X_SIZE )
    {
        dumpString( bits & ( 1 << currentBitNr ) ? "1 " : "0 " );
        ++currentBitNr;
    }
}

void dumpMainSpadEnableBitsAsText ( const tmf8x2xHalMainSpadConfig * config )
{
    uint32_t emptyRowsBot = ( TMF8X2X_MAIN_SPAD_SCREAMER_MAX_Y_SIZE - config->ySize + config->yOffset_2 ) >> 1;
    uint32_t emptyRowsTop = TMF8X2X_MAIN_SPAD_SCREAMER_MAX_Y_SIZE - config->ySize - emptyRowsBot;
    uint32_t enabledRows  = config->ySize;

    dumpString("/* SPAD Enable Mask Visualization */\n");
    dumpString("/* x =     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7  */\n\n");

    uint32_t currentRow = TMF8X2X_MAIN_SPAD_SCREAMER_MAX_Y_SIZE - 1;

    while ( emptyRowsTop > 0 )
    {
        dumpEnabledBitsLineHead( &currentRow );
        dumpEnableBits( 0 );
        dumpString("\n");
        --emptyRowsTop;
    }

    while ( enabledRows > 0 )
    {
        dumpEnabledBitsLineHead(&currentRow);

        uint32_t currentEnableBits = config->enableSpad[ enabledRows - 1 ];

        /* masks with odd number of rows start with a shift of half a SPAD */
        uint32_t currentShift = ( TMF8X2X_MAIN_SPAD_SCREAMER_MAX_X_SIZE - config->xSize + config->xOffset_2 ) >> 1;
        currentEnableBits <<= currentShift;

        dumpEnableBits( currentEnableBits );
        dumpString("\n");
        --enabledRows;
    }

    while ( emptyRowsBot > 0 )
    {
        dumpEnabledBitsLineHead( &currentRow );
        dumpEnableBits( 0 );
        dumpString("\n");
        --emptyRowsBot;
    }
}

void dumpMainSpadConfigAsCstruct ( const char * name, const tmf8x2xHalMainSpadConfig * config )
{
    int32_t i;
    dumpString( "/* use this format for custom SPAD maps in the TMF882X firmware */");
    dumpString( "\nconst tmf8x2xHalMainSpadConfig ");
    dumpString( name );
    dumpString( " = \n{ /*enableSpad[ ]*/ 0x" );
    dumpUnsignedHex( config->enableSpad[ 0 ] );
    for ( i = 1; i < TMF8X2X_MAIN_SPAD_MAX_Y_SIZE ; i++ )
    {
        dumpString( ", 0x" );
        dumpUnsignedHex( config->enableSpad[ i ] );
    }
    dumpString( "\n, /*tdcChannel[ ]*/ 0x" );
    dumpUnsignedHex( config->tdcChannel[ 0 ] );
    for ( i = 1; i < TMF8X2X_MAIN_SPAD_MAX_X_SIZE ; i++ )
    {
        dumpString( ", 0x" );
        dumpUnsignedHex( config->tdcChannel[ i ] );
    }
    dumpString( "\n, /*tdcChannelSelect*/ 0x" );
    dumpUnsignedHex( config->tdcChannelSelect );
    dumpString( "\n, /*xOffset_2*/ " );
    dumpSignedDecimal( config->xOffset_2 );
    dumpString( "\n, /*yOffset_2*/ " );
    dumpSignedDecimal( config->yOffset_2 );
    dumpString( "\n, /*xSize*/ " );
    dumpSignedDecimal( config->xSize );
    dumpString( "\n, /*ySize*/ " );
    dumpSignedDecimal( config->ySize );
    dumpString( "\n};\n\n" );
}

void dumpMainSpadConfigAsI2Cstrings ( const char * name, const tmf8x2xHalMainSpadConfig * config )
{
    dumpString( "# use this format to set up custom SPAD maps via I2C transfers");
    dumpString( "\n# tmf8x2xHalMainSpadConfig ");
    dumpString( name );
    dumpString( "\n# enableSpad[ ]\nS 41 W ");
    i2c8( TMF8X2X_COM_SPAD_ENABLE_SPAD0_0 );
    for ( uint32_t i = 0; i < TMF8X2X_MAIN_SPAD_MAX_Y_SIZE; i++)
    {
        i2c24( config->enableSpad[ i ] );
    }
    dumpString("P\n# tdcChannel[ ]\nS 41 W ");
    i2c8(TMF8X2X_COM_SPAD_TDC_CHANNEL0_0);
    for ( uint32_t i = 0; i < TMF8X2X_MAIN_SPAD_MAX_X_SIZE; i++ )
    {
        i2c32(config->tdcChannel[ i ] );
    }
    dumpString("P\n# tdcChannelSelect\nS 41 W ");
    i2c8(TMF8X2X_COM_SPAD_TDC_CHANNEL_SELECT_0);
    i2c24(config->tdcChannelSelect);
    dumpString("P\n# xOffset_2\nS 41 W ");
    i2c8(TMF8X2X_COM_SPAD_X_OFFSET_2);
    i2c8(config->xOffset_2);
    dumpString("P\n# yOffset_2\nS 41 W ");
    i2c8(TMF8X2X_COM_SPAD_Y_OFFSET_2);
    i2c8(config->yOffset_2);
    dumpString("P\n# xSize\nS 41 W ");
    i2c8(TMF8X2X_COM_SPAD_X_SIZE);
    i2c8(config->xSize);
    dumpString("P\n# ySize\nS 41 W ");
    i2c8(TMF8X2X_COM_SPAD_Y_SIZE);
    i2c8(config->ySize);
    dumpString("P\n");
}

/*
 *****************************************************************************
 * SPAD MAP CHECKS
 *****************************************************************************
 */

uint8_t tmf8x2xCheckMainSpadAssignment ( const tmf8x2xHalMainSpadConfig * config )
{
    int32_t x;
    int32_t y;
    uint8_t z;
    int32_t rowIdx;

    uint8_t scratchBuffer[TMF8X2X_CHECK_SCRATCH_BUFFER_SIZE];
    uint8_t * scratch = scratchBuffer;
    uint8_t * zones = scratch;

    scratch += TMF8X2X_NUMBER_OF_CHANNELS;

    if (  ! config
        || ( config->xSize > TMF8X2X_MAIN_SPAD_MAX_X_SIZE )
        || ( config->ySize > TMF8X2X_MAIN_SPAD_MAX_Y_SIZE )
        || ( config->xSize < 1 )
        || ( config->ySize < 1 )
        || ( config->xSize == 1 && config->ySize == 1 )  /* single SPAD are not allowed */
        )
    {
        return TMF8X2X_SPAD_MAP_ERROR_CONFIG; /* out of bound error */
    }

    for ( z = 0; z < TMF8X2X_NUMBER_OF_CHANNELS; z++ )
    {
        zones[ z ] = TMF8X2X_ZONE_UNVERIFIED;
    }
    for ( x = 0; x < (TMF8X2X_MAIN_SPAD_MAX_X_SIZE * TMF8X2X_MAIN_SPAD_MAX_Y_SIZE); x++ )
    {
        scratch[ x ] = TMF8X2X_NUMBER_OF_CHANNELS;  /* initialise everything for an illegal channel */
    }
    for ( y = 0; y < config->ySize; y++ )
    {
        int8_t is89 = !!( config->tdcChannelSelect & (1 << y ) ); /* if it is the alternate channels, we need to add 8 -> get 0/1 to 8/9 */
        rowIdx = config->xSize * y;
        for ( x = 0; x < config->xSize; x++ )
        {
            uint32_t conf = config->tdcChannel[ x ];
            uint8_t ch = TMF8X2X_MAIN_SPAD_DECODE_CHANNEL( conf, y );
            if ( is89 && ( ch == 0 || ch == 1 ) )
            {
                ch += 8;
            }
            scratch[ rowIdx + x ] = ch;
        }
    }
    /* now the matrix is filled with channels, and the range is xSize * ySize */
    for ( y = 0; y < config->ySize; y++ )
    {
        rowIdx = config->xSize * y; /* point at the beginning of the corresponding row */
        for ( x = 0; x < config->xSize; x++ )
        {
            uint8_t ch = scratch[ rowIdx + x ];
            if (  ( ch < TMF8X2X_NUMBER_OF_CHANNELS )
                && ( zones[ ch ] != TMF8X2X_ZONE_VERIFIED )  /* still need to check for 2 adjacent SPAD in this zone */
                && ( TMF8X2X_APP_IS_MAIN_SPAD_ENABLED( config, x, y ) )
                )
            {                /* this spad is enabled, see if (x+1|y) or (x|y+1) or (x+1|y+1) or (x-1|y+1) are from the same zone and enabled too */
                if (  ( ( x + 1 ) < config->xSize )                    /* this is to handle also the corner cases where a mask is only xSize = 1 */
                    && ( scratch[ rowIdx + x + 1 ] == ch )
                    && TMF8X2X_APP_IS_MAIN_SPAD_ENABLED( config, x+1, y )
                    )
                {
                    zones[ ch ] = TMF8X2X_ZONE_VERIFIED; /* found two adjacent SPAD */
                }
                else if (  ( ( y + 1 ) < config->ySize )                /* this is to handle also the corner cases where a mask is only ySize = 1 */
                    && ( scratch[ rowIdx + ( config->xSize ) + x ] == ch )
                    && TMF8X2X_APP_IS_MAIN_SPAD_ENABLED( config, x, y+1 )
                    )
                {
                    zones[ ch ] = TMF8X2X_ZONE_VERIFIED; /* found two adjacent SPAD */
                }
                else if (   ( ( x + 1 ) < config->xSize )                    /* this is to handle also the corner cases where a mask is only xSize = 1 */
                    &&  ( ( y + 1 ) < config->ySize )                    /* this is to handle also the corner cases where a amsk is only ySize = 1 */
                    &&  ( scratch[ rowIdx + ( config->xSize ) + x + 1 ] == ch )
                    && TMF8X2X_APP_IS_MAIN_SPAD_ENABLED( config, x+1, y+1 )
                    )
                {
                    zones[ ch ] = TMF8X2X_ZONE_VERIFIED; /* found two adjacent SPAD */
                }
                else if (     ( x > 0 )                                      /* this is to handle a corner case where only diagonal SPAD are enabled */
                    &&  ( ( y + 1 ) < config->ySize )                    /* this is to handle also the corner cases where a amsk is only ySize = 1 */
                    &&  ( scratch[ rowIdx + ( config->xSize ) + x - 1 ] == ch )
                    && TMF8X2X_APP_IS_MAIN_SPAD_ENABLED( config, x-1, y+1 )
                    )
                {
                    zones[ ch ] = TMF8X2X_ZONE_VERIFIED; /* found two adjacent SPAD */
                }
                else
                {
                    zones[ ch ] = TMF8X2X_ZONE_INVESTIGATE; /* zone is in use, but so far no two adjacent, enabled SPAD found*/
                }
            }
            /* else ignore, as either this zone has already 2 adjacent cells, or is not used, or spad is not enabled */
        }
    }

    for ( z = 0; z < TMF8X2X_NUMBER_OF_CHANNELS; z++ )
    {
        if ( zones[ z ] == TMF8X2X_ZONE_INVESTIGATE )
        {
            return TMF8X2X_SPAD_MAP_ERROR_CONFIG;
        }
    }

    return TMF8X2X_SPAD_MAP_OK;
}

uint8_t tmf8x2xCheckMainSpadArea ( const tmf8x2xHalMainSpadConfig * config )
{
    /* center for even X / Y SPAD map size */
    int8_t llcX = mainSpadLlc( TMF8X2X_MAIN_SPAD_SCREAMER_MAX_X_SIZE, config->xOffset_2, config->xSize );
    int8_t llcY = mainSpadLlc( TMF8X2X_MAIN_SPAD_SCREAMER_MAX_Y_SIZE, config->yOffset_2, config->ySize );

    if (  llcX < 0 )
    {
        return TMF8X2X_SPAD_MAP_ERROR_CONFIG;
    }

    if (  llcY < 0 )
    {
        return TMF8X2X_SPAD_MAP_ERROR_CONFIG;
    }

    if ( mainSpadUrc( llcX, config->xSize ) >= TMF8X2X_MAIN_SPAD_SCREAMER_MAX_X_SIZE )
    {
        return TMF8X2X_SPAD_MAP_ERROR_CONFIG;
    }

    if ( mainSpadUrc( llcY, config->ySize ) >= TMF8X2X_MAIN_SPAD_SCREAMER_MAX_Y_SIZE )
    {
        return TMF8X2X_SPAD_MAP_ERROR_CONFIG;
    }

    /* center for odd X / Y SPAD map size */
    llcX = mainSpadLlc( TMF8X2X_MAIN_SPAD_SCREAMER_MAX_X_SIZE - 1, config->xOffset_2, config->xSize );
    llcY = mainSpadLlc( TMF8X2X_MAIN_SPAD_SCREAMER_MAX_Y_SIZE - 1, config->yOffset_2, config->ySize );

    if (  llcX < 0 )
    {
        return TMF8X2X_SPAD_MAP_ERROR_CONFIG;
    }

    if (  llcY < 0 )
    {
        return TMF8X2X_SPAD_MAP_ERROR_CONFIG;
    }

    if ( mainSpadUrc( llcX, config->xSize ) >= TMF8X2X_MAIN_SPAD_SCREAMER_MAX_X_SIZE )
    {
        return TMF8X2X_SPAD_MAP_ERROR_CONFIG;
    }

    if ( mainSpadUrc( llcY, config->ySize ) >= TMF8X2X_MAIN_SPAD_SCREAMER_MAX_Y_SIZE )
    {
        return TMF8X2X_SPAD_MAP_ERROR_CONFIG;
    }

    return TMF8X2X_SPAD_MAP_OK;
}

uint8_t tmf8x2xCheckMainSpadChannelSetup ( const uint8_t * spadMap, const uint8_t xSize, const uint8_t ySize )
{
    uint8_t spadsPerChannel[ TMF8X2X_NUMBER_OF_CHANNELS ];

    for ( uint32_t s = 0; s < TMF8X2X_NUMBER_OF_CHANNELS; ++s )
    {
        spadsPerChannel[ s ] = 0;
    }

    for ( uint32_t c = 0; c < xSize * ySize; ++c )
    {
        uint8_t currentChannel = spadMap[ c ];

        /* check if channel 0 was used in the channel assignments */
        if ( currentChannel == 0 )
        {
            return TMF8X2X_SPAD_MAP_ERROR_CONFIG;
        }

        /* check if an undefined channel was used in the channel assignments */
        if ( currentChannel >= TMF8X2X_NUMBER_OF_CHANNELS )
        {
            return TMF8X2X_SPAD_MAP_ERROR_CONFIG;
        }

        ++spadsPerChannel[ currentChannel ];
    }

    /* check if TDC1 .. TDC4 have at least one SPAD attached (electrical calibration) */
    if ( ( spadsPerChannel[ CHANNEL_2 ] + spadsPerChannel[ CHANNEL_3 ] ) == 0 )
    {
        return TMF8X2X_SPAD_MAP_ERROR_CONFIG;
    }

    if ( ( spadsPerChannel[ CHANNEL_4 ] + spadsPerChannel[ CHANNEL_5 ] ) == 0 )
    {
        return TMF8X2X_SPAD_MAP_ERROR_CONFIG;
    }

    if ( ( spadsPerChannel[ CHANNEL_6 ] + spadsPerChannel[ CHANNEL_7 ] ) == 0 )
    {
        return TMF8X2X_SPAD_MAP_ERROR_CONFIG;
    }

    if ( ( spadsPerChannel[ CHANNEL_8 ] + spadsPerChannel[ CHANNEL_9 ] ) == 0 )
    {
        return TMF8X2X_SPAD_MAP_ERROR_CONFIG;
    }

    return TMF8X2X_SPAD_MAP_OK;
}
