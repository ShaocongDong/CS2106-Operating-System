CC = gcc
CCOPTS = -O3 -Wall
DEPS = db.h
%.o: %.c $(DEPS)
	$(CC) $(CCOPTS) -c -o $@ $<

BINARIES = db.o
ALL = phonebook

all: $(ALL)

clean:
	rm -f $(ALL) $(BINARIES)
	rm -f phonebook.o

phonebook: phonebook.o $(BINARIES)
	$(CC) $(CCOPTS) $^ -o $@
