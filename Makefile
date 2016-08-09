CC = gcc
CFLAGS  = -g -Wall -c 

all: utarray.h sqlite3.h
	$(CC) $(CFLAGS) sqlite3.c -o sqlite3.o
	$(CC) $(CFLAGS) main.c -o main.o
	gcc -o test -pthread -ldl -lm sqlite3.o main.o
#gcc -Os -I. -DSQLITE_THREADSAFE=0 -DSQLITE_ENABLE_FTS4 -DSQLITE_ENABLE_EXPLAIN_COMMENTS   -DHAVE_USLEEP -DHAVE_READLINE  -ldl -lreadline -c -o sqlite3 sqlite3.c


clean: 
	$(RM) count *.o *~
	$(RM) test
	$(RM) grafo2.db

