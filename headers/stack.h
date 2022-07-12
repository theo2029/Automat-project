struct stack{
    int top;
    int size;
    int* content;
};

typedef struct stack stack;

/*  @requires nothing
    @assings nothing
    @ensures returns an initialized stack with top = -1*/
stack stack_init();

/*  @requires r is a valid stack
    @assings nothing
    @ensures return 1 if r is empty otherwise 0*/
int is_empty(stack r);

/*  @requires *r is a valid pointer to a stack
    @assigns nothing
    @ensures pushes an element at the top of the list*/
void push(int elem, stack* r);

/*  @require r is a valid stack
    @assings nothing
    @ensures return 1 if r is full otherwise 0*/
int isFull(stack r);

/*  @requires nothing
    @assigns 
    @ensures double the size of the stack if it's full*/
void modif(stack* r, int newSize);

/*  @requires r is a non-empty stack
    @assings nothing
    @ensures returns the top element of r*/
int top_elem(stack r);

/*  @requires r is a not empty stack; *r is a valid pointer
    @assings nothing
    @ensures return last element of r and removes it from r*/
int pop(stack* r);

/*  @requires r is a valid stack
    @assings nothing
    @ensures display the stack r from top to bottom*/
void display_stack(stack r);