CC=gcc
CFLAGS=-Wall -Wextra -g -std=c11 -Wpedantic -DENV_DEV -D_POSIX_C_SOURCE
PROD_CFLAGS=-Wall -Wextra -O2 -std=c11 -Wpedantic
FMT=clang-format

OBJDIR=./obj
BINDIR=./bin
SRCDIR=./src

_OBJ_MAIN=main.o defaults.o str_creator.o argprinter.o cli_parser.o ipc_wrapper.o
OBJ_MAIN=$(patsubst %,$(OBJDIR)/%,$(_OBJ_MAIN))
_OBJ_STUDENT=student.o argparser.o argprinter.o argvalidator.o defaults.o ipc_wrapper.o
OBJ_STUDENT=$(patsubst %,$(OBJDIR)/%,$(_OBJ_STUDENT))
_OBJ_DEAN=dean.o argparser.o argprinter.o argvalidator.o defaults.o ipc_wrapper.o
OBJ_DEAN=$(patsubst %,$(OBJDIR)/%,$(_OBJ_DEAN))
_OBJ_BOARD=board.o argparser.o argprinter.o argvalidator.o defaults.o ipc_wrapper.o
OBJ_BOARD=$(patsubst %,$(OBJDIR)/%,$(_OBJ_BOARD))

all: main student dean board

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/**/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ_MAIN)
	make setup
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) -lm

student: $(OBJ_STUDENT)
	make setup
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS)

dean: $(OBJ_DEAN)
	make setup
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS)

board: $(OBJ_BOARD)
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
