CC = gcc
rad_OBJS = send.o
CFLAGS += -Wall -g 
all: send
%.o:%.c
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<
send: $(rad_OBJS)
	$(CC) -o $@ $(rad_OBJS)
clean:
	rm -f rbcfg *.o
	rm -f rbcfg *.so
