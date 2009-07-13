
# uncomment this to print debug information
#DEBUG = y


# uncomment this to use libncurses
USE_CURSES = y

ifeq ($(DEBUG),y)
CFLAGS += -DDEBUG
endif

ifeq ($(USE_CURSES),y)
CFLAGS += -DCURSES
endif

all : hangman1.o hangman2.o player1.o player2.o 
	gcc $(CFLAGS) -o hang1 hangman1.o player1.o player2.o -lncurses
	gcc $(CFLAGS) -o hang2 hangman2.o player1.o player2.o -lncurses

hangman1.o : hangman1.c
	gcc $(CFLAGS) -c hangman1.c

hangman2.o : hangman2.c
	gcc $(CFLAGS) -c hangman2.c

player1.o : player1.c
	gcc $(CFLAGS) -c player1.c

player2.o : player2.c
	gcc $(CFLAGS) -c player2.c

clean : 
	rm -f hang1 hang2 *.o

