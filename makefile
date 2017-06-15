TARGET = disasm

CC = cc
CFLAGS = -Wall -std=c99
LIBS = -lc

SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

SOURCES := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(INCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@ 

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(CC) -o $(BINDIR)/$(TARGET) $(OBJECTS) $(CFLAGS) $(LIBS) 

.PHONY: clean

clean:
	rm $(OBJECTS) $(BINDIR)/$(TARGET) 
