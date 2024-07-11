.PHONY: all clean run mc_hdc

CC := gcc
CFLAGS := -O3 -Wall -g
LDFLAGS := -lm

INC += inc/
SRC = $(wildcard src/*.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

all: run

run: mc_hdc
	./mc_hdc

mc_hdc: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) -c -o $@ -I $(INC) $(CFLAGS) $<

clean:
	rm **/*.o
	rm mc_hdc
