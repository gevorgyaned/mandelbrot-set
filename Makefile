CC = gcc
CFLAGS = -O2 -g -mavx2 -std=gnu99
LDFLAGS = -lm -lSDL2

SRCS = mandelbrot_avx.c
EXECS = $(SRCS:.c=)

.PHONY: all clean

all: $(EXECS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	$(RM) $(EXECS)
