

PROG=SuperMastermind

OBJS=main.o secret.o

all: $(PROG)

$(PROG): $(OBJS)
	gcc -o $@ $^

%.o: %.c
	gcc -c $<

.PHONY: clean mrproper

clean:
	rm -f *.o

mrproper: clean
	rm -f $(PROG)



