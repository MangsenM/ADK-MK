#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/**
 * @brief Persistant array containing root node and history
 */
struct array{
  struct node* root;
  struct node_stack* history;
};

/**
 * @brief Persistant tree node
 *
 * Node stores the max subtree value (or value if leaf) as well
 * as connections to right and left child, NULL pointer if no child
 */
struct node{
    const int max;
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
    struct node* head;
    struct node_stack* tail;
};

/**
 * @brief Creates new dynamic array.
 */
struct array* newarray(){
  struct array* aptr = malloc(sizeof(struct array));
  struct node* nptr = malloc(sizeof(struct node));
  struct node_stack* sptr = malloc(sizeof(struct node_stack));

  struct node nullnode;
  memcpy(nptr, &nullnode, sizeof(struct node));

  struct node_stack nullstack;
  memcpy(sptr, &nullstack, sizeof(struct node_stack));

  struct array nullarray = {nptr, sptr};

  return memcpy(aptr, &nullarray, sizeof(struct array));
}

/**
  * @brief Calculates hight of tree.
  *
  * @param node Root node of tree.
  */
char get_height(const struct node* root){
//printf("get_height reached\n");
      if (root == NULL){printf("root is null"); return 0;}
//printf("get_height reached first if passed\n");

    if(root->left_child != NULL){
//printf("get_height looking left\n");
        return 1 + get_height(root->left_child);
    } else if(root->right_child != NULL){
//printf("get_height reached looking right\n");
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
struct node* extend_tree(const struct node* root, char l, struct node* t){
    if (l == 0)
        return root;

    //struct node* rc; //should be able to deleat this and just assign NULL
    struct node* lc = extend_tree(root, l - 1, &t[1]);

    struct node newnode = {root->max, lc, NULL}; //TODO: Look at if able to do without copy
    return memcpy(t, &newnode, sizeof(struct node));
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
struct node* replace_leaf(const struct node* root, u_int i, char l, int val, struct node* t){
printf("replace_leaf reached\n");
    struct node nullnode = {(int)NULL,NULL,NULL}; //nullnode to avoid null pointer in input
    if(root == NULL)
        root = &nullnode;

    if (l == 0){
printf("rl basecase reached reached\n");
        struct node leaf = {val, NULL, NULL};
        memcpy(t, &leaf, sizeof(struct node));
        printf("leaf value at set: %d\n",(*t).max);
        return t;
    }

    struct node* rc;
    struct node* lc;
    int max;

    if (i >> (l - 1 ) == 1){
printf("creating right path\n");
      lc = root->left_child;
      rc = replace_leaf(root->right_child, i - (1 << (l-1)), l-1, val, &t[1]);
printf("check child: %d\n", t[1].max);
      if (lc != NULL){
      max = (*lc).max > (*rc).max ? (*lc).max : (*rc).max;
      } else {max = (*rc).max;}
    }
    else {
printf("creating left path\n");
      lc = replace_leaf(root->left_child, i, l-1, val, &t[1]);
      rc = root->right_child;
printf("check child: %d\n", t[1].max);
      if (rc != NULL){
      max = (*lc).max > (*rc).max ? (*lc).max : (*rc).max;
      } else {max = (*lc).max;}
    }

printf("max is = %d\n", t[1].max);
    struct node newnode = {max, lc, rc};
    return memcpy(t, &newnode, sizeof(struct node));
}

/**
 * @brief Returns the number of bits required to reprecent input.
 *
 * @param b Number to calculate number of bits for.
 */
char get_bits(u_int b){
    u_int val = b;
    char shift = 0;
    for (u_int bit = val; bit > 1; bit = val >> shift++);
    return shift;
}

/**
 * @brief Updates value at specific index of tree.
 *
 * @param root Root node of tree to update value at.
 * @param i Index to insert value at.
 * @param val Value to insert.
 */
void set(struct array* array, u_int i, int val){
printf("reached set\n");
    char h0 = get_height(array->root);
    char b = get_bits(i);
    struct node_stack* newstack = malloc(sizeof(struct node_stack));
    *newstack = (struct node_stack){array->root, array->history};
    array->history = newstack;

printf("choosing path with i = %d , h0 = %d, b = %d \n", i,h0,b);
    if(b > h0){
printf("b > h0\n");

      if((array->root->right_child != NULL) || (array->root->left_child != NULL)){ 
printf("root is not nullnode\n");
        struct node nullnode = {(int) NULL,NULL,NULL};
        struct node* t = malloc(sizeof(struct node)*(2*b - h0)); //new path: b+1 nodes, extention of old tree: b-h0-1
        struct node* lc = extend_tree(array->root, b - h0 - 1, &t[b+2]);
printf("extended hight: %d\n",get_height(lc));
        struct node* rc = replace_leaf(&nullnode, i - (1 << (b-1)), b-1, val, &t[1]);

        int max = (*lc).max > (*rc).max ? (*lc).max : (*rc).max;
        struct node newroot = {max, lc, rc};
        array->root = memcpy(t, &newroot, sizeof(struct node));
      } else { //edgecase for first set
printf("root is nullnode\n");
        struct node* t = malloc(sizeof(struct node)*(b+1));
        array->root = replace_leaf(array->root, i, b, val, t);
printf("replace_leaf done \n");
printf("rc max is = %d\n", (((array)->root->right_child)->max));
printf("ROOT set \n");
      }
    } else {

printf("b <= h0\n");
    struct node* t = malloc(sizeof(struct node)*(h0 + 1));
    array->root = replace_leaf(array->root, i, h0, val, t); 
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
    } else {return 0;}
}

int get(const struct array* array, u_int i){
printf("get reached\n");
    
    char h = get_height(array->root);
    char b = get_bits(i);
    if (b > h)
      return 0;
    
    return find(array->root, i, h);
}

void unset(struct array* array){
    if(array->history == NULL){return;}
    free(array->root);
    array->root = (array->history)->head;
    array->history = (array->history)->tail;
}

/**
 * @brief Checks max interval for subtree only including lower bound.
 *
 * @param root Current node in searchpath.
 * @param h Height remaining in tree.
 * @param lbound Lower index bound (inclusive).
 */
int maxinterval_lowerbound(const struct node* root, char h, int lbound){
  if(lbound >> h-1 == 0){
    int lMAX = root->left_child != NULL ? root->left_child->max : -1;
    int nodeMAX = root->max;

    return lMAX <= nodeMAX ? nodeMAX : maxinterval_lowerbound(root->left_child,h-1,lbound);
  }
  else if (root->right_child != NULL)
    return maxinterval_lowerbound(root->right_child, h-1, lbound-(1<<h-1));
  else
    return root->max;
}

/**
 * @brief Checks max interval for subtree only including lower bound.
 *
 * @param root Current node in searchpath.
 * @param h Height remaining in tree.
 * @param lbound Lower index bound (inclusive).
 */
int maxinterval_upperbound(const struct node* root, char h, int ubound){
  if(ubound >> h-1 == 1){
    int rMAX = root->right_child != NULL ? root->right_child->max : -1;
    int nodeMAX = root->max;

    return rMAX <= nodeMAX ? nodeMAX : maxinterval_upperbound(root->right_child,h-1,ubound-(1<<(h-1)));
  }
  else if (root->left_child != NULL)
    return maxinterval_upperbound(root->left_child, h-1, ubound);
  else
    return root->max;
}

/**
 * @brief Checks how to split search path for maxinterval
 *
 * @param root Current node in searchpath.
 * @param h Height remaining in tree.
 * @param lbound Lower index bound (inclusive)
 * @param ubound Upper index bound (inclusive)
 */
int maxinterval_decrese(const struct node* root, char h, int lbound, int ubound){
  const struct node* lc = root->left_child;
  const struct node* rc = root->right_child;

  if (lc == NULL && rc == NULL)
    return root->max;

  if(lbound >> h-1 == ubound >> h-1){
    if (lbound >> h-1 == 1 && rc != NULL)
      return maxinterval_decrese(rc,h-1, lbound-(1<<h-1), ubound-(1<<h-1));
    else if (lc != NULL)
      return maxinterval_decrese(lc,h-1,lbound,ubound);
    else
      return -1;
  } else {
    int leftmax = lc != NULL ? maxinterval_lowerbound(lc, h-1, lbound) : -1;
    int rightmax = rc != NULL ? maxinterval_upperbound(rc, h-1, ubound) : -1;
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
int maxinterval(const struct array* array, int lbound, int ubound){
    if (lbound > ubound)
      return 0;

    int max = maxinterval_decrese(array->root, get_height(array->root), lbound, ubound); 

    if (max == -1) 
      return 0;
    else
      return max;
}

int main(){
    struct array* A = newarray();

    set(A, 4, 2);
    set(A, 1, 10);
    set(A, 2, 20);
    set(A, 3, 1);
    set(A, 4, 112);
    set(A, 5, 200);
    set(A, 6, 90);
    set(A, 12, 90);

    printf("%d \n", maxinterval(A,0,2));
    printf("%d \n", maxinterval(A,2,2));
    printf("%d \n", maxinterval(A,0,12));
    printf("%d \n", maxinterval(A,6,14));
    printf("%d \n", maxinterval(A,8,12));
    printf("%d \n", maxinterval(A,0,2));

    int rootmax;
    char p;
    int i;
    char h;
    i = get(A, 4);
    printf("current value at %d is: %d\n",4,i);
    printf("\n");

    printf("inserting %d at index %d \n", 100,4);
    set(A, 4, 100);
    printf("set done \n");
    h = get_height(A->root);
    printf("get_height done \n");
    printf("current height is: %d\n", h);
    printf("looking for rootmax \n");
    rootmax = (A->root)->max;
    printf("rootmax assigned\n");
    printf("max in Tree is: %d\n", rootmax);
    i = get(A, 4);
    printf("current value at %d is: %d\n",4,i);

    printf("\n");
    printf("inserting %d at index %d \n", 120,9);
    set(A, 9, 120);
    h = get_height(A->root);
    printf("current height is: %d\n", h);
    rootmax = (A->root)->max;
    printf("max in Tree is: %d\n", rootmax);
    i = get(A, 9);
    printf("current value at %d is: %d\n",9,i);
    printf("sanity: %d\n", ((A->root)[4]).max);
    i = get(A, 4);
    printf("current value at %d is: %d\n",4,i);

    printf("\n");
    printf("inserting %d at index %d \n", 10,4);
    set(A, 4, 10);
    h = get_height(A->root);
    printf("current height is: %d\n", h);
    rootmax = (A->root)->max;
    printf("max in Tree is: %d\n", rootmax);
    
    h = get_height(A->root);
    printf("height = %d\n",h);
    i = get(A, 4);
    printf("current value at %d is: %d\n",4,i);

    printf("\n");
    h = get_height(A->root);
    printf("height = %d\n",h);
    i = get(A, 9);
    printf("current value at %d is: %d\n",9,i);
    
    printf("\n");
printf("unset");
    printf("\n");
    unset(A);
    h = get_height(A->root);
    printf("height = %d\n",h);
    i = get(A, 4);
    printf("current value at %d is: %d\n",4,i);

    h = get_height(A->root);
    printf("height = %d\n",h);
    i = get(A, 9);
    printf("current value at %d is: %d\n",9,i);
    
    printf("\n");
printf("unset");
    printf("\n");
    unset(A);
    h = get_height(A->root);
    printf("height = %d\n",h);
    i = get(A, 4);
    printf("current value at %d is: %d\n",4,i);

    h = get_height(A->root);
    printf("height = %d\n",h);
    i = get(A, 9);
    printf("current value at %d is: %d\n",9,i);

    printf("\n");
printf("unset");
    printf("\n");
    unset(A);
    h = get_height(A->root);
    printf("height = %d\n",h);
    i = get(A, 4);
    printf("current value at %d is: %d\n",4,i);

    h = get_height(A->root);
    printf("height = %d\n",h);
    i = get(A, 9);
    printf("current value at %d is: %d\n",9,i);
    return 0;
//*/
}
