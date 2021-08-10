#include <stdlib.h>
#include <stdio.h>

#include "stack.h"

struct stack {
    int height;
    int content[STACK_SIZE];
    int pushes;
    int pops;
    int maxSize;
};

struct stack *stack_init() {
    struct stack *s = (struct list *) malloc(sizeof(struct stack));

    if(!s){
        return NULL;
    }

    s->height = 0;
    s->pushes = 0;
    s->pops = 0;
    s->maxSize = 0;
    return s;
}

void stack_cleanup(struct stack *s) {

    fprintf(stderr,"stats %d %d %d\n",s->pushes,s->pops,s->maxSize);

    free(s);
}

int stack_push(struct stack *s, int e) {
    if(s == NULL){
        return 1;
    }

    if(s->height < STACK_SIZE){
        s->content[s->height] = e;
        s->height++;
        s->pushes++;
        if(s->height > s->maxSize){
            s->maxSize = s->height;
        }
        return 0;
    }
    return 1;
}

int stack_pop(struct stack *s) {
    if(s == NULL){
        return -1;
    }

    if(s->height != 0){
        s->height--;
        s->pops++;
        int toreturn = s->content[s->height];
        return toreturn;
    }
    return -1;
}

int stack_peek(struct stack *s) {
    if(s == NULL){
        return -1;
    }

    if(s->height != 0){
        return s->content[s->height - 1];
    }
    return -1;
}

int stack_empty(struct stack *s) {
    if(s == NULL){
        return -1;
    }

    if(s->height == 0){
        return 1;
    }

    if(s->height > 0){
        return 0;
    }

    return -1;
}