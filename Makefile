CC = g++
CFLAG = -Wall -Werror -g -std=c++20 -I$(IDIR)
LDFLAGS = -lstdc++fs -pthread -lssl -lcrypto # Add this if your compiler requires explicit linking for filesystem

BDIR = bin
ODIR = build
IDIR = include
SDIR = src

TARGET_RT = $(BDIR)/table

_OBJ_RT = rtable.o main.o
OBJ_RT = $(patsubst %, $(ODIR)/%, $(_OBJ_RT))
_DEPS_RT = rtable.hpp
DEPS_RT = $(patsubst %, $(IDIR)/%, $(_DEPS_RT))

.DEFAULT_GOAL := all

$(ODIR)/%.o: $(SDIR)/%.cpp | dir
	$(CC) -c -o $@ $< $(CFLAG)

$(TARGET_RT): $(OBJ_RT) | dir
	$(CC) -o $@ $^ $(CFLAG) $(LDFLAGS) 

all:  $(TARGET_RT) 

.PHONY: all dir clean

dir:
	@mkdir -p $(BDIR) $(ODIR)

clean:
	rm -rf $(ODIR)/* $(BDIR)/* core