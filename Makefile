TARGET = bin/encode
SRC = $(wildcard main/*.c)
OBJ = $(patsubst main/%.c, objs/%.o, $(SRC))

$(TARGET): $(OBJ)
	gcc -o $(TARGET) $(OBJ)

objs/%.o: main/%.c
	gcc -Iincludes -o $@ -c $<

clean_objs:
	rm -f objs/*.o
	
clean_bin:
	rm bin/encode
