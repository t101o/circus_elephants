
OBJS := $(patsubst %.c,%.o,$(wildcard *.c))
CFLAGS = `sdl2-config --cflags` --std=c99 -O0 -Wall
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm
CC = clang

	
game:${OBJS}
	${CC} ${CFLAGS} -o ../bin/$@ ${OBJS} ${LIBS}
	
clean:
	rm -f *.o
.c.o:
	${CC} ${CFLAGS}  -c $? -o $@
