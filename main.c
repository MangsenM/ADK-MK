#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct node{
    const int max;
    const struct node* left_child;
    const struct node* right_child;
};


struct node_stack{
    struct node* head;
    struct node_stack* tail;
};
//set up root
const struct node nullnode;
struct node* ROOT = &nullnode;
struct node_stack* stack;

//height
/**
  *@brief Calculates hight of tree*
  *
  *@param node Root node of tree.
  **/

char get_height(const struct node* root){
    if((*root).left_child != NULL){
        return 1 + get_height((*root).left_child);
    } else if((*root).right_child != NULL){
        return 1 + get_height((*root).right_child);
    } else {
        return 0;
    }
}

struct node* extend_tree(const struct node* root, char l, struct node* t){
    if (l == 0){
        return root;
    }
    
    struct node* rc;
    struct node* lc = extend_tree(root, l - 1, &t[1]);
    
    struct node newnode = {(*root).max, lc, rc};
    return memcpy(t, &newnode, sizeof(struct node));
    
    
}

struct node* replace_leaf(const struct node* root, u_int i, char l, int val, struct node* t){
    if(root == NULL){root = &nullnode;}

    if (l == 0){
        struct node leaf = {val, NULL, NULL};
        return memcpy(t, &leaf, sizeof(struct node));
    }
    
    struct node* rc;
    struct node* lc;
    int max;
    
    if (i >> (l - 1 ) == 1){
      lc = (*root).left_child;
      rc = replace_leaf((*root).right_child, i - (1 << (l-1)), l-1, val, &t[1]);
      if (lc != NULL){
      max = (*lc).max > (*rc).max ? (*lc).max : (*rc).max;
      } else {max = (*rc).max;}
    }
    else {
      lc = replace_leaf((*root).left_child, i, l-1, val, &t[1]);
      rc = (*root).right_child;
      if (rc != NULL){
      max = (*lc).max > (*rc).max ? (*lc).max : (*rc).max;
      } else {max = (*lc).max;}
    }

    struct node newnode = {max, lc, rc};
    return memcpy(t, &newnode, sizeof(struct node));
}

char get_bits(u_int b){
    u_int val = b;
    char shift = 0;
    for (u_int bit = val; bit > 1; bit = val >> shift++);
    return shift;
}

void set(const struct node* root, u_int i, int val){
    char h0 = get_height(root);
    char b = get_bits(i);

    struct node_stack* newstack = malloc(sizeof(struct node_stack));
    *newstack = (struct node_stack){ROOT, stack};
    stack = newstack;

    if(b > h0){
    
    if(root != &nullnode){ 
    struct node* t = malloc(sizeof(struct node)*(2*b - h0));
    struct node* lc = extend_tree(root, b - h0 - 1, &t[b+2]);
    struct node* rc = replace_leaf(&nullnode, i, b, val, &t[1]);

    int max = (*lc).max > (*rc).max ? (*lc).max : (*rc).max;
    struct node newroot = {max, lc, rc};
    ROOT = memcpy(t, &newroot, sizeof(struct node));
    } else {
    struct node* t = malloc(sizeof(struct node)*(b));
    struct node* lc;
    struct node* rc = replace_leaf(&nullnode, i, b, val, &t[1]);
    struct node newroot = {(*rc).max, lc, rc};
    ROOT = memcpy(t, &newroot, sizeof(struct node));
    }

    } else {

    struct node* t = malloc(sizeof(struct node)*(h0 + 1));
    ROOT = replace_leaf(root, i, h0, val, t); 
    }
}

int find(const struct node* root, u_int i, char l){
    if (l == 0){
        return (*root).max;
    }

    if ((i >> (l - 1) == 1) && ((*root).right_child != NULL)){
        find((*root).right_child, i - (1 << (l - 1)), l - 1);
    } else if ((i >> (l - 1 ) == 0) && ((*root).left_child != NULL)){
        find((*root).left_child, i, l - 1);
    } else {return -1;}
}

int get(const struct node* root, u_int i){
    char h = get_height(root);
    char b = get_bits(i);
    if (b > h){return 0;}
    
    return find(root, i, h);
}

void unset(){
    if(stack == NULL){return;}
    free(ROOT);
    ROOT = (*stack).head;
    stack = (*stack).tail;
}

void maxinterval(struct node a, u_int l_bound, u_int r_bound){
}

int main(){

    int rootmax;
    char p;
    int i;
    char h;
    i = get(ROOT, 4);
    printf("current value at %d is: %d\n",4,i);
    printf("\n");

    printf("inserting %d at index %d \n", 100,4);
    set(ROOT, 4, 100);
    h = get_height(ROOT);
    printf("current height is: %d\n", h);
    rootmax = (*ROOT).max;
    printf("max in Tree is: %d\n", rootmax);
    i = get(ROOT, 4);
    printf("current value at %d is: %d\n",4,i);

    printf("\n");
    printf("inserting %d at index %d \n", 120,9);
    set(ROOT, 9, 120);
    h = get_height(ROOT);
    printf("current height is: %d\n", h);
    rootmax = (*ROOT).max;
    printf("max in Tree is: %d\n", rootmax);
    i = get(ROOT, 9);
    printf("current value at %d is: %d\n",9,i);
    i = get(ROOT, 4);
    printf("current value at %d is: %d\n",4,i);

    printf("\n");
    printf("inserting %d at index %d \n", 10,4);
    set(ROOT, 4, 10);
    h = get_height(ROOT);
    printf("current height is: %d\n", h);
    rootmax = (*ROOT).max;
    printf("max in Tree is: %d\n", rootmax);
    
    h = get_height(ROOT);
    printf("height = %d\n",h);
    i = get(ROOT, 4);
    printf("current value at %d is: %d\n",4,i);

    printf("\n");
    h = get_height(ROOT);
    printf("height = %d\n",h);
    i = get(ROOT, 9);
    printf("current value at %d is: %d\n",9,i);
    
    printf("\n");
printf("unset");
    printf("\n");
    unset();
    h = get_height(ROOT);
    printf("height = %d\n",h);
    i = get(ROOT, 4);
    printf("current value at %d is: %d\n",4,i);

    h = get_height(ROOT);
    printf("height = %d\n",h);
    i = get(ROOT, 9);
    printf("current value at %d is: %d\n",9,i);
    
    printf("\n");
printf("unset");
    printf("\n");
    unset();
    h = get_height(ROOT);
    printf("height = %d\n",h);
    i = get(ROOT, 4);
    printf("current value at %d is: %d\n",4,i);

    h = get_height(ROOT);
    printf("height = %d\n",h);
    i = get(ROOT, 9);
    printf("current value at %d is: %d\n",9,i);

    printf("\n");
printf("unset");
    printf("\n");
    unset();
    h = get_height(ROOT);
    printf("height = %d\n",h);
    i = get(ROOT, 4);
    printf("current value at %d is: %d\n",4,i);

    h = get_height(ROOT);
    printf("height = %d\n",h);
    i = get(ROOT, 9);
    printf("current value at %d is: %d\n",9,i);
    return 0;
}
