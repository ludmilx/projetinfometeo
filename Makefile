source:=$(wildcard *.c)
app: $(source)
	gcc $(source) -o app

run: app


# OBJ = main.o liste.o avl.o valeur.o

# %.o: %.c
#	gcc -c -o $@ $<

# main: $(OBJ)
#	gcc -o $@ $^ 
