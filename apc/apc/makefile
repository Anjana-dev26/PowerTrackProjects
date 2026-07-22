OBJ := $(patsubst %.c, %.o, $(wildcard *.c))

a.out: $(OBJ)
	gcc -o $@ $^

clean:
	rm -f *a.out *.o
