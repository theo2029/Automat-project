#include <stdio.h>
#include <stdlib.h>
#include "../headers/stack.h"
#include "../headers/matrix.h"

/*  @requires fp as the automat format
    @assings an integer
    @returns the number of state of the automat*/
int state_number(FILE* fp){
    int n,i=0;
    unsigned char carac;
    char s[10];
    //The cursor is set in the position two, so as to avoid to read : "a_"
    fseek(fp, 2, SEEK_SET);
    carac = fgetc(fp);
    while(carac != '\n'){//terminating : Reading the entire first line
        s[i] = carac;
        carac = fgetc(fp);
        i++;
    }
    s[i] = '\0';
    n = atoi(s); //ascii to integer
    return n;
}   

/*  @requires s is a valid pointer
    @assings nothing
    @ensures return the len of s*/
int len_string(char * s){
    int i = 0;
    while (s[i]!='\0') i++;
    return i;
}

/*  @requires fp is a pointer to an automat file, n is the number of states of this automat
    @assings a matrix
    @ensures a matrix with the action of the automat.*/
matrix action_matrix(FILE* fp, int n){
    int i, N=n, P=128; //128 is the number of octets per lines
    matrix M = create_matrix(N,P); 
    for (i=0; i<N; i++)
        fread(M[i], sizeof(char), P, fp);
    return M;
}

/*  @requires fp is a valid automat file
    @assings nothing
    @ensures returns the length of first line*/
int len_first_line(FILE* fp){
    int i=0;
    char c = 0; //!=10
    fseek(fp, 0, SEEK_SET);
    while(c != '\n') { //terminatin : reading the first line
        c = fgetc(fp);//Incrementation of the cursor
        i++;
    }
    return i;
}

int lfl; //len of first line.

/*  @requires fp is a valid automat file, n is a number of states of this automat
    @assings a matrix
    @ensures returns R, the matrix (2 *n) of reduction, both composante*/
matrix reducted_matrix(FILE* fp, int n){
    lfl = len_first_line(fp);
    int saut = lfl + n * 128 + 1; //number of octet of the first line + actions + '\n'
    matrix R = create_matrix(2,n); //both reducting states
    fseek(fp, saut, SEEK_SET); //The cursor is now at the correct position
    for(int i=0; i<2; i++){
        for(int j=0; j<n; j++)
            R[i][j] = fgetc(fp);
        fseek(fp, 1, SEEK_CUR); //Skip '\n'
    }
    return R;
}

int len=-1;
int n_decale, n_branchement;

/*  @requires fp is a valid automat file, n is a number of states of this automat
    @assings a matrix
    @ensures returns D, matrix of decale (shift) numbers*/
matrix decale_matrix(FILE* fp, int n){
    int saut = lfl + n * 128 + 2*n + 3 ;
    /*  lfl : len of first line
        128 * n : size of action(s,c) par
        2*n : reduce() part
        3 : 3 times '\n'*/
    fseek(fp, saut, SEEK_SET);
    int count = 0;
    char a,b,c;

    while (a != '\255' && b != '\255' && c != '\255'){ //terminating according automat file format
        a = fgetc(fp);
        b = fgetc(fp);
        c = fgetc(fp);
        count += 3;
    }
    count -=3;
    len = count; //len will be useful for the branchement matrix.
    n_decale = count/3;

    fseek(fp, saut, SEEK_SET);
    matrix D;
    D = create_matrix(n_decale,n_decale);

    for (int i=0; i<n_decale; i++){
            D[i][0] = fgetc(fp);
            D[i][1] = fgetc(fp);
            D[i][2] = fgetc(fp);
    }
    return D;
}

/*  @requires fp is a valid automat file, n is a number of states of this automat
    @assings a matrix
    @ensures returns D, matrix of branchement (links) numbers*/
