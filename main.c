#include <sys/types.h>
#include <stdlib.h>

struct node root;

struct node{
    int max_in_sub_tree;
    struct node* left_child;
    struct node* right_child;
};


char get_height(struct node root){
    if(root.left_child != NULL){
        return 1 + get_height(*root.left_child);
    } else if(root.right_child != NULL){
        return 1 + get_height(*root.right_child);
    } else {
        return 0;
    }
}

void set(struct node a, u_int i, int val){
    char h = get_height(a);
    int* new_path = malloc(sizeof(struct node)*(h+1));
    
    



}

struct node* construct_path(struct node* root, u_int i, int val){


}

int get(struct node a, u_int i){

return -1;
}

void maxinterval(struct node a, u_int l_bound, u_int r_bound){
}

int main(){

    return 0;
}
