CC      = gcc
CFLAGS  = -Wall -Wextra -g -Iinc
SRCS    = src/main.c src/parser.c src/component.c src/geometry.c \
          src/graph.c src/emitter.c src/wire.c src/ground.c
TARGET  = ltspice2tikz

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) -lm

clean:
	rm -f $(TARGET)