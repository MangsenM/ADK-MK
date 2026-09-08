#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/**
 * @brief Persistant array containing root node and history
 */
struct array{
  const struct node* root;
  struct node_stack* history;
};

/**
 * @brief Persistant tree node
 *
 * Node stores the max subtree value (or value if leaf) as well
 * as connections to right and left child, NULL pointer if no child
 */
struct node{
    int32_t max;
    const struct node* left_child;
    const struct node* right_child;
};

/**
 * @brief Stack for storing old tree root nodes
 *
 * Each root node is stored at start of allocation segment
 * All previous tree roots are stored in order with most
 * recent at the top of the stack
 */
struct node_stack{
    const struct node* head;
    struct node_stack* tail;
};

/**
 * @brief Creates new dynamic array.
 */
struct array* newarray(){
  struct array* aptr = malloc(sizeof(struct array));
  struct node* nptr = malloc(sizeof(struct node));
  struct node_stack* sptr = malloc(sizeof(struct node_stack));

  const struct node empty = {-1,NULL,NULL}; //NOTE: added assign to bot get garbge value

  *nptr = empty;
  //memcpy(nptr, &empty, sizeof(struct node));

  struct node_stack nullstack = {NULL,NULL};
  *sptr = nullstack;

  struct array narray = {nptr, sptr};
  *aptr = narray;

  return aptr;
}

/**
  * @brief Calculates hight of tree.
  *
  * @param node Root node of tree.
  *
  * NOTE: As leafs are all on the same hight it is 
  * enough to traverse one branch to leaf.
  */
char get_height(const struct node* root){
    if (root == NULL) { return 0; }

    if(root->left_child != NULL){
        return 1 + get_height(root->left_child);
    } else if(root->right_child != NULL){
        return 1 + get_height(root->right_child);
    } else {
        return 0;
    }
}

/**
 * @brief Extends hight of old tree by extending zeros to new height.
 *
 * @param root Root node of tree being extended.
 * @param l Levels to zero extend tree by
 * @param t Pointer to allocated space for new nodes
 */
const struct node* extend_tree(const struct node* root, char l, struct node* t){
    if (l == 0)
        return root;

    const struct node* lc = extend_tree(root, l - 1, &t[1]);

    const struct node newnode = {root->max, lc, NULL};
    *t = newnode;
    return t;
}

/**
 * @brief Updates leaf in tree of same height with new value.
 *
 * @param root Reference to root of remaining part of old tree.
 * @param i Remaining index where new value is being inserted.
 * @param l Remaining number of nodes to be created until leaf.
 * @param val Value to be inserted at leaf.
 * @param t Pointer to address where new nodes will be stored.
 *
 * The function recursevly creates new node path to index starting
 * at the root. Allocated space needs have been created as a block
 * with precalculated size for the entire new path to the leaf.
 */
const struct node* replace_leaf(const struct node* root, int32_t i, char l, int32_t val, struct node* t){
    const struct node nullnode = {0,NULL,NULL}; //nullnode to avoid null pointer in input

    if(root == NULL)
        root = &nullnode;

    if (l == 0){
        const struct node leaf = {val, NULL, NULL};
        memcpy(t, &leaf, sizeof(struct node));
        return t;
    }

    const struct node* rc;
    const struct node* lc;
    int32_t max;

    if (i >> (l - 1 ) == 1){
      lc = root->left_child;
      rc = replace_leaf(root->right_child, i - (1 << (l-1)), l-1, val, &t[1]);
      if (lc != NULL){
        max = (*lc).max > (*rc).max ? (*lc).max : (*rc).max;
      } else {max = (*rc).max;}
    }
    else {
      lc = replace_leaf(root->left_child, i, l-1, val, &t[1]);
      rc = root->right_child;
      if (rc != NULL){
        max = (*lc).max > (*rc).max ? (*lc).max : (*rc).max;
      } else {max = (*lc).max;}
    }

    const struct node newnode = {max, lc, rc};
    *t = newnode;
    return t;
}

