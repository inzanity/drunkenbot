USE_SDL:=$(shell sdl-config --version)

all:
	make -C src USE_SDL=$(USE_SDL)
	make -C bots

alot:
	make -C bots alot

clean:
	make -C src clean
	make -C bots clean

distclean:
	make -C src distclean
	make -C bots distclean
