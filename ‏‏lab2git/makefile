all: myshell looper mypipe

looper: looper.o
	gcc -m32 -g -Wall -o looper looper.o

looper.o: looper.c
	gcc -g -m32 -Wall -c looper.c

mypipe: mypipe.o
	gcc -m32 -g -Wall -o mypipe mypipe.o

mypipe.o: mypipe.c
	gcc -g -m32 -Wall -c mypipe.c

myshell : clean myshell.o LineParser.o 
	gcc -m32 -g -Wall -o myshell myshell.o LineParser.o 
	
myshell.o :myshell.c 
	gcc -g -m32 -Wall -c myshell.c 

LineParser.o : LineParser.c
	gcc -g -m32 -Wall -c LineParser.c

clean :
	rm -f *.o myshell looper mypipe
