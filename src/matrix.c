#include <stdio.h>
#include <stdlib.h>
#include "../headers/matrix.h"

/*The @requires / @assings / @ensures of the following 
functions can be found in the matrix.h file. */

typedef unsigned char** matrix;

matrix create_matrix(int N, int P){

    matrix Mat;
    Mat = (unsigned char **) malloc(N * sizeof(unsigned char*));
    int i;
    for (i=0; i<P; i++){
        Mat[i] = (unsigned char*) malloc(P * sizeof(unsigned char));
    }
    
    return Mat;
}

void display_matrix(matrix M, int N, int P){
    int i,j;
    for (i=0; i<N; i++){
        printf("Line %i : ", i);
        for (j=0; j<P; j++){
            printf("%3d,",M[i][j]);
        }
        printf("\n");
    }
}

void free_matrix(matrix M, int N){
    int i;
    for (i=0; i<N; i++){
        free(M[i]);
    }
    free(M);
}
