#
# @file Makefile
# @brief contains build rules for HOST, BBB, and KL25Z
#
# @author Seth Miers and Jake Cazden
# @date February 11, 2017
#

# include source files
include sources.mk

INC_DIR = ../include/
INC_COMMON = ../include/common
INC_CMSIS = ../include/CMSIS
INC_KL25Z = ../include/kl25z
KL25ZLD = ../platform/MKL25Z128xxx4_flash.ld
INCLUDES = $(INC_DIR) $(INC_COMMON) $(INC_CMSIS) $(INC_KL25Z)
INC_PARAMS=$(foreach d, $(INCLUDES), -I$d)
STD = -std=c99
DEP= -M

FILES = $(SOURCES)
OBJS = $(SOURCES:.c=.o)
IFILES = $(SOURCES:.c=.i)
ASMS = $(SOURCES:.c=.asm)
DEPS = $(SOURCES:.c=.d)

ifndef PLATFORM
PLATFORM=HOST
endif

ifdef VERBOSE
CFLAGS += -DVERBOSE
endif

ifdef DEBUG
CFLAGS += -DDEBUG
endif

ifdef CMOCKA
FILES += unittest.c
OBJS += unittest.o
INCLUDES += ../cmocka/include ../cmocka/lib ../cmocka
LIBS += -L../cmocka/lib
CFLAGS += -DCMOCKA
POSTFLAGS = -lcmocka
endif

ifndef PROJECT
PROJECT=PROJECT3
endif

ifeq ($(PROJECT),PROJECT1)
OUTPUT=project1.elf
LDFLAGS += ,-Map=project1.map
FILES += project1.c
OBJS += project1.o
endif
ifeq ($(PROJECT),PROJECT2)
OUTPUT=project2.elf
LDFLAGS += ,-Map=project2.map
FILES += project2.c
OBJS += project2.o
endif
ifeq ($(PROJECT),PROJECT3)
OUTPUT=project3.elf
LDFLAGS += ,-Map=project3.map
FILES += project3.c
OBJS += project3.o
endif

ifeq ($(PLATFORM), HOST)
#	echo Building for platform: HOST
CC=gcc
CFILES=$(FILES)
		
CFLAGS +=$(STD)\
		-Wall\
		-Werror\
		-Wfatal-errors\
		-O0\
		-g
CPPFLAGS +=  $(INC_PARAMS)\
			-D $(PLATFORM)\
			-D $(PROJECT)
endif
ifeq ($(PLATFORM), BBB)
#	echo Building for platform: BBB
CC=arm-linux-gnueabihf-gcc
CFILES +=$(FILES)
CFLAGS +=$(STD)\
		-Wall\
		-Werror\
		-Wfatal-errors\
		-O0\
		-g
CPPFLAGS +=  $(INC_PARAMS)\
			-D $(PLATFORM)\
			-D $(PROJECT)
endif
ifeq ($(PLATFORM), KL25Z)
#	echo Building for platform: KL25Z
CC=arm-none-eabi-gcc
FILES +=startup_MKL25Z4.S\
	system_MKL25Z4.c\
	port.c\
	arch_arm32.c\
	uart.c\
	nordic.c\
	spi.c
OBJS += startup_MKL25Z4.o\
	system_MKL25Z4.o\
	port.o\
	arch_arm32.o\
	uart.o\
	nordic.o\
	spi.o
CFILES +=$(FILES)
CFLAGS +=$(STD)\
		-Wall\
		-Werror\
		-Wfatal-errors\
		-O0\
		-g\
		-mcpu=cortex-m0plus\
		-mthumb\
		-march=armv6-m\
		-mfloat-abi=soft\
		--specs=nosys.specs\
		-fgnu89-inline
CPPFLAGS +=  $(INC_PARAMS)\
			-D $(PLATFORM)\
			-D $(PROJECT)
ifeq ($(PROJECT),PROJECT1)
LDFLAGS = ,-Map=project1.map,-T,$(KL25ZLD)
endif
ifeq ($(PROJECT),PROJECT2)
LDFLAGS = ,-Map=project2.map,-T,$(KL25ZLD)
endif
ifeq ($(PROJECT),PROJECT3)
LDFLAGS = ,-Map=project3.map,-T,$(KL25ZLD)
endif
endif


.PHONY: all
all: build

%.i: %.c
	$(CC) $(CPPFLAGS) -E $^ -o $@

%.asm: %.c
	$(CC) $(CPPFLAGS) $(LIBS) $(CFLAGS) -S $^ -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(LIBS) $(CFLAGS) -c $^ -o $@
%.o: %.S
	$(CC) $(CPPFLAGS) $(LIBS) $(CFLAGS) -c $^ -o $@

%.d: %.c
	$(CC) $(CPPFLAGS) $(LIBS) -M $(CFLAGS) -c $^ -o $@

.PHONY: compile-all
compile-all: $(OBJS) $(DEPS)

.PHONY: build
build: compile-all
	$(CC) $(CPPFLAGS) $(LIBS) $(CFLAGS) -Wl$(LDFLAGS) $(OBJS) -o $(OUTPUT) $(POSTFLAGS)

.PHONY: clean
clean:
	rm *.map *.out *.o *.d *.i *.asm *.elf
