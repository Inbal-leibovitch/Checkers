/*
 * gurobi.h
 * this module uses gurobi to solve a sudoku board of any size
 */

#ifndef GUROBI_H_
#define GUROBI_H_

#define GUROBIERROR "Error: gurobi error\n"
#include "gurobi_c.h"

/* returns 1 if the board is solvable, 0 if unsolvable, -1 if error occured. if board is solvable, solution is in tempSol field.*/
int ILPSolver();
/* allocate memory used by the module*/
int allocateMemory();
/*create environment*/
int createEnvironment();
/*cancel consule prints by gurobi*/
int cancelPrints();
/*create an empty model*/
void createEmptyModel();
/*create a new model*/
int createNewModel();
/*add constraint that each cell has a value*/
int valueForEachCell();
/*add constraint that each value appears once in each row*/
int addRowConstr();
/*add constrain that each value appears once in each col*/
int addColConstr();
/*add constarint that each value appears once in each subgrid*/
int addSubgridConstr();
/*optimize model*/
int optimizeModel();
/*write model to log sudoku.lp*/
int writeModelToLog();
/*get the solution from the model*/
int getSolution();
/*copy solution to tempSol field in each cell in the board*/
void copySolutionToBoard();
/*free all resources used by gurobi and this module*/
void freeGurobiResources();

#endif /* GUROBI_H_ */
