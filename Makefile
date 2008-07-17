all: demo

demo: main.c Makefile
	gcc -o $@ $< $(shell pkg-config --cflags --libs gtk+-2.0)

clean:
	rm -f demo

check:
install:
