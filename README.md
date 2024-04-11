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
