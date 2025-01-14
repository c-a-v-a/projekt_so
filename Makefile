CC=gcc
CFLAGS=-Wall -Wextra -g -std=c11 -Wpedantic
PROD_CFLAGS=-O2
FMT=clang-format

OBJDIR=./obj
BINDIR=./bin
SRCDIR=./src

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJDIR)/main.o
	make setup
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS)

format:
	$(FMT) --style=google -i src/*.c

setup:
	[ -d $(BINDIR) ] || mkdir $(BINDIR)
	[ -d $(OBJDIR) ] || mkdir $(OBJDIR)

clean:
	[ -d $(BINDIR) ] && rm $(BINDIR)/*
	[ -d $(OBJDIR) ] && rm $(OBJDIR)/*
