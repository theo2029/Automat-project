typedef unsigned char** matrix;

/*  @require size>0
    @assings a matrix whitch of size N*P
    @ensures a matrix of size N*P*/
matrix create_matrix(int N, int P);

/*  @requiers M is a valid matrix of size size
    @assings nothing
    @ensures free the allocated memory of M*/
void free_matrix(matrix M, int N);

/*  @requires M is a valid matrix of size NxP
    @assings nothing
    @ensures the dispay of M*/
void display_matrix(matrix M, int N, int P);