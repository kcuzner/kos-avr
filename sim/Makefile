# Makefile for kos simulation
#
# The basic simulation is just a flashing led
#

BINDIR=../bin
OBJDIR=obj
SRCDIR=src
INCDIR=include

INCLUDES=-I$(INCDIR)
LIBS=simavr sdl2

CC=gcc
MKDIR=mkdir
RM=rm -rf

SRC=$(wildcard $(SRCDIR)/*.c)
OBJS:=$(addprefix $(OBJDIR)/,$(notdir $(SRC:.c=.o)))

CFLAGS=$(INCLUDES) -g
CFLAGS+=`pkg-config --cflags $(LIBS)`
LDFLAGS=`pkg-config --libs $(LIBS)` -lpthread

all:: $(BINDIR)/sim

clean:
	$(RM) $(BINDIR)/sim
	$(RM) $(OBJDIR)

$(BINDIR)/sim: $(OBJS)
	@$(MKDIR) -p $(dir $@)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(MKDIR) -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

