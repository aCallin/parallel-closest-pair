CC = /opt/opencilk/bin/clang
FLAGS = -lm -fopencilk -O3
SRC = main.c point.c test.c
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
