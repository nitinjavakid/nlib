SRC = adc.c

MMCU = atmega328p

OBJS = $(SRC:.c=.${MMCU}.o)

all: clean ${OBJS}

clean:
	rm -f ${OBJS}

%.${MMCU}.o: %.c
	avr-g++ -Os -c -mmcu=${MMCU} -o $@ $<

