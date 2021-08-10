

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

/* C files can be modified anywhere.
 * So you can change or remove these structs to suit your needs. */
struct tree {
    struct node *root;
};

struct node {
    int data;
    struct node *lhs;
    struct node *rhs;

    // ... SOME CODE MISSING HERE ...
};
typedef struct node node;

/* Unique id's for numbering nodes in dot format. */
static int global_node_counter = 0;

/* Helper function: Allocate a new tree node and initialise it with
 * the given parameters. Return a pointer to the new node or NULL on
 * failure. */
static node *make_node(int data) {
    node *n = malloc(sizeof(node));

    if(n){
        n->data = data;
        n->lhs = NULL;
        n->rhs = NULL;
        return n;
    }

    return NULL;
}

static int print_tree_dot_r(node *root, FILE *dotf) {
    int left_id, right_id, my_id = global_node_counter++;

    if (root == NULL) {
        fprintf(dotf, "    %d [shape=point];\n", my_id);
        return my_id;
    }

    fprintf(dotf, "    %d [color=%s label=\"%d\"]\n", my_id, "black", root->data);

    left_id = print_tree_dot_r(root->lhs, dotf);
    fprintf(dotf, "    %d -> %d [label=\"l\"]\n", my_id, left_id);

    right_id = print_tree_dot_r(root->rhs, dotf);
    fprintf(dotf, "    %d -> %d [label=\"r\"]\n", my_id, right_id);

    return my_id;
}

void tree_dot(struct tree *tree, char *filename) {
    node *root = tree->root;
    global_node_counter = 0;
    FILE *dotf = fopen(filename, "w");
    if (!dotf) {
        printf("error opening file: %s\n", filename);
        exit(1);
    }
    fprintf(dotf, "digraph {\n");
    if (root) {
        print_tree_dot_r(root, dotf);
    }
    fprintf(dotf, "}\n");
    fclose(dotf);
}

int tree_check(struct tree *tree) {

    // ... SOME CODE MISSING HERE ...
}

struct tree *tree_init(int turbo) {
    struct tree *tree = malloc(sizeof(struct tree));

    if(tree){
        tree->root = NULL;
        return tree;
    }

    return NULL;
}

node* node_insert(node *n, int data){
    if(n == NULL){
        n = make_node(data);
        return n;
    }
    else if(data < n->data){
        n->lhs = node_insert(n->lhs, data);
    }
    else
    {
        n->rhs = node_insert(n->rhs, data);
    }

    return n;
}


int tree_insert(struct tree *tree, int data) {
    if(!tree->root){
        tree->root = make_node(data);
        return 0;
    }

    if(tree_find(tree, data) == 1){
        return 1;
    }

    node *n = tree->root;

    if(node_insert(n, data)){
        return 0;
    }

    return -1;
}

node* node_find(node *n, int data){
    if(n == NULL || n->data == data){
        return n;
    }
    else if(data < n->data){
        return node_find(n->lhs, data);
    }
    else{
        return node_find(n->rhs, data);
    }
}

int tree_find(struct tree *tree, int data) {
    node *n = tree->root;

    if(node_find(n, data)){
        return 1;
    }

    return 0;
}

node* max(node *n){
    node *cur = n;

    while(cur->rhs){
        cur = cur->rhs;
    }

    return cur;
}

node* node_remove(node *n, int data){
    if(data < n->data){
        n->lhs = node_remove(n->lhs, data);
    }
    else if(data > n->data){
        n->rhs = node_remove(n->rhs, data);
    }
    else{
        if(!n->lhs){
            node *temp = n->rhs;
            free(n);
            n = temp;
        }
        else if(!n->rhs){
            node *temp = n->lhs;
            free(n);
            n = temp;
        }
        else{
            node *p = max(n->lhs);
            n->data = p->data;
            n->lhs = node_remove(n->rhs, p->data);
        }
    }

    return n;
}

int tree_remove(struct tree *tree, int data) {
    if(tree_find(tree, data) == 1){
        node *n = tree->root;
        node_remove(n, data);
        return 0;
    }

    return 1;
}

void tree_print(struct tree *tree) {

    // ... SOME CODE MISSING HERE ...
}

void node_free(node *n){
    if(n->lhs){
        node_free(n->lhs);
    }

    if(n->rhs){
        node_free(n->rhs);
    }

    free(n);
}

void tree_cleanup(struct tree *tree) {
    node *n = tree->root;

    if(n){
        node_free(n);
    }

    free(tree);
}
