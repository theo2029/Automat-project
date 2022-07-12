CC = gcc
CF = -Wall -Wextra -std=c99 #flags

OBJ_DIR=obj
HEADERS_DIR=headers
SOURCE_DIR=src

projet: ${OBJ_DIR}/main.o ${OBJ_DIR}/stack.o ${OBJ_DIR}/matrix.o
	${CC} -o $@ $^

${OBJ_DIR}/%.o: ${SOURCE_DIR}/%.c ${HEADERS_DIR}/%.h #A un .o on associe le .c correspondant
	${CC} ${CF} -g -o $@ -c $<

all: projet

clear:
	rm ${OBJ_DIR}/*
