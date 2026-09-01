#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

struct node{
    int max;
    struct node* left_child;
    struct node* right_child;
};

struct nodeStack{
    struct node* top;
    struct nodeStack* rest;
};

struct node currentroot = {-1, NULL, NULL};
struct nodeStack stacktop = {NULL, NULL};


char get_height(struct node root){
    if(root.left_child != NULL){
       // printf("lc not null\n");
        return 1 + get_height(*root.left_child);
    } else if(root.right_child != NULL){
       // printf("rc not null\n");
        return 1 + get_height(*root.right_child);
    } else {
        return 0;
    }
}

struct node* construct_tree(struct node root, u_int i, char h, int val){
   // printf("%d",i);
   // printf("%d\n",h);
if (h == 0){
    struct node leaf = {val, NULL, NULL};
    struct node* node = malloc(sizeof(struct node));
    *node = leaf;
   // printf("basecase reached\n");
   // if (node != NULL){
     //   printf("node not null");
   // }
    //currentroot = *node;
   // char p = get_height(currentroot);
   // printf("%d", p);
    return node;
}
if (i >> (h - 1 ) == 1){
    struct node* lchild = NULL;
    if(root.left_child != NULL){
        lchild = root.left_child;
    }

    struct node rchild = {-1};
    if (root.right_child != NULL){
        rchild = *root.right_child;
    }

    struct node tree = {-1, lchild, construct_tree(rchild, i - (1 << (h - 1)), h - 1, val)};
    struct node* branch = malloc(sizeof(struct node));
    *branch = tree;
   // if (branch != NULL){
     //       printf("branch not null \n");
   // }
   // if ((*branch).right_child != NULL){
       //     printf("branch  rc not null \n");
   // }
   // currentroot = *branch;
   // char p = get_height(currentroot);
   // printf("%d", p);
    return branch;
} else {
    struct node* rchild = NULL;
    if(root.right_child != NULL){
        rchild = root.right_child;
    }

    struct node lchild = {-1};
    if (root.left_child != NULL){
        lchild = *root.left_child;
    }

    struct node tree = {-1, construct_tree(lchild, i - (1 << (h - 1)), h - 1, val), rchild};
    struct node* branch = malloc(sizeof(struct node));
    *branch = tree;
   // if (branch != NULL){
     //       printf("branch not null \n");
  //  }
   // if ((*branch).right_child != NULL){
       //     printf("branch  rc not null \n");
 //   }
   // currentroot = *branch;
  //  char p = get_height(currentroot);
   // printf("%d", p);
    return branch;
}
}

char getbits(u_int b){
    u_int val = b;
    char shift = 0;
    for (u_int bit = val; bit > 1; bit = val >> shift++);
    return shift;
}

void set(struct node root, u_int i, int val){
    char h = get_height(root);
    char d = getbits(i);
    char new_height;
    if (h>d) {new_height = h;} else {new_height = d;}
    struct node new_root = *construct_tree(root, i, new_height, val); 
    
   // printf("construction done\n");
    
   // if (new_root.right_child != NULL){
     //   printf("rpath exists");
  //  }
   // if (new_root.left_child != NULL){
      //  printf("lpath exists");
   // }
    currentroot = new_root;
    //char m = get_height(new_root);
    //printf("%d", m);
    //struct nodeStack newStack = {&root, &stacktop};
    //stacktop = newStack;
}

int get(struct node a, u_int i){

return -1;
}

void maxinterval(struct node a, u_int l_bound, u_int r_bound){
}

int main(){

    set(currentroot, 7, 10);
    char p = get_height(currentroot);
    printf("%d", p);

    set(currentroot, 100000, 10);
    char p1 = get_height(currentroot);
    printf("%d", p1);

    set(currentroot, 4, 10);
    char p2 = get_height(currentroot);
    printf("%d", p2);
    return 0;
}
