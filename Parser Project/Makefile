all: parser

parser: front.o parser.o
	$(CC) -Wall front.o parser.o -o dcooke_parser	

.c.o:
	$(CC) -Wall -c $<


test: parser front.in
	./dcoooke_parser

clean:
	$(RM) parser.o front.o dcooke_parser
