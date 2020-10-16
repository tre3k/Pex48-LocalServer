all:
	gcc main.c -o pex48localdevice -O2 -I/usr/include/ixpio
clean:
	rm -rf pex48localdevice *.o
