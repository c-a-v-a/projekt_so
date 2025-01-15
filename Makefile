CC=gcc
CFLAGS=-Wall -Wextra -g -std=c11 -Wpedantic -DENV_DEV
PROD_CFLAGS=-Wall -Wextra -O2 -std=c11 -Wpedantic
FMT=clang-format

OBJDIR=./obj
BINDIR=./bin
SRCDIR=./src

_OBJ_MAIN=main.o argparser.o defaults.o
OBJ_MAIN=$(patsubst %,$(OBJDIR)/%,$(_OBJ_MAIN))
_OBJ_STUDENT=student.o
OBJ_STUDENT=$(patsubst %,$(OBJDIR)/%,$(_OBJ_STUDENT))
_OBJ_DEAN=dean.o
OBJ_DEAN=$(patsubst %,$(OBJDIR)/%,$(_OBJ_DEAN))
_OBJ_BOARD=board.o
OBJ_BOARD=$(patsubst %,$(OBJDIR)/%,$(_OBJ_BOARD))

all: main student dean board

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/**/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ_MAIN)
	make setup
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS)

student: $(OBJ_STUDENT)
	make setup
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS)

dean: $(OBJ_DEAN)
	make setup
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS)

board: $(OBJ_BOARD)
	make setup
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS)

format:
	$(FMT) --style=google -i $(SRCDIR)/**/*.c
	$(FMT) --style=google -i $(SRCDIR)/*.h

setup:
	[ -d $(BINDIR) ] || mkdir $(BINDIR)
	[ -d $(OBJDIR) ] || mkdir $(OBJDIR)

clean:
	[ -d $(OBJDIR) ] && rm $(OBJDIR)/*
	[ -d $(BINDIR) ] && rm $(BINDIR)/*
