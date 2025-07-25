all:
	gcc -Wall -Werror -fsanitize=address trippy.c WGraph.c -o trippy

dcc:
	dcc trippy.c WGraph.c -o trippy

gcc:
	gcc trippy.c WGraph.c -o trippy	

clean:
	rm -f trippy
