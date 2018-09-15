/*
 * gurobi.c
 * this module uses gurobi ILP to solve and validate sudoku board
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"
#include "gurobi.h"
#include "game.h"

int opt; /* 1=boardSolvable, -1=error, 0=boardUnsolvable*/
int* ind=NULL;
double *val=NULL,*lb=NULL, *sol=NULL;
char* vtype=NULL;
double objval;
int optimstatus, error=0;
int N ,row , col;
int error;
GRBenv *env = NULL;
GRBmodel *model = NULL;

/*
 * this functions return 1 if the board is solvable and puts the solution in tempSol field in each cell in the board.
 * the function returns 0 if the board is unsolvable and -1 if an error occurred.
 */
int ILPSolver() {
	N =board.N;
	row = board.row;
	col = board.col;
	if (allocateMemory() == -1) { /*allocte memory for arrays used by gurobi*/
		return -1;
	}
	if (createEnvironment() == -1) { /*create environment*/
		return -1;
	}
	if (cancelPrints() == -1) { /*cancel gurobi consule prints*/
		return -1;
	}
	createEmptyModel(lb, vtype); /*creates an empty model*/
	if (createNewModel() == -1) { /* creates a new model*/
		return -1;
	}
	if (valueForEachCell() == -1) { /*add constraint that each cell gets a value*/
		return -1;
	}
	if (addRowConstr() == -1) { /*add constraint that each value appears once in each row*/
		return -1;
	}
	if (addColConstr() == -1) { /*add constraint that each value appears once in each col*/
		return -1;
	}
	if (addSubgridConstr()== -1) { /*add constraint that each value appears once in each subgrid*/
		return -1;
	}
	if (optimizeModel() == -1) { /*optimize model*/
		return -1;
	}
	if (writeModelToLog() == -1) { /* write model to log*/
		return -1;
	}

	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus); /*Capture solution information*/
	if (error) {
		printf(GUROBIERROR);
		freeGurobiResources(env, NULL, ind, val, lb, vtype, sol);
		return -1;
	}

	error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval); /*get objective from the function*/
	if (error) {
		printf(GUROBIERROR);
		freeGurobiResources(env, NULL, ind, val, lb, vtype, sol);
		return -1;
	}
	if (getSolution() == -1) { /* get solution */
		return -1;
	}
	copySolutionToBoard(); /* copy the solution to the board*/
	if (optimstatus == GRB_OPTIMAL) { /*check if the solution was successful or not*/
		opt = 1;
	} else if (optimstatus == GRB_INF_OR_UNBD) {
		opt = 0;
	} else {
		opt = -1;
	}
	freeGurobiResources(); /*free all resources used by gurobi*/
	return opt;
}


/*
 * allocate memroy for arrays used by gurobi
 * if and error occurs return -1
 * if all allocations were successful returns 0
 */
int allocateMemory(){
	ind = (int*)malloc(board.N*sizeof(int));
		if (ind==NULL){
			printf(CALLOC);
			return -1;
		}

	val = (double*) malloc(board.N * sizeof(double));
	if (val == NULL ) {
		printf(CALLOC);
		return -1;
	}

	lb = (double*) malloc((board.N * board.N * board.N) * sizeof(double));
	if (lb == NULL ) {
		printf(CALLOC);
		return -1;
	}
	vtype = (char*) malloc((board.N * board.N * board.N) * sizeof(char));
	if (vtype == NULL ) {
		printf(CALLOC);
		return -1;
	}
	sol = (double*) malloc((board.N * board.N * board.N) * sizeof(double));
	if (sol == NULL ) {
		printf(CALLOC);
		return -1;
	}
	return 0;
}

/* create environment
 * return -1 if error occured
 */
int createEnvironment(){
	int error;
	error = GRBloadenv(&env, "sudoku.log");
	if (error) {
		printf(GUROBIERROR);
		freeGurobiResources();
		return -1;
	}
	return 0;
}

/* cancel print outs by gurobi
 * return -1 if error occured
 */
int cancelPrints(){
	int error;
	error =GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error){
		printf(GUROBIERROR);
		freeGurobiResources();
		return -1;
	}
	return 0;
}

/*
 * creates an empty model
 */
