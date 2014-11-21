chaos:chaos.c
	gcc -o $@ $^  -O2 -I/usr/local/include/SDL  -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lSDL
