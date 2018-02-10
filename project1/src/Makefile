#
# @file memory.h
# @brief contains functions for allocating, freeing, and copying memory
#
# this makefile implements build instructions for three systems
#
# @author Seth Miers and Jake Cazden
# @date February 11, 2017
#
#
LINUXCC = gcc
#BBBCC =
#FSCC =

INC_DIR = ../include/
INC_COMMON = ../include/common
INC_CMSIS = ../include/CMSIS
INC_KL25Z = ../include/kl25z
INC = $(INC_DIR) $(INC_COMMON) $(INC_CMSIS) $(INC_KL25Z)
INC_PARAMS=$(foreach d, $(INC), -I$d)

# compile to c89 standard
# show all warnings
# show all errors
# stop compilation on first error
# no optimization
# define PROJECT1
# compile with debugger support
# include directory
LINUXFLAGS = -std=c89 -I../include/ -I../include/common/ -Wall -Werror -Wfatal-errors -O0 -D PROJECT1 -D VERBOSE
#			-g \
#			-I$(INC_DIR)

#BBBFLAGS = -std=c89
#FSFLAGS = -std=c89

# include source files
include sources.mk

LINUXFILES = $(SRCS)
#BBBFILES
#FSFILES =

#ifeq ($(PLATFORM), KL25Z)
#	echo Building for platform: KL25Z
#endif

LINUXOUTPUT = main.out
#BBBOUTPUT =
#FSOUTPUT =


linux: $(LINUXFILES)
	$(LINUXCC) $(LINUXFLAGS) $(LINUXFILES) -o $(LINUXOUTPUT)
#bbb: $(BBBFILES)
#	$(BBBCC) $(BBBFLAGS) $(SRC) $(BBBFILES) -o $(BBBOUTPUT)
#fs: $(FILES)
#	$(FSCC) $(FSFLAGS) $(SRC) $(FSFILES) -o $(FSOUTPUT)

#all: $(FILES)
#	$(CC) $(CFLAGS) $(FILES) -o $(OUTPUT)

#target: prereq1 prereq2 prereq3
#	command 1
#	command 2
