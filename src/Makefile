OBJS := $(patsubst %.cpp, %.o, $(wildcard *.cpp)) 
HDRS := $(wildcard ../inc/*.h)
CXX=g++
LDFLAGS=-ldl -Wl,-E
SDLLIBS:=$(if $(USE_SDL), `sdl-config --libs`,)
CXXFLAGS=-O2 -W -Wall -ansi -pedantic -g
SDLFLAGS:=$(if $(USE_SDL), -DSDL `sdl-config --cflags`,)

all: ../dbot

cvsall: upd ../dbot

upd:
	(cd ..; cvs update)

../dbot: $(OBJS)
	$(CXX) $(LDFLAGS) -o ../dbot $(OBJS) $(SDLLIBS)

clean:
	rm -f $(OBJS)

distclean: clean
	rm -f *~ ../dbot


%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(SDLFLAGS)
