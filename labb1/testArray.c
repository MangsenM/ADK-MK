#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "main.h"


void set_first_at_0(void){
    A = newArray();
    
    set(A, 0, 5);
}

void set_first_at_1000(void){
    A = newArray();
    
    set(A, 0, 5);

}

void set_first_at_MAX(void){
    A = newArray();
    
    set(A, 0, 5);

}

void set_increse_1level(void){
    A = newArray();
    
    set(A, 0, 5);

}

void set_increse_5levels(void){
    A = newArray();
    
    set(A, 0, 5);

}

void set_negative_index(void){
    A = newArray();
    
    set(A, 0, 5);
}

void set_first_at_zero(void){

}

void set_first_at_zero(void){

}

int init_suite(void) {
  return 0;
}

int clean_suite(void) {
  return 0;
}

int main(){

    if (CU_initialixe_registry() != CUE_SUCCESS)
        return CU_get_error();

    CU_pSuite pSuite = CU_add_suite("Array_Suite", init_suite, clean_suite);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuite, "desc", func))||
        (NULL == CU_add_test(pSuite, "desc", func))||
        (NULL == CU_add_test(pSuite, "desc", func))||
        (NULL == CU_add_test(pSuite, "desc", func))||
        (NULL == CU_add_test(pSuite, "desc", func))||
        (NULL == CU_add_test(pSuite, "desc", func))||
        (NULL == CU_add_test(pSuite, "desc", func))||
        (NULL == CU_add_test(pSuite, "desc", func))||
        (NULL == CU_add_test(pSuite, "desc", func))||
        (NULL == CU_add_test(pSuite, "desc", func))||
        (NULL == CU_add_test(pSuite, "desc", func))||
        (NULL == CU_add_test(pSuite, "desc", func))||
        (NULL == CU_add_test(pSuite, "desc", func))){

        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_run_tests();

    CU_cleanup_registry();
    return CU_get_error();
}
