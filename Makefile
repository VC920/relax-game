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
	$(CC) $(CFLAGS) -o rg $(SRCS) $(INCLUDES) $(LIBS)

run:
	./rg

clean:
	rm -rf ./rg
