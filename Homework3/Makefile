CC = gcc
CFLAGS =
OBJS = main.o paging.o LRU_stack.o
SRCS = main.c paging.c LRU_stack.c
PROG = memory_manager

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS)

main.o: paging.h main.c
paging.o: paging.h LRU_stack.h paging.c
LRU_stack.o: LRU_stack.h LRU_stack.c

clean:
	rm $(OBJS) 
