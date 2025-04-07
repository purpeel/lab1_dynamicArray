.PHONY: all run clean 

head := $(wildcard inc/*.h)
src := $(wildcard src/*.c)
obj := $(src:%.h=%.o)
flags := -Wall -Wextra


all: launch

run:
	./lab1.exe

clean:
	rm -rf *.exe *.o lab1

%.o: %.c $(head)
	gcc -c -o $@ $<

launch: $(obj) 
	gcc -o lab1 $^ $(flags)