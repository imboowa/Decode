all:
	gcc -Iincludes main/main.c main/comp.c -o decode
	
clean:
	rm decode
