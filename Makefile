all:
	gcc -Wall -Werror -fsanitize=address trippy.c WGraph.c queue.c stack.c -o trippy

dcc:
	dcc trippy.c WGraph.c queue.c stack.c -o trippy

gcc:
	gcc trippy.c WGraph.c queue.c stack.c -o trippy	

clean:
	rm -f trippy
