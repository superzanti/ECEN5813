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
CFLAGS+=-DVERBOSE
endif

ifndef PROJECT
PROJECT=PROJECT1
endif

ifeq ($(PLATFORM), HOST)
#	echo Building for platform: HOST
CC=gcc
FILES+=unittest.c
OBJS+=unittest.c
CFILES=$(FILES)
		
CFLAGS =$(STD)\
		-Wall\
		-Werror\
		-Wfatal-errors\
		-O0\
		-g
CPPFLAGS +=  $(INC_PARAMS)\
			-D $(PLATFORM)\
			-D $(PROJECT)
LDFLAGS = -Map=project1.map
OUTPUT=project1.elf
endif
ifeq ($(PLATFORM), BBB)
#	echo Building for platform: BBB
CC=arm-linux-gnueabihf-gcc
CFILES+=$(FILES)
CFLAGS+=$(STD)\
		-Wall\
		-Werror\
		-Wfatal-errors\
		-O0\
		-g
CPPFLAGS +=  $(INC_PARAMS)\
			-D $(PLATFORM)\
			-D $(PROJECT)
LDFLAGS = -Map=project1.map
OUTPUT=project1.elf
endif
ifeq ($(PLATFORM), KL25Z)
#	echo Building for platform: KL25Z
CC=arm-none-eabi-gcc
FILES+=startup_MKL25Z4.S\
	system_MKL25Z4.c
	port.c
	arch_arm32.c
	uart.c
OBJS+= startup_MKL25Z4.o\
	system_MKL25Z4.o
	port.o
	arch_arm32.o
	uart.o
CFILES=$(FILES)
CFLAGS =$(STD)\
		-Wall\
		-Werror\
		-Wfatal-errors\
		-O0\
		-g\
		-mcpu=cortex-m0plus\
		-mthumb\
		-march=armv6-m\
		-mfloat-abi=soft\
		--specs=nosys.specs
CPPFLAGS =  $(INC_PARAMS)\
			-D $(PLATFORM)\
			-D $(PROJECT)
LDFLAGS = -Map=project1.map,-T,$(KL25ZLD)
OUTPUT=project1.elf
endif

.PHONY: all
all: build

%.i: %.c
	$(CC) $(CPPFLAGS) -E $^ -o $@

%.asm: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -S $^ -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $^ -o $@
%.o: %.S
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $^ -o $@

%.d: %.c
	$(CC) $(CPPFLAGS) -M $(CFLAGS) -c $^ -o $@

.PHONY: compile-all
compile-all: $(OBJS) $(DEPS)

.PHONY: build
build: compile-all
	$(CC) $(CPPFLAGS) $(CFLAGS) -Wl,$(LDFLAGS) $(OBJS) -o $(OUTPUT)

.PHONY: clean
clean:
	rm *.map *.out *.o *.d *.i *.asm *.elf
#
# compile to c89 standard				-std=c89
# include 'include' directory			-I$(INC_DIR)
# include 'include/common' directory 	-I$(INC_COMMON)
# show all warnings 					-Wall
# show all errors 						-Werror
# stop compilation on first error 		-Wfatal-errors
# no optimization 						-O0
# define PROJECT1 						-D PROJECT1
# define VERBOSE 						-D VERBOSE
# compile with debugger support 		-g
