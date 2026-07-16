CC = gcc
CFLAGS = -Wall -Wextra

SRCS = \
thirdparty/glad/src/glad.c \
src/game/*.c \
src/render/*.c


INCLUDES = \
-Ithirdparty/cglm/include \
-Ithirdparty/glad/include \
-Ithirdparty/stb/include \
-Isrc/game \
-Isrc/render

LIBS = -lSDL2 -lm

all:
	$(CC) $(CFLAGS) -o rr $(SRCS) $(INCLUDES) $(LIBS)

run:
	./rr

clean:
	rm -rf ./rr
