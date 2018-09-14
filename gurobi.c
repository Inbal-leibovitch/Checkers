#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"
#include "gurobi.h"
#include "game.h"
#include "actions.h"

/*
 * return 1 if board is solvable
 * and put solution in tempSol
 */
int ILPSolver() {
	int N = board.N;
	int row = board.row;
	int col = board.col;
	GRBenv *env = NULL;
	GRBmodel *model = NULL;
	int opt; /* 1=boardSolvable, -1=error, 0=boardUnsolvable

	int* ind;
	double* val;
	double *lb;
	char* vtype;
	double* sol;

	int optimstatus;
	double objval;
	int i, j, v, x, y, count;
	int error = 0;

	ind = (int*)malloc(N*sizeof(int));
	if (ind==NULL){
		printf(CALLOC);
		return -1;
	}

	val = (double*)malloc(N*sizeof(double));
	if(val == NULL){
		printf(CALLOC);
		return -1;
	}

	lb = (double*)malloc((N*N*N)*sizeof(double));
	if(lb == NULL){
		printf(CALLOC);
		return -1;
	}
	vtype = (char*)malloc((N*N*N)*sizeof(char));
	if(vtype == NULL){
		printf(CALLOC);
		return -1;
	}
	sol = (double*)malloc((N*N*N)*sizeof(double));
	if(sol == NULL){
		printf(CALLOC);
		return -1;
	}



	/* Create environment*/

	error = GRBloadenv(&env, "sudoku.log");
	if (error) {
		printf(GUROBIERROR);
		 /*Free model*/
		GRBfreemodel(model);
		 /*Free environment*/
		GRBfreeenv(env);
		free(ind);
		free(val);
		free(lb);
		free(vtype);
		free(sol);
		return -1;
	}

	error =GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
		if (error){
			printf(GUROBIERROR);
			 /*Free model*/
			GRBfreemodel(model);
			 /*Free environment*/
			GRBfreeenv(env);
			free(ind);
			free(val);
			free(lb);
			free(vtype);
			free(sol);
			return -1;
		}

	/* Create an empty model*/

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (v = 0; v < N; v++) {
				if (board.gameBoard[i][j].value == v + 1)
					lb[i * N * N + j * N + v] = 1;
				else
					lb[i * N * N + j * N + v] = 0;
				vtype[i * N * N + j * N + v] = GRB_BINARY;
			}
		}
	}

/*	 Create new model*/

	error = GRBnewmodel(env, &model, "sudoku", N * N * N, NULL, lb, NULL, vtype,
			NULL );
	if (error) {
		printf(GUROBIERROR);
		 /*Free model*/
		GRBfreemodel(model);
		 /*Free environment*/
		GRBfreeenv(env);
		free(ind);
		free(val);
		free(lb);
		free(vtype);
		free(sol);
		return -1;
	}
/*
	 Each cell gets a value*/

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (v = 0; v < N; v++) {
				ind[v] = i * N * N + j * N + v;
				val[v] = 1.0;
			}

			error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL );
			if (error) {
				printf(GUROBIERROR);
				 /*Free model*/
				GRBfreemodel(model);
				 /*Free environment*/
				GRBfreeenv(env);
				free(ind);
				free(val);
				free(lb);
				free(vtype);
				free(sol);
				return -1;
			}
		}
	}

	 /*Each value must appear once in each row*/

	for (v = 0; v < N; v++) {
		for (j = 0; j < N; j++) {
			for (i = 0; i < N; i++) {
				ind[i] = i * N * N + j * N + v;
				val[i] = 1.0;
			}

			error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL );
			if (error) {
				printf(GUROBIERROR);
				 /*Free model*/
				GRBfreemodel(model);
				 /*Free environment*/
				GRBfreeenv(env);
				free(ind);
				free(val);
				free(lb);
				free(vtype);
				free(sol);
				return -1;
			}
		}
	}

/*	 Each value must appear once in each column*/

	for (v = 0; v < N; v++) {
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				ind[j] = i * N * N + j * N + v;
				val[j] = 1.0;
			}

			error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL );
			if (error) {
				printf(GUROBIERROR);
				 /*Free model*/
				GRBfreemodel(model);
				 /*Free environment*/
				GRBfreeenv(env);
				free(ind);
				free(val);
				free(lb);
				free(vtype);
				free(sol);
				return -1;
			}
		}
	}

	 /*Each value must appear once in each subgrid*/

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
					 /*Free model*/
					GRBfreemodel(model);
					 /*Free environment*/
					GRBfreeenv(env);
					free(ind);
					free(val);
					free(lb);
					free(vtype);
					free(sol);
					return -1;
				}
			}
		}
	}

	/* Optimize model*/

	error = GRBoptimize(model);
	if (error) {
		printf(GUROBIERROR);
		 /*Free model*/
		GRBfreemodel(model);
		 /*Free environment*/
		GRBfreeenv(env);
		free(ind);
		free(val);
		free(lb);
		free(vtype);
		free(sol);
		return -1;
	}

	/* Write model to 'sudoku.lp'*/

	error = GRBwrite(model, "sudoku.lp");
	if (error) {
		printf(GUROBIERROR);
		 /*Free model*/
		GRBfreemodel(model);
		 /*Free environment*/
		GRBfreeenv(env);
		free(ind);
		free(val);
		free(lb);
		free(vtype);
		free(sol);
		return -1;
	}

	 /*Capture solution information*/
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) {
		printf(GUROBIERROR);
		 /*Free model*/
		GRBfreemodel(model);
		 /*Free environment*/
		GRBfreeenv(env);
		free(ind);
		free(val);
		free(lb);
		free(vtype);
		free(sol);
		return -1;
	}
	/*get objective from the function*/
	error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
	if (error) {
		printf(GUROBIERROR);
		 /*Free model*/
		GRBfreemodel(model);
		 /*Free environment*/
		GRBfreeenv(env);
		free(ind);
		free(val);
		free(lb);
		free(vtype);
		free(sol);
		return -1;
	}

	/*get solution*/
	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N * N * N, sol);
	if (error) {
		printf(GUROBIERROR);
		 /*Free model*/
		GRBfreemodel(model);
		 /*Free environment*/
		GRBfreeenv(env);
		free(ind);
		free(val);
		free(lb);
		free(vtype);
		free(sol);
		return -1;
	}

	/*copy solution to board (tempSol)*/
	for (i=0; i<N; i++){
		for (j=0; j<N; j++){
			for (v=0; v<N; v++){
				if (sol[i*N*N+j*N+v]==1){
					board.gameBoard[i][j].tempSol = v+1;
				}
			}
		}
	}



	/*printf("\nOptimization complete\n");
		if (optimstatus == GRB_OPTIMAL)
			printf("Optimal objective: %.4e\n", objval);
		else if (optimstatus == GRB_INF_OR_UNBD)
			printf("Model is infeasible or unbounded\n");
		else
			printf("Optimization was stopped early\n");
		printf("\n");*/

	if (optimstatus == GRB_OPTIMAL){
		opt = 1;
	}
	else if (optimstatus ==  GRB_INF_OR_UNBD){
		opt =0;
	}
	else {
		opt =-1;
	}
	 /*Free model*/
	GRBfreemodel(model);
	/* Free environment*/
	GRBfreeenv(env);
	free(ind);
	free(val);
	free(lb);
	free(vtype);
	free(sol);
	return opt;
}
