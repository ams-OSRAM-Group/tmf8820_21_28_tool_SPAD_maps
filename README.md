About
=====

This SPAD tool is used to create a custom SPAD mask and map for the TMF882x zones.
For a time-multiplexed SPAD map (4x4 zones), you need to generate two SPAD masks + maps separately.

Create a new SPAD mask
======================

1. Write your SPAD mask and SPAD map into tmf8x2x_test_masks.c:
  - TEST_SPAD_MAP_XOFFSET_2 / TEST_SPAD_MAP_YOFFSET_2
  - TEST_SPAD_MAP_XSIZE / TEST_SPAD_MAP_YSIZE
  - testSpadMapChannel
  - testSpadMapEnable
2. Compile your project via "make"
3. Run ./spad_masks from a command line. The tool prints the SPAD map and mask as c-struct, I2C sequence, and 2D-view.

Sample Output
=============

```
SPAD map tool - standalone version v1.0
(c) 2022 by ams OSRAM AG. All rights reserved.

/* use this format for custom SPAD maps in the TMF882X firmware */
const tmf8x2xHalMainSpadConfig tmf8x2xTestSpadMap = 
{ /*enableSpad[ ]*/ 0x2aaaa, 0x15555, 0x2aaaa, 0x15555, 0x2aaaa, 0x15555, 0x0, 0x0, 0x0, 0x0
, /*tdcChannel[ ]*/ 0xf00c33, 0xf00c33, 0xf00c33, 0xf00c33, 0xf00c33, 0xf00c33, 0xc0c00c, 0xc0c00c, 0xc0c00c, 0xc0c00c, 0xc0c00c, 0xc0c00c, 0xc0f033, 0xc0f033, 0xc0f033, 0xc0f033, 0xc0f033, 0xc0f033
, /*tdcChannelSelect*/ 0x3
, /*xOffset_2*/ 0
, /*yOffset_2*/ 0
, /*xSize*/ 18
, /*ySize*/ 6
};

# use this format to set up custom SPAD maps via I2C transfers
# tmf8x2xHalMainSpadConfig tmf8x2xTestSpadMap
# enableSpad[ ]
S 41 W 24 aa aa 02 55 55 01 aa aa 02 55 55 01 aa aa 02 55 55 01 00 00 00 00 00 00 00 00 00 00 00 00 P
# tdcChannel[ ]
S 41 W 42 33 0c f0 00 33 0c f0 00 33 0c f0 00 33 0c f0 00 33 0c f0 00 33 0c f0 00 0c c0 c0 00 0c c0 c0 00 0c c0 c0 00 0c c0 c0 00 0c c0 c0 00 0c c0 c0 00 33 f0 c0 00 33 f0 c0 00 33 f0 c0 00 33 f0 c0 00 33 f0 c0 00 33 f0 c0 00 P
# tdcChannelSelect
S 41 W 8a 03 00 00 P
# xOffset_2
S 41 W 8d 00 P
# yOffset_2
S 41 W 8e 00 P
# xSize
S 41 W 8f 12 P
# ySize
S 41 W 90 06 P

/* SPAD Map Assignment between Zones and TDCs */
xOffset_2=0 yOffset_2=0 xSize=18 ySize=6
/* x =     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7  */

/* y=5 */  1 1 1 1 1 1 2 2 2 2 2 2 3 3 3 3 3 3 
/* y=4 */  1 1 1 1 1 1 2 2 2 2 2 2 3 3 3 3 3 3 
/* y=3 */  4 4 4 4 4 4 5 5 5 5 5 5 6 6 6 6 6 6 
/* y=2 */  4 4 4 4 4 4 5 5 5 5 5 5 6 6 6 6 6 6 
/* y=1 */  7 7 7 7 7 7 8 8 8 8 8 8 9 9 9 9 9 9 
/* y=0 */  7 7 7 7 7 7 8 8 8 8 8 8 9 9 9 9 9 9 

/* SPAD Enable Mask Visualization */
/* x =     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7  */

/* y=11 */ 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
/* y=10 */ 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
/* y= 9 */ 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
/* y= 8 */ 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 
/* y= 7 */ 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 
/* y= 6 */ 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 
/* y= 5 */ 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 
/* y= 4 */ 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 
/* y= 3 */ 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 
/* y= 2 */ 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
/* y= 1 */ 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
/* y= 0 */ 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
```