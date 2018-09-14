CC = gcc
OBJS = main.o parser.o game.o actions.o gurobi.o list.o stack.o mainUtils.o
EXEC = sudoku-consule
COMP_FLAG = -ansi -Wall -Wextra \
-Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56


$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c game.h parser.h SPBufferset.h actions.h list.h stack.h gurobi_c.h gurobi.h mainUtils.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
mainUtils.o: mainUtils.c parser.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
game.o: game.c game.h parser.h   actions.h list.h stack.h gurobi_c.h gurobi.h mainUtils.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
parser.o: parser.c parser.h game.h   game.h actions.h list.h stack.h gurobi_c.h gurobi.h mainUtils.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
actions.o: actions.c actions.h game.h   game.h parser.h list.h stack.h gurobi_c.h gurobi.h mainUtils.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
gurobi.o: gurobi.c gurobi.h game.h   game.h parser.h list.h stack.h gurobi_c.h actions.h mainUtils.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
list.o: list.c list.h game.h   game.h parser.h actions.h stack.h gurobi_c.h gurobi.h mainUtils.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
stack.o: stack.c stack.h game.h   game.h parser.h actions.h list.h gurobi_c.h gurobi.h mainUtils.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)




