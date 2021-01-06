CC=gcc
LIBS=-lm

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

dcal: dcal.o ddate-routine.o
	$(CC) $(CFLAGS) $(LIBS) -o dcal dcal.o ddate-routine.o

ddate: dcal.o ddate-routine.o
	$(CC) $(CFLAGS) $(LIBS) -o ddate dcal.o ddate-routine.o

clean:
	rm -f ddate.o dcal.o ddate-routine.o dcal ddate
	rm -f *~
