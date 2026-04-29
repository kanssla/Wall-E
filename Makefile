CC ?= gcc
CFLAGS ?= 
PREFIX ?= /usr/local
DESTDIR ?=

.PHONY: all clean install prereq uninstall
prereq: config.h
	mkdir -p $(HOME)/.config/cw/
	mkdir -p $(HOME)/Documents/wallpapers/
	cp wallpaper.jpg $(HOME)/Documents/wallpapers/

config.h:
	echo 'static const char configpath[] = "$(HOME)/.config/cw/";' > config.h
	echo 'static const char wallpaperspath[] = "$(HOME)/Documents/wallpapers/";' >> config.h

all: cw getmonitor

cw: cw.c getmonitor.o 
	$(CC) $(CFLAGS)  cw.c getmonitor.o -o cw -lX11 -lXrandr

getmonitor.o: getmonitor.c 
	$(CC) $(CFLAGS) -c getmonitor.c -o getmonitor.o -lX11 -lXrandr

getmonitor: getmon.c getmonitor.o
	$(CC) $(CFLAGS) getmon.c getmonitor.o -o getmonitor -lX11 -lXrandr

clean:
	rm getmonitor.o
	rm getmonitor
	rm cw

install: all
	@if [ ! -f config.h ]; then \
		echo "Run 'make' as a normal user first"; \
		exit 1; \
	fi
	cp getmonitor $(DESTDIR)$(PREFIX)/bin/
	cp cw $(DESTDIR)$(PREFIX)/bin/


uninstall: 
	rm $(DESTDIR)$(PREFIX)/bin/getmonitor
	rm $(DESTDIR)$(PREFIX)/bin/cw

