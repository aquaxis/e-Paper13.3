DIR_SRC = ./src
DIR_BIN = ./bin

OBJ_C = $(wildcard ${DIR_SRC}/*.c)
OBJ_O = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${OBJ_C}))

TARGET = epd
CC = gcc

USELIB = USE_BCM2835_LIB
# USELIB = USE_WIRINGPI_LIB

ifeq ($(USELIB), USE_BCM2835_LIB)
    LIB = -lbcm2835 -lm
else ifeq ($(USELIB), USE_WIRINGPI_LIB)
    LIB = -lwiringPi -lm
endif

MSG = -g -O0 -Wall
DEBUG = -D DEBUG
CFLAGS += $(MSG) $(DEBUG) -D $(USELIB)

$(shell mkdir -p $(DIR_BIN))

${TARGET}:${OBJ_O}
	$(CC) $(CFLAGS) $(OBJ_O) -o $@ $(LIB)

${DIR_BIN}/%.o : $(DIR_SRC)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB) -I $(DIR_SRC)

clean :
	rm $(DIR_BIN)/*.*
	rm $(TARGET)