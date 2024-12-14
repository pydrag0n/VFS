CC=gcc
CFLAGS=-Wall -g

O=out
OBJS = $(O)/src/file.o $(O)/src/user.o $(O)/src/vfs.o $(O)/src/include/logger/logger.o $(O)/cli.o

.PHONY: all clean
all: out $(O)/vfs.exe
	$(O)/vfs.exe

out:
	mkdir out

$(O)/vfs.exe: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(O)/vfs.exe

$(O)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
