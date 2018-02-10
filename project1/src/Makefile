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

ifeq ($(PLATFORM), HOST)
#	echo Building for platform: HOST
CC=gcc
CFILES=$(FILES)
CFLAGS =$(STD)\
		-I$(INC_DIR)\
		-I$(INC_COMMON)\
		-Wall\
		-Werror\
		-Wfatal-errors\
		-O0\
		-D PROJECT1\
		-D VERBOSE\
		-Wl,-Map=project1.map
OUTPUT=project1.elf
endif
ifeq ($(PLATFORM), BBB)
#	echo Building for platform: BBB
CC=
CFILES=$(FILES)
CFLAGS = $(STD)\
		 -I$(INC_DIR)\
		 -I$(INC_COMMON)\
		 -Wall\
		 -Werror\
		 -Wfatal-errors\
		 -O0\
		 -D PROJECT1\
		 -D VERBOSE\
		 -Wl,-Map=project1.map
OUTPUT=project1.elf
endif
ifeq ($(PLATFORM), KL25Z)
#	echo Building for platform: KL25Z
CC=
CFILES=$(FILES)
CFLAGS = $(STD)\
		 -I$(INC_DIR)\
		 -I$(INC_COMMON)\
		 -Wall\
		 -Werror\
		 -Wfatal-errors\
		 -O0\
		 -D PROJECT1\
		 -D VERBOSE\
		 -Wl,-Map=project1.map
OUTPUT=project1.elf
endif


.PHONY: native
native: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $(OUTPUT)

%.i: %.c
	$(CC) $(CFLAGS) -E -o $@ $^

%.asm: %.i
	$(CC) $(CFLAGS) -S -o $@ $^

%.o: %.asm
	$(CC) $(CFLAGS) -c -o $@ $^

.PHONY: compile-all
compile-all: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $(OUTPUT)

.PHONY: build
build: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUTPUT)

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
