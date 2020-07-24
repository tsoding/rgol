CFLAGS=-Wall
LIBS=-lglfw -lGL

all: rgol rgol_glfw gen_nbor_masks

gen_nbor_masks: gen_nbor_masks.c
	$(CC) $(CFLAGS) -o gen_nbor_masks gen_nbor_masks.c

rgol: main.c rgol.h
	$(CC) $(CFLAGS) -o rgol main.c

rgol_glfw: main_glfw.c rgol.h
	$(CC) $(CFLAGS) -o rgol_glfw main_glfw.c $(LIBS)
