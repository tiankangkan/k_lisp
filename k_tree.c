#include "k_tree.h"
#include "k_timer.h"

struct k_data *k_handle_get_data(void *handle, void *parameter)
{
    return (struct k_data *)handle;
}

int test_base()
{
	struct k_tree *root = NULL;
    char *s1 = "hehe", *s2 = "heihei", *s3 = "GG";
    int n = 200;
	struct k_data *d1, *d2, *d3, *d4;
	char *str;
    
    k_data_malloc(&d1, s1, KDATA_TYPE_STRING);
    k_data_malloc(&d2, s2, KDATA_TYPE_STRING);
    k_data_malloc(&d3, s3, KDATA_TYPE_STRING);
    k_data_malloc(&d4, &n, KDATA_TYPE_INT32);
    
    k_tree_insert_handle(&root, d1, k_handle_get_data, NULL, k_data_comp);
    k_tree_insert_handle(&root, d2, k_handle_get_data, NULL, k_data_comp);
    k_tree_insert_handle(&root, d3, k_handle_get_data, NULL, k_data_comp);
    //k_tree_insert_handle(&root, d4, k_data_get_value, NULL, k_data_comp);
    
    //k_tree_ergodic(root, k_tree_print_single_item_string);
    
    k_tree_insert_handle(&root, d3, k_handle_get_data, NULL, k_data_comp);
    k_tree_insert_handle(&root, d3, k_handle_get_data, NULL, k_data_comp);
    k_tree_insert_handle(&root, d3, k_handle_get_data, NULL, k_data_comp);

    k_tree_ergodic_handle(root, k_handle_get_data, NULL, k_tree_print_handle);

    printf("tree size = %d\n", sizeof(struct k_tree) + sizeof(struct k_list));
    
    printf("================ test_base finish ================\n");
    return 0;
}

int test_speed()
{
    struct k_tree *root = NULL;
    struct k_data *d1;
    int i, number;
    char *str;
    
    for(i = 0; i < 1000000; i ++){
        number = (rand() * i) % 12345678;
        k_data_malloc(&d1, &number, KDATA_TYPE_INT32);
        k_tree_insert_handle(&root, d1, k_handle_get_data, NULL, k_data_comp);
    }
    
    printf("check memory ~~\n");
    system("pause");

    k_tree_free_all_node(root);

    printf("check memory again ~~\n");
    system("pause");
    
    printf("================ test_speed finish ================\n");
    return 0;
}

int main()
{
    struct k_timer timer;
    
    
    test_base();
    k_timer_start(&timer);
    test_speed();
    k_timer_stop(&timer);
    k_timer_print(&timer);
    
    system("pause");
}
