
CC=g++
CFLAGS=-Wall -O -g

httpDownLoad : 
	$(CC) $(OBJS) -o httpDownLoad httpDown.cpp main.cpp -lcurl -lpthread

clean:
	rm -rf *.o httpDownLoad
