CC = /opt/opencilk/bin/clang
FLAGS = -lm -fopencilk -O3
SRC = src/*.c
OUT = closest-pair.out

.PHONY: all
all:
	$(CC) $(FLAGS) $(SRC) -o $(OUT)

.PHONY: run
run: all
	./$(OUT)

.PHONY: clean
clean:
	rm $(OUT)
