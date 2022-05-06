

action: comp run

comp:
	clear
	gcc base64.c -o smd

run:
	./smd

clean:
	clear
	rm smd
