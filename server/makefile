CFLAGS=-c

all:
	$(CROSS_COMPILE)gcc main.c serv_thread_func.c file_size.c -pthread -o server

rebuild:
	make clean
	make

clean:
	rm -rf *.o server
