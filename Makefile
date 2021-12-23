PREFIX=/usr/
BIN_DIR=bin/
SHARE_DIR=share/
PROGRAM=e4bible
BIBLE=bible.sqlite3
CONFIG_DIR=$(PREFIX)$(SHARE_DIR)$(PROGRAM)

CC=gcc
#CC=afl-gcc-fast
CFLAGS=-Wall -g -pthread 
LDFLAGS=-ldl -lm 

FILES = $(PROGRAM).c sqlite/sqlite3.c -o $(PROGRAM)

all:
	$(CC) $(CFLAGS) $(FILES) $(LDFLAGS)
	@echo "Generating config.h"
	@echo "#define CONFIG_DIR \"$(CONFIG_DIR)\"" > config.h

clean:
	rm -f $(PROGRAM)

install:
	sudo rm -f $(PREFIX)$(BIN_DIR)$(PROGRAM)
	sudo rm -rf $(CONFIG_DIR)
	sudo install -d $(CONFIG_DIR)
	sudo install $(PROGRAM) $(PREFIX)$(BIN_DIR)
	sudo install $(BIBLE) $(CONFIG_DIR)

uninstall:
	sudo rm -f $(PREFIX)$(BIN_DIR)$(PROGRAM)
	sudo rm -rf $(CONFIG_DIR)
