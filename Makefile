INCLUDE_DIRS = 
LIB_DIRS = 
CC=g++

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= -lrt
CPPLIBS= -L/usr/lib -lopencv_core -lopencv_flann -lopencv_video

HFILES= 
CFILES= 
CPPFILES= lane_detection.cpp
#hough_circle.cpp hough_line.cpp canny.cpp sobel.cpp capture.cpp captureskel.cpp

SRCS= ${HFILES} ${CFILES}
CPPOBJS= ${CPPFILES:.cpp=.o}

all:	lane_detection


clean:
	-rm -f *.o
	-rm -f lane_detection

distclean:
	-rm -f *.o *.d



lane_detection: lane_detection.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o `pkg-config --libs opencv` $(CPPLIBS)



depend:

.c.o:
	$(CC) $(CFLAGS) -c $<

.cpp.o:
	$(CC) $(CFLAGS) -c $<
