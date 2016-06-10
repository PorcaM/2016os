CC = gcc
CFLAGS =
OBJS = main.o paging.o
SRCS = main.c paging.c
PROG = memory_manager

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS)

main.o: paging.h main.c
paging.o: paging.h paging.c

clean:
	rm $(OBJS) 
