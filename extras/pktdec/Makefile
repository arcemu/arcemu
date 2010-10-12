CC = g++
 
CFLAGS = -Wall -O3 -fno-strict-aliasing
CFLAGS+= `xml2-config --cflags`

LNFLAGS = -lm -lz -lrt -Wall -g
LNFLAGS+= `xml2-config --libs` 

PROG = pktdec
OBJS = Main.o Opcodes.o
DEPS = $(OBJS:.o=.d)

all: pktdec

pktdec: ${OBJS}
	${CC} ${OBJS} -o ${PROG} ${LNFLAGS}

clean:
	rm -f *.o *.d ${PROG}

%.o: %.c
	${CC} ${CFLAGS} -c -o $@ $< -MD -MF $(patsubst %.o,%.d,$@)

%.o: %.cpp
	${CC} ${CFLAGS} -c -o $@ $< -MD -MF $(patsubst %.o,%.d,$@)

.PHONY: clean

-include ${DEPS}
 