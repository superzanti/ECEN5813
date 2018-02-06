LINUXCC = gcc
#BBBCC =
#FSCC =

# compile to c89 standard
# show all warngings
# stop compilation on first error
# no optimization
# define PROJECT1
# define VERBOSE
LINUXFLAGS = -std=c89 \
			-Wall \
			-Wfatal-errors \
			-O0 \
			-D PROJECT1 \
			-D VERBOSE

#BBBFLAGS = -std=c89
#FSFLAGS = -std=c89

# include source files
include sources.mk

LINUXFILES = $(SRCS)
#BBBFILES
#FSFILES =

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
