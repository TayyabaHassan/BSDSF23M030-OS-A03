# Simple Makefile for the starter shell
CC := gcc
CFLAGS := -Wall -Wextra -g -Iinclude
SRCDIR := src
OBJDIR := obj
BINDIR := bin
TARGET := $(BINDIR)/psh

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

.PHONY: all clean

all: $(TARGET)

# ensure obj and bin directories exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# compile .c -> .o
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# link
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(TARGET)
