CC = gcc

DEPS = virtual_mem.h
OBJ = sortArrays.o virtual_mem.o

CLAGS = -02 -g
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

sortArrays: $(OBJ)
	gcc $(CFLAGS) -o $@ $^ -lm -lpthread
.PHONY : clean
clean:
	-rm *.o $(objects) sortArrays