void createEmptyModel(){
	int i, j, v;
	for (i = 0; i < board.N; i++) {
		for (j = 0; j < board.N; j++) {
			for (v = 0; v < board.N; v++) {
				if (board.gameBoard[i][j].value == v + 1)
					lb[i * board.N * board.N + j * board.N + v] = 1;
				else
					lb[i * board.N * board.N + j * board.N + v] = 0;
				vtype[i * board.N * board.N + j * board.N + v] = GRB_BINARY;
			}
		}
	}
}
/*
 * create a new model
 * return -1 if error occured
 */
int createNewModel(){
	int error;
	error = GRBnewmodel(env, &model, "sudoku", board.N * board.N * board.N, NULL, lb, NULL, vtype, NULL );
	if (error) {
		printf(GUROBIERROR);
		freeGurobiResources();
		return -1;
	}
	return 0;
}
/*
 * add constrain that each cell get a value
 */
int valueForEachCell(){
	int i,j,v;
	int error;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (v = 0; v < N; v++) {
				ind[v] = i * N * N + j * N + v;
				val[v] = 1.0;
			}
			error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL );
			if (error) {
				printf(GUROBIERROR);
				freeGurobiResources();
				return -1;
			}
		}
	}
	return 0;
}

/*
 * each value may appear once in each row
 */
int addRowConstr(){
	int i,j,v, error;
	for (v = 0; v < N; v++) {
		for (j = 0; j < N; j++) {
			for (i = 0; i < N; i++) {
				ind[i] = i * N * N + j * N + v;
				val[i] = 1.0;
			}
			error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL );
			if (error) {
				printf(GUROBIERROR);
				freeGurobiResources();
				return -1;
			}
		}
	}
	return 0;
}

/*
 * each value may appear once in each col
 */
int addColConstr(){
	int i,j,v,error;
	for (v = 0; v < N; v++) {
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				ind[j] = i * N * N + j * N + v;
				val[j] = 1.0;
			}

			error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL );
			if (error) {
				printf(GUROBIERROR);
				freeGurobiResources();
				return -1;
			}
		}
	}
	return 0;
}
/*
 * each value may appear once in each subgrid
 */
int addSubgridConstr(){
	int v,x,y,error,count,i,j;
	for (v = 0; v < N; v++) {
		for (x = 0; x < row; x++) {
			for (y = 0; y < col; y++) {
				count = 0;
				for (i = x * col; i < (x + 1) * col; i++) {
					for (j = y * row; j < (y + 1) * row; j++) {
						ind[count] = j * N * N + i * N + v;
						val[count] = 1.0;
						count++;
					}
				}
				error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL );
				if (error) {
					printf(GUROBIERROR);
					freeGurobiResources();
					return -1;
				}
			}
		}
	}
	return 0;
}

/*
 * optimize model
 */
int optimizeModel(){
	int error;
	error = GRBoptimize(model);
	if (error) {
		printf(GUROBIERROR);
		freeGurobiResources();
		return -1;
	}
	return 0;
}

/*
 * write model to log
 */
int writeModelToLog(){
	int error;
	error = GRBwrite(model, "sudoku.lp");
	if (error) {
		printf(GUROBIERROR);
		freeGurobiResources();
		return -1;
	}
	return 0;
}

/*
 * get solution
 */
int getSolution(){
	int error;
	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0,
			board.N * board.N * board.N, sol);
	if (error) {
		printf(GUROBIERROR);
		freeGurobiResources();
		return -1;
	}
	return 0;
}

void copySolutionToBoard(){
	int i, j, v;
	for (i = 0; i < board.N; i++) {
		for (j = 0; j < board.N; j++) {
			for (v = 0; v < board.N; v++) {
				if (sol[i * board.N * board.N + j * board.N + v] == 1) {
					board.gameBoard[i][j].tempSol = v + 1;
				}
			}
		}
	}
}

/*
 * free resources allocated
 */
void freeGurobiResources(){
	if (model!=NULL){
			GRBfreemodel(model);
	}
	if (env!=NULL){
		GRBfreeenv(env);
	}

	free(ind);
	free(val);
	free(lb);
	free(vtype);
	free(sol);
}
