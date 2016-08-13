CC = gcc
CFLAGS  = -g -Wall -c 

all: sqlite3 bfs_db bfs_algorithm sqlite3.h
	$(CC) $(CFLAGS) -c main.c -o main.o
	$(CC) -o test sqlite3.o bfs_db.o bfs_algorithm.o main.o -pthread -ldl -lm

sqlite3: sqlite3.h 
	$(CC) $(CFLAGS) sqlite3.c -o sqlite3.o

bfs_db: utarray.h uthash.h db.h bfs_db.h type.h sqlite3.h 
	$(CC) $(CFLAGS) bfs_db.c -o bfs_db.o sqlite3.o  

bfs_algorithm: utarray.h uthash.h bfs_db.h type.h algorithm.h bfs_algorithm.h 
	$(CC) $(CFLAGS) bfs_algorithm.c -o bfs_algorithm.o 
	
#gcc -Os -I. -DSQLITE_THREADSAFE=0 -DSQLITE_ENABLE_FTS4 -DSQLITE_ENABLE_EXPLAIN_COMMENTS   -DHAVE_USLEEP -DHAVE_READLINE  -ldl -lreadline -c -o sqlite3 sqlite3.c


clean: 
	$(RM) count *.o *~
	$(RM) test
	$(RM) grafo2.db
	$(RM) time.txt

