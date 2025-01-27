CC=gcc
CFLAGS=-Wall -Wextra -g -std=c11 -Wpedantic -DENV_DEV -D_POSIX_C_SOURCE -D_DEFAULT_SOURCE
PROD_CFLAGS=-Wall -Wextra -O2 -std=c11 -Wpedantic -D_POSIX_C_SOURCE -D_DEFAULT_SOURCE
FMT=clang-format

OBJDIR=./obj
BINDIR=./bin
SRCDIR=./src

_OBJ_FILES=logger.o cli_parser.o ipc_wrapper.o linked_list.o grades.o
OBJ_FILES=$(patsubst %,$(OBJDIR)/%,$(_OBJ_FILES))

all: setup main student dean board
dev_all: setup dev_main dev_student dev_dean dev_board

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) -c -o $@ $< $(PROD_CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/**/%.c
	$(CC) -c -o $@ $< $(PROD_CFLAGS)

main: $(OBJDIR)/main.o $(OBJ_FILES)
	$(CC) -o $(BINDIR)/$@ $^ $(PROD_CFLAGS) -lm -pthread

student: $(OBJDIR)/student.o $(OBJ_FILES)
	$(CC) -o $(BINDIR)/$@ $^ $(PROD_CFLAGS)

dean: $(OBJDIR)/dean.o $(OBJ_FILES)
	$(CC) -o $(BINDIR)/$@ $^ $(PROD_CFLAGS)

board: $(OBJDIR)/board.o $(OBJ_FILES)
	$(CC) -o $(BINDIR)/$@ $^ $(PROD_CFLAGS) -pthread

format:
	$(FMT) --style=google -i $(SRCDIR)/*.*
	$(FMT) --style=google -i $(SRCDIR)/**/*.*

setup:
	[ -d $(BINDIR) ] || mkdir $(BINDIR)
	[ -d $(OBJDIR) ] || mkdir $(OBJDIR)

clean:
	[ -d $(OBJDIR) ] && rm $(OBJDIR)/*
	[ -d $(BINDIR) ] && rm $(BINDIR)/*
