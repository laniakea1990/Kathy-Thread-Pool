EXEC = main
CC = gcc
CFLAGS = -c -Wall -O -g
$(EXEC) : ks_thread_pool.o test.o
	$(CC) -o $(EXEC) ks_thread_pool.o test.o -lpthread

ks_thread_pool.o : ks_thread_pool.c ks_thread_pool.h
	$(CC) $(CFLAGS) ks_thread_pool.c -lpthread

test.o : test.c
	$(CC) $(CFLAGS) test.c

clean : 
	rm *.o main
