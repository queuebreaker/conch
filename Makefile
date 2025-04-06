# conch - c interpreter/shell for linux

CC := gcc
CFLAGS := -Wall -Wextra -g
CLIBS := -lreadline
SRC := conch.c
OBJ := ${SRC:.c=.o}
BIN := conch

# include config.make

all: $(BIN)

.c.o:
	$(CC) -c $(CFLAGS) $(CLIBS) $<

$(BIN): $(OBJ)
	$(CC) $(CLIBS) $(CFLAGS) $(OBJ) -o $(BIN)

$(OBJ): $(SRC)
	$(CC) $(CLIBS) $(CFLAGS) -c $(SRC)

clean:
	rm -rf $(OBJ) $(BIN)

install: all
	sudo mv $(BIN) /usr/local/bin/$(BIN)

uninstall:
	sudo rm $(BIN) /usr/local/bin/$(BIN)

test:
	echo $(SRC) 

.PHONY: all clean install uninstall