matrix branchement_matrix(FILE* fp, int n){
    int saut = lfl + n*128 + 2*n + 3 + len + 3;
    /*  len : size of decale
        3 : size of '\255' '\255' '\255' */
    fseek(fp, saut, SEEK_SET);
    char a,b,c;
    int count = 0;

    while (a!='\255' && b!='\255' && c!='\255'){ //terminating according automat file format
        a = fgetc(fp);
        b = fgetc(fp);
        c = fgetc(fp);
        count+=3;
    }
    count -= 3;
    n_branchement = count/3;

    fseek(fp, saut, SEEK_SET);

    matrix B = create_matrix(n_branchement,n_branchement);
    for (int i=0; i<n_branchement; i++)
        for(int j=0; j<3; j++)
            B[i][j] = fgetc(fp);
    
    return B;
}

/*  @requires M is the matrix of decale octets, 
    @assings nothing
    @ensures returns decale(s,c)=s'*/
char decale(matrix M_decale, char s, char c){
    for(int i=0; i<n_decale; i++)
        if (M_decale[i][0] == s && M_decale[i][1] == c) return M_decale[i][2];
    return 0;
}

/*  @requires M is a matrix of branchement octets , 
    @assings nothing
    @ensures returns branchement(s,A)*/
char branchement(matrix M_branchement, char s, char A){
    for(int i=0; i<n_decale; i++)
        if (M_branchement[i][0] == s && M_branchement[i][1] == A) return M_branchement[i][2];
    return 0;
}

/*  @requires a buffer, the Matrix of action / reduction / decale / branchement with the proper size, int n number of states, stack* state tack
    @assings int i, s (state), dec, A, branch, len
    @ensures makes the reading of the automats works and returns Accepted if there is a match otherwise rejected.*/
int game_loop(char* buf, matrix M_action, matrix M_reduc, matrix M_decale,matrix M_branchement, stack* state_stack){

    int i=0, s=0, dec, A, branch; len = len_string(buf);
    char c,action;

    buf[len] = '\n';
    push(0,state_stack);

    while (1){ //Les conditions de sorties assurent la terminaisons de la boucle.
        c = buf[i];
        s = top_elem(*state_stack); //curent state
        action = M_action[s][c]; //value of the action, treated with a switch.
        switch (action){
            case 0 : //Rejeted
                printf("Error at character number %i : \'%c\'.\n",i+1,c);
                printf("Rejected.\n");
                return 0;
                break;
            case 1 : //Accepted
                printf("Accepted.\n") ; 
                return 1;
                break;
            case 2 : //décale
                dec = decale(M_decale,s,c);
                push(dec,state_stack);
                i++; //next charac of the buffer
                break;
            case 3 : //Reduit(s) = (n,A)
                A = M_reduc[1][s];
                for (int i=0; i<M_reduc[0][s]; i++)
                    pop(state_stack); //n states are popped
                s = top_elem(*state_stack);//new current state
                branch = branchement(M_branchement,s,A);
                push(branch,state_stack); //On empile branchement(s',A)
            break;
        }       
    }
}

int main(int argc, char *argv[]){ 

    if (argc < 2){ //In case of no given file.
        printf("A file must be given.\n");
        return -1;
    }
    FILE* fp = fopen(argv[1],"rb"); //fp : file pointer

    //Defining useful constants:

    int n = state_number(fp);
    matrix M_action = action_matrix(fp,n);
    matrix M_reduc = reducted_matrix(fp,n);
    matrix M_decale = decale_matrix(fp, n);
    matrix M_branchement = branchement_matrix(fp,n);
    stack state_stack = stack_init();

    printf("File \"%s\" correctly read. Please enter your inputs.\n",argv[1]);

    char buf[256];
    fgets(buf, 256, stdin);
    sscanf(buf, "%s", buf);

    //Game section :
    while (1) {
        game_loop(buf, M_action,  M_reduc,  M_decale, M_branchement, &state_stack);
        fgets(buf, 256, stdin);
        sscanf(buf, "%s", buf);
    }

    return 0;
}