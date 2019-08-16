
INCLUDE_DIRS = 
LIB_DIRS = 
CC=g++

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= -lrt -lpthread
CPPLIBS= -L/usr/lib -lopencv_core -lopencv_flann -lopencv_video

HFILES:= -I ../inc/
CPPFILES= $(wildcard *.cpp)

CPPOBJS= $(CPPFILES:.cpp=.o)

all:	main


clean:
	-rm -f *.o
	-rm -f main
	-rm -f *.jpg

distclean:
	-rm -f *.o *.d


main: $(CPPOBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(CPPOBJS) `pkg-config --libs opencv` $(CPPLIBS)  $(HFILES) $(LIBS) 


%.o: %.cpp
	$(CC) -c $(CFLAGS) $^ -o $@ $(LIBS) $(HFILES)

