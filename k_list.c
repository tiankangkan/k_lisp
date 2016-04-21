#include "k_list.h"


int test_base()
{
    struct k_list *list = NULL;
	
    k_list_insert_tail_malloc(&list, "hello");
    k_list_insert_tail_malloc(&list, ", ");
    k_list_insert_tail_malloc(&list, "world, ");
    k_list_insert_tail_malloc(&list, "baby");
    
    
    
    k_list_print_string(list);
    
    k_list_node_free(list -> next_node -> next_node);
    
    k_list_print_string(list);
    
    k_list_insert_tail_malloc(&list, "again ~");
    
    k_list_print_string(list);


/************************************/

//    struct k_list *list = NULL;
//    char *s1 = "hehe", *s2 = "heihei", *s3 = "GG";
//	struct k_data d1, d2, d3;
//	d1.type = d2.type = d3.type = KDATA_TYPE_STRING;
//    d1.d.p_data = s1, d2.d.p_data = s2, d3.d.p_data = s3;
//    k_list_insert_tail_malloc(&list, &d1);
//    k_list_insert_tail_malloc(&list, &d2);
//    k_list_insert_tail_malloc(&list, &d3);
//    
//    k_list_traversal_print_handle(list, NULL, NULL);
    
    return 0;
}

int test_memory()
{
    int i;
    struct k_list *list = NULL, *now;
    
    for(i = 0; i < 7; i ++){
        k_list_insert_head_malloc(&list, "hello");
    }
    
    system("pause");
    
    k_list_free_all(&list);
    
    return 0;
}


int main()
{
    test_base();
    //test_memory();
    system("pause");
}
