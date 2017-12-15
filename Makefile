

PROG=SuperMastermind

OBJS=main.o secret.o game.o board.o

all: $(PROG)

$(PROG): $(OBJS)
	gcc -o $@ $^ -lSDL

%.o: %.c
	gcc -c $<

.PHONY: clean mrproper

clean:
	rm -f *.o

mrproper: clean
	rm -f $(PROG)

