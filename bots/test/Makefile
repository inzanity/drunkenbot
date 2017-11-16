OBJS := $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS := $(wildcard *.h)

LIBS := 
CXXFLAGS := -W -Wall -pedantic -ansi -g -c -fPIC

all: ../test.so

../test.so: $(OBJS)
	$(CXX) $(LIBS) $(OBJS) -o ../test.so -lc -shared -Wl,-soname,test.so

%.o: %.cpp $(HEADERS) Makefile
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) core

distclean: clean
	rm -f *~ ../test.so
