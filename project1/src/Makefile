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
#INC = $(INC_DIR) $(INC_COMMON) $(INC_CMSIS) $(INC_KL25Z)
#INC_PARAMS=$(foreach d, $(INC), -I$d)
STD = -std=c99

FILES = $(SRCS)
OBJS = $(SRCS:.c=.o)
IFILES = $(SRCS:.c=.i)
ASMS = $(SRCS:.c=.asm)

ifndef PLATFORM
PLATFORM=HOST
endif

ifeq ($(PLATFORM), HOST)
#	echo Building for platform: HOST
CC=gcc
LL = ld
CFILES=$(FILES)
CFLAGS =$(STD)\
		-Wall\
		-Werror\
		-Wfatal-errors\
		-O0
CPPFLAGS =  -I$(INC_DIR)\
			-I$(INC_COMMON)\
			-D PROJECT1\
			-D VERBOSE
LDFLAGS = -Map=project1.map
OUTPUT=project1.elf
endif
ifeq ($(PLATFORM), BBB)
#	echo Building for platform: BBB
CC=arm-none-eabi-gcc
LL = ld
OUTPUT=project1.elf
endif
ifeq ($(PLATFORM), KL25Z)
#	echo Building for platform: KL25Z
CC=arm-linux-gnueabi-gcc
LL = ld
CFILES=$(FILES)
CFLAGS =$(STD)\
		-Wall\
		-Werror\
		-Wfatal-errors\
		-O0
CPPFLAGS =  -I$(INC_DIR)\
			-I$(INC_COMMON)\
			-D PROJECT1\
			-D VERBOSE
LDFLAGS = -Map=project1.map
OUTPUT=project1.elf
endif


%.i: %.c
	$(CC) $(CPPFLAGS) -E -o $@ $^

%.asm: %.i
	$(CC) $(CFLAGS) -S -o $@ $^

%.o: %.asm
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: compile-all
compile-all: $(OBJS)

.PHONY: build
build: compile-all
	$(LL) $(LDFLAGS) $(OBJS) -o $(OUTPUT)

.PHONY: clean
clean:
	rm -r *.map *.out *.o *.i *.asm *.elf
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
