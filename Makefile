CFLAGS=-Wall

rgol: main.c
	$(CC) $(CFLAGS) -o rgol main.c -lglfw -lGL
