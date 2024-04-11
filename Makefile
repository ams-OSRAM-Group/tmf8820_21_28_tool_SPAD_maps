spad_tool: tmf8x2x_test_masks.o tmf8x2x_spad_mask_tool.o
	cc *.o -o spad_tool

clean:
	rm -f spad_tool *.o
