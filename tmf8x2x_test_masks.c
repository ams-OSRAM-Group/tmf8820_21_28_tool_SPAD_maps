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

/*! \file tmf8x2x_test_masks.c
 *  \brief generates a custom test SPAD mask for the TMF8820/8821 firmware
*/

/*
 *****************************************************************************
 * INCLUDES
 *****************************************************************************
 */

#include "tmf8x2x_spad_mask_tool.h"

/*
 *****************************************************************************
 * DEFINES
 *****************************************************************************
 */

/* only placeholder, no need to update this */
#define TEST_SPAD_MAP_ID        (0)

/* *** update these values for your custom SPAD map */
#define TEST_SPAD_MAP_XOFFSET_2 (0)
#define TEST_SPAD_MAP_YOFFSET_2 (0)
#define TEST_SPAD_MAP_XSIZE     (18)
#define TEST_SPAD_MAP_YSIZE     (6)

/*
 *****************************************************************************
 * TEST SPAD MAP, 3x3 checkerboard, 41° x 32°
 *****************************************************************************
 */

/* *** update these values for your custom SPAD map */
static const uint8_t testSpadMapChannel [ TEST_SPAD_MAP_XSIZE * TEST_SPAD_MAP_YSIZE ] =
        /* x = 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 */
/* y =  5 */ { 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3
    /*  4 */ , 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3
    /*  3 */ , 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6
    /*  2 */ , 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6
    /*  1 */ , 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9
    /*  0 */ , 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9
             };

/* *** update these values for your custom SPAD mask */
static const uint8_t testSpadMapEnable [ TEST_SPAD_MAP_XSIZE * TEST_SPAD_MAP_YSIZE ] =
    /* x = 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 */
    /* y =  5 */ { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0
        /*  4 */ , 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1
        /*  3 */ , 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0
        /*  2 */ , 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1
        /*  1 */ , 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0
        /*  0 */ , 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1
    };

/* holds the packed version of the SPAD enable mask */
static uint32_t testSpadMaskEnablePacked[ TEST_SPAD_MAP_YSIZE ];

/*
 *****************************************************************************
 * TEST SPAD MAP IN LINUX DRIVER FORMAT, 3x3 checkerboard, 41° x 32°
 *****************************************************************************
 */

/* for reference only, not used in this program */

//  echo -n "
//  1 1 1 1 1 1 2 2 2 2 2 2 3 3 3 3 3 3
//  1 1 1 1 1 1 2 2 2 2 2 2 3 3 3 3 3 3
//  4 4 4 4 4 4 5 5 5 5 5 5 6 6 6 6 6 6
//  4 4 4 4 4 4 5 5 5 5 5 5 6 6 6 6 6 6
//  7 7 7 7 7 7 8 8 8 8 8 8 9 9 9 9 9 9
//  7 7 7 7 7 7 8 8 8 8 8 8 9 9 9 9 9 9
//  " > app/spad_map_0
//
//  echo -n "
//  1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0
//  0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1
//  1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0
//  0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1
//  1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0
//  0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1
//  " > app/spad_mask_0

/*
 *****************************************************************************
 * LOCAL PROTOTYPES
 *****************************************************************************
 */

static void tmf8x2xPackEnableMask( void );
static void tmf8x2xDumpTestMap( void );
static void displayCommandLineHelp( void );

/*
 *****************************************************************************
 * TEST SPAD MAP CREATION
 *****************************************************************************
 */

/* note: odd sized masks of size 1 in a dimension should have an offset of 1 in the odd dimension so that the center is properly selected. The truncation is round-down. */
static const tmf8x2xSpadMask tmf8x2xSpadMaskTestCfg =
{
    testSpadMaskEnablePacked, testSpadMapChannel, TEST_SPAD_MAP_ID, TEST_SPAD_MAP_XOFFSET_2, TEST_SPAD_MAP_YOFFSET_2, TEST_SPAD_MAP_XSIZE, TEST_SPAD_MAP_YSIZE
};

/* convert the SPAD enable mask from human readable to packed binary format */
static void tmf8x2xPackEnableMask ( void )
{
    for ( uint32_t row = 0; row < TEST_SPAD_MAP_YSIZE; ++row )
    {
        uint32_t currentRowMap = 0;

        for ( uint32_t col = 0; col < TEST_SPAD_MAP_XSIZE; ++col )
        {
            currentRowMap |= ( testSpadMapEnable[ row * TEST_SPAD_MAP_XSIZE + col ] > 0 ? 1 : 0 ) << col;
        }

        testSpadMaskEnablePacked[ row ] = currentRowMap;
    }
}

/* check test SPAD map / mask and output in human readable format */
static void tmf8x2xDumpTestMap ( void )
{
    tmf8x2xHalMainSpadConfig cfg;
    tmf8x2xPackEnableMask();

    tmf8x2xHalMainSpadConfig * spadConfig = tmf8x2xCreateMainSpad( &cfg, &tmf8x2xSpadMaskTestCfg );

    if ( spadConfig == 0 )
    {
        dumpString( "ERROR creating Test SPAD Setup (basic checks and channel 0/1 / 8/9 assignment).\n" );
        return;
    }

    if ( tmf8x2xCheckMainSpadArea( &cfg ) == TMF8X2X_SPAD_MAP_ERROR_CONFIG )
    {
        dumpString( "ERROR creating Test SPAD Setup (SPAD map out of bounds - size / offset).\n" );
        return;
    }

    if ( tmf8x2xCheckMainSpadChannelSetup( testSpadMapChannel, cfg.xSize, cfg.ySize ) == TMF8X2X_SPAD_MAP_ERROR_CONFIG )
    {
        dumpString( "ERROR creating Test Spad Map (channel setup checks).\n" );
        return;
    }

    if ( tmf8x2xCheckMainSpadAssignment( &cfg ) == TMF8X2X_SPAD_MAP_ERROR_CONFIG )
    {
        dumpString( "ERROR creating Test Spad Setup (SPAD assignment checks).\n" );
        return;
    }

    dumpMainSpadConfigAsCstruct(    "tmf8x2xTestSpadMap", &cfg );
    dumpMainSpadConfigAsI2Cstrings( "tmf8x2xTestSpadMap", &cfg );
    dumpString( "\n" );
    dumpChannelMapAsText( &tmf8x2xSpadMaskTestCfg );
    dumpString( "\n" );
    dumpMainSpadEnableBitsAsText( &cfg );
}

static void displayCommandLineHelp ( void )
{
    dumpString( "Run this program without any command line parameters to generate the data structures for a custom SPAD map.\n\n" );
    dumpString( "To define your custom SPAD map update the source code in the section *** DEFINES.\n" );
    dumpString( "After that assign SPADs to TDC channels in the array testSpadMapChannel[ ]\n" );
    dumpString( "Enable or disable SPADs by setting '1' (enable) or '0' (disable) in testSpadMapEnable[ ]\n" );
    dumpString( "Be sure to compare your intended SPAD setup with the output this tool provides as visual feedback.\n" );
}

int main(int argc, char **argv)
{
    (void)argv; /* prevent compiler warning */

    dumpString( "SPAD map tool - standalone version v1.0\n" );
    dumpString( "(c) 2022 by ams OSRAM AG. All rights reserved.\n\n" );

    if ( argc > 1 )
    {
        displayCommandLineHelp();
    }
    else
    {
        tmf8x2xDumpTestMap();
    }

    return 0;
}
