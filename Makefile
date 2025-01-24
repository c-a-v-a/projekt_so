CC=gcc
CFLAGS=-Wall -Wextra -g -std=c11 -Wpedantic -DENV_DEV -D_POSIX_C_SOURCE
PROD_CFLAGS=-Wall -Wextra -O2 -std=c11 -Wpedantic
FMT=clang-format

OBJDIR=./obj
BINDIR=./bin
SRCDIR=./src

_OBJ_FILES=logger.o cli_parser.o ipc_wrapper.o
OBJ_FILES=$(patsubst %,$(OBJDIR)/%,$(_OBJ_FILES))

all: main student dean board

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/**/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJDIR)/main.o $(OBJ_FILES)
	make setup
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) -lm

student: $(OBJDIR)/student.o $(OBJ_FILES)
	make setup
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS)

dean: $(OBJDIR)/dean.o $(OBJ_FILES)
	make setup
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS)

board: $(OBJDIR)/board.o $(OBJ_FILES)
	make setup
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) -pthread

format:
	$(FMT) --style=google -i $(SRCDIR)/*.*
	$(FMT) --style=google -i $(SRCDIR)/**/*.*

setup:
	[ -d $(BINDIR) ] || mkdir $(BINDIR)
	[ -d $(OBJDIR) ] || mkdir $(OBJDIR)

clean:
	[ -d $(OBJDIR) ] && rm $(OBJDIR)/*
	[ -d $(BINDIR) ] && rm $(BINDIR)/*
