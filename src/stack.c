#include <stdio.h>
#include <stdlib.h>
#include "../headers/stack.h"

/*Les @requires / @assings / @ensures of the following 
functions can be found in the stack.h file. */

stack stack_init(){
    stack r = {
        .top = -1,
        .size = 1,
        .content = malloc(sizeof(int))
    };
    return r;
}

int is_empty(stack r){
    return r.top == -1;
}

int isFull(stack r){
    return r.size == r.top;
}

void modif(stack* r, int newSize){
    int* tmp = r->content;
    r -> content = malloc(newSize*sizeof(int));
    for (int i=0; i<r->size+1; i++)
        r -> content[i] = tmp[i];
    r->size = newSize;
    free(tmp);
}

void push(int elem, stack* r){
    if (isFull(*r)) //if the stack is full, size is doubled
        modif(r, r->size*2);
    r -> top += 1;
    r -> content[r->top] = elem;
}

int pop(stack* r){
    if (is_empty(*r)) return -100;
    int top = r->content[r->top];
    r->top -= 1;
    return top;    
}

int top_elem(stack r){
    return r.content[r.top];
}

void display_stack(stack r){
    for (int i=r.top; i>= 0; i--){
        if (i==0) printf("%i. ", r.content[i]);
        else printf("%i, ", r.content[i]);
    }
    printf("\n");
}
