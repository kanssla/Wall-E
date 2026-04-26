CC ?= gcc
CFLAGS ?= 
PREFIX ?= /usr/local
DESTDIR ?=
USER_HOME := $(shell echo $HOME)

.PHONY: all clean install-sytem install-user uninstall-system uninstall-user 
all: sw cw getmonitor

sw: sw.c
	$(CC) $(CFLAGS) sw.c -o sw -lX11 -lXrandr

cw: cw.c getmonitor.o
	$(CC) $(CFLAGS)  cw.c getmonitor.o -o cw -lX11 -lXrandr

getmonitor.o: getmonitor.c
	$(CC) $(CFLAGS) -c getmonitor.c -o getmonitor.o -lX11 -lXrandr

getmonitor: getmon.c getmonitor.o
	$(CC) $(CFLAGS) getmon.c getmonitor.o -o getmonitor -lX11 -lXrandr

clean:
	rm getmonitor.o
	rm getmonitor
	rm sw
	rm cw

install-system: all
	cp getmonitor $(DESTDIR)$(PREFIX)/bin/
	cp sw $(DESTDIR)$(PREFIX)/bin/
	cp cw $(DESTDIR)$(PREFIX)/bin/

install-user: getmonitor
	mkdir -p $(HOME)/.config/cw/
	mkdir -p $(HOME)/Documents/wallpapers/
	cp wallpaper.jpg $(HOME)/Documents/wallpapers/

uninstall-system: 
	rm $(DESTDIR)$(PREFIX)/bin/getmonitor
	rm $(DESTDIR)$(PREFIX)/bin/sw
	rm $(DESTDIR)$(PREFIX)/bin/cw

uninstall-user: 
	rm -rf $(HOME)/.config/cw
