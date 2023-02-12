stuff.o: stuff.c stuff.h
	gcc -c stuff.c

themainprog.o: themainprog.c stuff.h
	gcc -c themainprog.c

pyramid: themainprog.o stuff.o
	gcc -o pyramid stuff.o themainprog.o

clean:
	rm -f pyramid stuff.o themainprog.o