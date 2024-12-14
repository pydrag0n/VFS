CC = gcc
CFLAGS = -Wall -Wextra -g
DEPFLAGS = -MT $@ -MMD -MP -MF $(O)/$*.d

O=out
OBJS = $(O)/file.o $(O)/user.o $(O)/vfs.o
OBJSTEST = $(O)/test.o $(O)/test2.o
DEPFILES = $(OBJS:%.o=%.d)
DEPFILES += $(OBJSTEST:%.o=%.d)
DEPFILES += $(O)/cli.d
LIBS = logger/out/log.a

.PHONY: all check
all: $(O) $(O)/vfs.exe
	$(O)/vfs.exe

check: $(O) $(LIBS) $(OBJS) $(OBJSTEST) $(O)/test.exe $(O)/test2.exe
	$(O)/test.exe
	$(O)/test2.exe

$(O)/vfs.exe: $(LIBS) $(OBJS) $(O)/cli.o
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(O)/cli.o -o $(O)/vfs.exe

$(O)/test.exe:
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(O)/test.o -o $(O)/test.exe

$(O)/test2.exe:
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(O)/test2.o -o $(O)/test2.exe

logger/out/log.a:
	make -C logger all

$(O):
	mkdir $@

$(O)/%.o : %.c $(O)/%.d
	$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@

$(DEPFILES):

include $(wildcard $(DEPFILES))
