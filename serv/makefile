CC=gcc
CFLAGS=-g -Wall
OBJS=serv.o
TARGET=serv
 
$(TARGET): $(OBJS)
	$(CC) -o $(@) $(OBJS)


serv.o: serv.h serv.c
clean:
	rm -f $(OBJS)
	rm -f $(TARGET)