/**
 * @brief Returns the number of bits required to reprecent input.
 *
 * @param b Number to calculate number of bits for.
 */
char get_bits(int32_t b){
    int32_t shift = 0;

    while(b >= 1){
        b = b >> 1;
        shift++;
    }
    //NOTE: changed from this, for (int bit = b; bit > 1; bit = b >> shift++);
    
    return shift;
}

/**
 * @brief Updates value at specific index of tree.
 *
 * @param root Root node of tree to update value at.
 * @param i Index to insert value at.
 * @param val Value to insert.
 */
void set(struct array* array, int32_t i, int32_t val){
    if (val < 0) { return; }

    char h0 = get_height(array->root);
    char b = get_bits(i);
    struct node_stack* newstack = malloc(sizeof(struct node_stack));
    *newstack = (struct node_stack){array->root, array->history};
    array->history = newstack;

    if(b > h0){
      if(array->root->max == -1){
        struct node* t = malloc(sizeof(struct node)*(b+1));
        array->root = replace_leaf(array->root, i, b, val, t);
      }else { 
        const struct node nullnode = {0,NULL,NULL};
        struct node* t = malloc(sizeof(struct node)*(2*b - h0)); //new path: b+1 nodes, extention of old tree: b-h0-1
        
        const struct node* lc = extend_tree(array->root, b - h0 - 1, &t[b+1]);
        const struct node* rc = replace_leaf(&nullnode, i - (1 << (b-1)), b-1, val, &t[1]);

        int32_t max = (*lc).max > (*rc).max ? (*lc).max : (*rc).max;
        const struct node newroot = {max, lc, rc};
        array->root = memcpy(t, &newroot, sizeof(struct node));
      }
    }else {
      struct node* t = malloc(sizeof(struct node)*(h0 + 1));
      array->root = replace_leaf(array->root, i, h0, val, t); 
    }
}

int32_t find(const struct node* root, int32_t i, char l){
    if (l == 0){
        return (*root).max;
    }

    if ((i >> (l - 1) == 1) && ((*root).right_child != NULL)){
        return find((*root).right_child, i - (1 << (l - 1)), l - 1); //NOTE: Added return
    } else if ((i >> (l - 1 ) == 0) && ((*root).left_child != NULL)){
        return find((*root).left_child, i, l - 1);
    } else {return 0;}
}

int32_t get(const struct array* array, int32_t i){
    if(array->root->max == -1) { return 0; }

    char h = get_height(array->root);
    char b = get_bits(i);
    if (b > h)
      return 0;
    
    return find(array->root, i, h);
}

void unset(struct array* array){
    if(array->history->head == NULL) { return; }
    free(array->root);
    array->root = (array->history)->head;

    struct node_stack* temp = array->history; //NOTE: Frees stackhead on pop now
    array->history = (array->history)->tail;
    free(temp);
}

/**
 * @brief Checks max interval for subtree only including lower bound.
 *
 * @param root Current node in searchpath.
 * @param h Height remaining in tree.
 * @param lbound Lower index bound (inclusive).
 */
int32_t maxinterval_lowerbound(const struct node *root, char h, int32_t lbound) {

    if (root->left_child == NULL && root->right_child == NULL) {
        return root->max;
    }

    if (lbound >> (h - 1) == 0) {
        if (root->left_child == NULL) { 
            return -1;

        } else if (root->left_child->max < root->max) {
            return root->max;

        } else {
            int32_t rcMAX = root->right_child != NULL ? root->right_child->max : -1;
            int32_t ltMAX = maxinterval_lowerbound(root->left_child, h - 1, lbound);
            return rcMAX > ltMAX ? rcMAX : ltMAX;
        }

    } else if (root->right_child != NULL) {
        return maxinterval_lowerbound(root->right_child, h - 1, lbound - (1 << (h - 1)));

    } else { return -1; }
}

