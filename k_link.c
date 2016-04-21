#include "k_link.h"


int test_base()
{
	struct k_link *root = NULL, *copy = NULL, *find;
    char *s1 = "hehe", *s2 = "heihei", *s3 = "GG";
    int n = 200;
	struct k_data *d1, *d2, *d3, *d4;
	char *str;
    
    k_data_malloc(&d1, s1, KDATA_TYPE_STRING);
    k_data_malloc(&d2, s2, KDATA_TYPE_STRING);
    k_data_malloc(&d3, s3, KDATA_TYPE_STRING);
    k_data_malloc(&d4, &n, KDATA_TYPE_INT32);
    
	k_link_insert_tail_malloc(&root, d1, KDATA_TYPE_KDATA);
	k_link_insert_tail_malloc(&root, d2, KDATA_TYPE_KDATA);
	k_link_insert_tail_malloc(&root, d3, KDATA_TYPE_KDATA);
	k_link_insert_this_tail_malloc(root -> next_node, d1, KDATA_TYPE_KDATA);
	k_link_insert_this_tail_malloc(root -> next_node, d2, KDATA_TYPE_KDATA);
	k_link_insert_this_tail_malloc(root -> next_node, d3, KDATA_TYPE_KDATA);
	k_link_insert_tail_malloc(&root, d4, KDATA_TYPE_KDATA);
    
	k_link_traversal_print_list_handle(root, 4, 1, NULL, NULL);
	printf("-------------------------------\n");
	k_link_free_node(k_link_get_node_offset(root, 2));
	
	k_link_traversal_print_list_handle(root, 4, 1, NULL, NULL);
	printf("-------------------------------\n");
	find = k_link_get_node_offset(root, 2);
	k_link_print_node_handle(find, NULL, NULL);
	
	printf("-------------------------------\n");
	k_link_node_copy_list_all(&copy, root);
	k_link_insert_this_tail_malloc(root -> next_node -> next_node, d1, KDATA_TYPE_KDATA);
	//k_link_traversal_free(root);
	k_link_traversal_print_list_handle(copy, 4, 1, NULL, NULL);
	k_link_traversal_print_list_handle(root, 4, 1, NULL, NULL);
	
	return 0;
}

int test_memary()
{
    int i, j;
    struct k_data *d1;
    struct k_link *root = NULL;
    char *s1 = "hehe";
    
    for(i = 0; i < 100000; i ++){
        k_data_malloc(&d1, s1, KDATA_TYPE_STRING);
        k_link_insert_after_malloc(&root, d1, KDATA_TYPE_KDATA);
        for(j = 0; j < 100; j ++){
            k_data_malloc(&d1, s1, KDATA_TYPE_STRING);
            k_link_insert_after_malloc(&root -> this_node, d1, KDATA_TYPE_KDATA);
        }
    }

    system("pause");
    
    k_link_traversal_free_handle(root, NULL, NULL);
    printf("free handle ~~\n");
    system("pause");
    
    k_link_free_all(root);
    printf("free k_link all ~~\n");
    system("pause");
    
    return 0;
}

int test_k_link_get_index()
{
    int record[16] = {3, 7, 7, 7, 0};
    int num = 4;
    int index, offset;
    
    index = 8, offset = 0;
    k_link_get_index(record, &num, &index, &offset);
    printf("get:[%d, %d]\n", index, offset);
    return 0;
}

int main()
{
    test_base();
    //test_memary();
    //test_k_link_get_index();
    
    printf("done");
    system("pause");
}
