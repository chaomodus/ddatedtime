CC=gcc
LIBS=-lm

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

qulawk: qulawk.o qulawk-routine.o
	$(CC) $(CFLAGS) $(LIBS) -o qulawk qulawk-routine.o qulawk.o

clean:
	rm -f qulawk qulawk.o qulawk-routine.o
	rm -f *~