/**
 * @brief Checks max interval for subtree only including lower bound.
 *
 * @param root Current node in searchpath.
 * @param h Height remaining in tree.
 * @param lbound Lower index bound (inclusive).
 */
int32_t maxinterval_upperbound(const struct node *root, char h, int32_t ubound) {

    if (root->left_child == NULL && root->right_child == NULL) {
        return root->max;
    }

    if (ubound >> (h - 1) == 1) {
        
        if (root->right_child == NULL) { 
            return -1;

        } else if (root->right_child->max < root->max) {
            return root->max;

        } else {

            int32_t lcMAX = root->left_child != NULL ? root->left_child->max : -1;
            int32_t rtMAX = maxinterval_upperbound(root->right_child, h - 1, ubound - (1 << (h - 1)));
            return lcMAX > rtMAX ? lcMAX : rtMAX;
        }

    } else if (root->left_child != NULL) {
        return maxinterval_upperbound(root->left_child, h - 1, ubound);

    } else { return -1; }
}

/**
 * @brief Checks how to split search path for maxinterval
 *
 * @param root Current node in searchpath.
 * @param h Height remaining in tree.
 * @param lbound Lower index bound (inclusive)
 * @param ubound Upper index bound (inclusive)
 */
int32_t maxinterval_decrese(const struct node *root, char h, int32_t lbound, int32_t ubound) {
    const struct node *lc = root->left_child;
    const struct node *rc = root->right_child;

    if (lc == NULL && rc == NULL) {
        return root->max;
    }

    if (lbound >> (h - 1) == ubound >> (h - 1)) {

        if (lbound >> (h - 1) == 1 && rc != NULL){
            return maxinterval_decrese(rc, h - 1, lbound - (1 << (h - 1)), ubound - (1 << (h - 1)));

        } else if (ubound >> (h - 1) == 0 && lc != NULL){
            return maxinterval_decrese(lc, h - 1, lbound, ubound);

        } else { return -1; }

    } else {
        int32_t leftmax = lc != NULL ? maxinterval_lowerbound(lc, h - 1, lbound) : -1;
        int32_t rightmax = rc != NULL ? maxinterval_upperbound(rc, h - 1, ubound - (1 << (h - 1))) : -1;
        return leftmax > rightmax ? leftmax : rightmax;
    }
}

/**
 * @brief Gets max element between specified indexes (inclusive).
 *
 * @param array Array being serched
 * @param lbound Lower index bound (inclusive)
 * @param ubound Upper index bound (inclusive)
 */
int32_t maxinterval(const struct array *array, int32_t lbound, int32_t ubound) {

    int h = get_height(array->root);
    int32_t max;

    if (lbound > ubound || lbound < 0 || get_bits(lbound) > h ) { return 0; }

    if (get_bits(ubound) > h){
      max = maxinterval_lowerbound(array->root, h, lbound);
    }else {
      max = maxinterval_decrese(array->root, h, lbound, ubound);
    }

    return max == -1 ? 0 : max;
}

int main(){

    struct array* A = newarray();

    int run = 1;

    while(run){
        char buff[50];
        char command[15];
        int arg1;
        int arg2;
        int parts;

        if (fgets(buff, 50, stdin) != NULL){
            parts = sscanf(buff, "%14s%d%d", command, &arg1, &arg2);

            if(strcmp(command, "set") == 0 && parts == 3){

                set(A, arg1, arg2);


            } else if(strcmp(command, "get") == 0 && parts == 2){

                printf("%d\n", get(A, arg1));

            } else if(strcmp(command, "unset") == 0 && parts == 1){

                unset(A);

            } else if(strcmp(command, "maxininterval") == 0 && parts == 3){

                printf("%d\n",maxinterval(A, arg1, arg2));
            }
        }else{run = 0;}
    }

return 0;
}
