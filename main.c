#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>

struct node{
    int max;
    struct node* left_child;
    struct node* right_child;
};

struct nodeStack{
    struct node* top;
    struct nodeStack* rest;
};

struct node currentroot;
struct nodeStack stacktop;


char get_height(struct node root){
    if(root.left_child != NULL){
        return 1 + get_height(*root.left_child);
    } else if(root.right_child != NULL){
        return 1 + get_height(*root.right_child);
    } else {
        return 0;
    }
}

struct node* construct_tree(struct node root, u_int i, char h, int val){
    if (h == 0){

        struct node leaf = {val, NULL, NULL};
        struct node* node = malloc(sizeof(struct node));
        *node = leaf;
        return node;
    }

    if (i >> (h - 1 ) == 1){

        struct node rchild = {INT32_MIN};
        if (root.right_child != NULL){rchild = *root.right_child;}

        struct node tree = {INT32_MIN, root.left_child, construct_tree(rchild, i - (1 << (h - 1)), h - 1, val)};
        
        int maxval;
        int rmax = (*tree.right_child).max; 

        if(tree.left_child != NULL){
        int lmax = (*tree.left_child).max;
        maxval = (rmax > lmax ? rmax : lmax);
        } else {maxval = rmax;}

        tree.max = maxval;

        struct node* branch = malloc(sizeof(struct node));
        *branch = tree;
        return branch;

    } else {

        struct node lchild = {INT32_MIN};
        if (root.left_child != NULL){lchild = *root.left_child;} 

        struct node tree = {INT32_MIN, construct_tree(lchild, i - (1 << (h - 1)), h - 1, val), root.right_child};

        int maxval;
        int lmax = (*tree.left_child).max; 

        if(tree.right_child != NULL){
        int rmax = (*tree.right_child).max;
        maxval = (lmax > rmax ? lmax : rmax);
        } else {maxval = lmax;}

        tree.max = maxval;

        struct node* branch = malloc(sizeof(struct node));
        *branch = tree;
        return branch;
    }
}

char get_bits(u_int b){
    u_int val = b;
    char shift = 0;
    for (u_int bit = val; bit > 1; bit = val >> shift++);
    return shift;
}

void set(struct node root, u_int i, int val){
    char h = get_height(root);
    char b = get_bits(i);
    char new_height;
    if (h>b) {new_height = h;} else {new_height = b;}
    currentroot = *construct_tree(root, i, new_height, val); 
}

int find(struct node root, u_int i, char h){
    if (h == 0){
        return root.max;
    }

    if ((i >> (h - 1 ) == 1) && (root.right_child != NULL)){
        find(*root.right_child, i - (1 << (h - 1)), h - 1);
    } else if ((i >> (h - 1 ) == 0) && (root.left_child != NULL)){
        find(*root.left_child, i, h - 1);
    } else {return 0;}
}

int get(struct node root, u_int i){
    char h = get_height(root);
    char b = get_bits(i);
    if (b > h){return 0;}
    
    return find(root, i, h);

}

void maxinterval(struct node a, u_int l_bound, u_int r_bound){
}

int main(){

    int rootmax;
    char p;
    int i;

    printf("inserting %d at index %d \n", 100,4);
    set(currentroot, 4, 100);
    p = get_height(currentroot);
    printf("current height is: %d\n", p);
    rootmax = currentroot.max;
    printf("max in Tree is: %d\n", rootmax);
    i = get(currentroot, 4);
    printf("current value at %d is: %d\n",4,i);

    printf("inserting %d at index %d \n", 120,6);
    set(currentroot, 6, 120);
    p = get_height(currentroot);
    printf("current height is: %d\n", p);
    rootmax = currentroot.max;
    printf("max in Tree is: %d\n", rootmax);
    i = get(currentroot, 6);
    printf("current value at %d is: %d\n",6,i);

    printf("inserting %d at index %d \n", 10,4);
    set(currentroot, 4, 10);
    p = get_height(currentroot);
    printf("current height is: %d\n", p);
    rootmax = currentroot.max;
    printf("max in Tree is: %d\n", rootmax);
    i = get(currentroot, 4);
    printf("current value at %d is: %d\n",4,i);

    i = get(currentroot, 6);
    printf("current value at %d is: %d\n",6,i);
    return 0;
}
