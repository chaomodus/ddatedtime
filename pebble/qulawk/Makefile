# makefile cuz waf was serious harshing my buzz

QULAWKPATH=../../qulawk/c/
DDATEPATH=../../ddate/c/
PHONE=192.168.0.32

build: src/qulawk.c src/qulawk-routine.h src/qulawk-routine.c src/ddate-routine.h src/ddate-routine.c appinfo.json
	pebble build

src/qulawk-routine.h: $(QULAWKPATH)/qulawk-routine.h
	cp $(QULAWKPATH)/qulawk-routine.h src/

src/qulawk-routine.c: $(QULAWKPATH)/qulawk-routine.c
	cp $(QULAWKPATH)/qulawk-routine.c src/

src/ddate-routine.c: $(DDATEPATH)/ddate-routine.c
	cp $(DDATEPATH)/ddate-routine.c src/

src/ddate-routine.h: $(DDATEPATH)/ddate-routine.h
	cp $(DDATEPATH)/ddate-routine.h src/

clean:
	rm -f src/*-routine.[hc]
	pebble clean

install: build
	pebble install --phone $(PHONE)
