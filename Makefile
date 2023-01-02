source:=$(wildcard *.c)
app: $(source)
	gcc $(source) -o app

run: app
