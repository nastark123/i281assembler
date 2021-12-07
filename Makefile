CC=gcc

INCDIR=./include

SRCDIR=./src
SRCFILES=$(wildcard $(SRCDIR)/*.c)
SRCBUILD=$(SRCDIR)/build
SRCOBJ=$(patsubst $(SRCDIR)/%.c, $(SRCBUILD)/%.o, $(SRCFILES))

TARGETDIR=./out

CFLAGS=-I$(INCDIR) -Wall -g
COFLAGS=-c
# flags for linking, currently not needed for this project
# LFLAGS

all: build


build: directories $(SRCOBJ)
	$(CC) $(SRCBUILD)/*.o -o $(TARGETDIR)/i281assembler
	@echo Build done


directories:
	mkdir -p $(SRCBUILD)
	mkdir -p $(TARGETDIR)
	@echo Directories created


clean:
	rm -f $(SRCBUILD)/*.o
	rm -f $(TARGETDIR)/*


$(SRCOBJ): $(SRCBUILD)/%.o: $(SRCDIR)/%.c
	@echo Compiling object $@...
	$(CC) $(CFLAGS) $(COFLAGS) -o $@ $<
	@echo Done