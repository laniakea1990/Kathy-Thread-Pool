EXEC = main
CC = gcc
CFLAGS = -c -Wall -O -g
$(EXEC) : kathy_thread_pool.o test.o
	$(CC) -o $(EXEC) kathy_thread_pool.o test.o -lpthread

ks_thread_pool.o : kathy_thread_pool.c kathy_thread_pool.h
	$(CC) $(CFLAGS) kathy_thread_pool.c -lpthread

test.o : test.c
	$(CC) $(CFLAGS) test.c

clean : 
	rm *.o main
