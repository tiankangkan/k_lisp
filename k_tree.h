/**
    name      : k_tree.h
    date      ��2015.07.25
    describe  : ������ ����������
    copyright : ���������ʹ����, ���ǲ����ڲ�����汾�ŵ�����¶��������޸�.
    note      : ��ע�� ���е��ͷź���������Խڵ�ָ���ʵ���������ͷŲ���.
    updata    :
        1) �����������ͷź��� k_tree_free_all_node().
*/

#ifndef __K_TREE_H__
    #define  __K_TREE_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "k_error.h"
#include "k_base.h"
#include "k_data.h"
#include "k_list.h"

struct k_tree{
    void *p_data;            /* ����һ��ٶ� p_data ָ��ĵ�ַ���ڴ������ k_tree.h �Ĺ�Ͻ��Χ */
    int   count;             /* �ظ���Ӽ��� */
    struct k_list *record;   /* record ���ڶ����������¼, ��ʹ�ظ�����Ҳ����¼ */
    struct k_tree *p_left;   /* ���ӽڵ�, һ���� "С��" ���ڵ� */
    struct k_tree *p_right;  /* ���ӽڵ�, һ���� "����" ���ڵ� */
    struct k_tree *p_parent; /* ���ڵ� */
};

/* ���һ����Ŀ�� ��, ���� 1 ������Ŀ�Ѵ���, �� *tree Ϊ NULL ʱ, ����Ϊ������ڴ�, ����������Ϊ���ڵ� */
int k_tree_insert_item_to_tree(struct k_tree **tree, void *data, int (*k_tree_comp)(void *a1, void *a2));
/* ɾ��һ�����ڵ�, ����Ϊ struct k_tree **node Ϊ�˿���ɾ�� ���ڵ�, ע�⺯���Ӳ���ɾ�� tree.p_data */
int k_tree_delete_node(struct k_tree **p_node);
/* �ͷ������� */
int k_tree_free_all_node(struct k_tree *tree);
/* ����һ�����ڵ�(�����ڴ�) */
int k_tree_create_node(struct k_tree **node,  void *data);
/* ǿ���ͷ�һ�����ڵ���ռ�ڴ�, �������ͷ�ʱ���� -1 */
int tree_free_node(struct k_tree *node);
/* ��� int ���͵ıȽϺ��� */
int k_tree_comp_int(void *a1, void *a2);
/* ��Գ��� �ַ���('\0' ��β) �ıȽϺ��� */
int k_tree_comp_string(void *a1, void *a2);
/* ������������, ����ֵΪָ�� ���ڵ��ָ��, û�в�ѯ������� NULL */
struct k_tree * k_tree_search(struct k_tree *tree, void *data, int (*k_tree_comp)(void *a1, void *a2));
/* ����������(�ݹ鷨, �ٶ���, �������ִ�� k_tree_process_on_single_item() ����), ���ṩһ����Ե������ڵ�Ĵ����� */
int k_tree_ergodic(struct k_tree *tree, int (*k_tree_process_on_single_item)(struct k_tree *tree));
/* ����ڵ���Ϣ, �ڵ�����Ϊ int ���� */
int k_tree_print_single_item_int(struct k_tree *tree);
/* ����ڵ���Ϣ, �ڵ�����Ϊ����� �ַ������� */
int k_tree_print_single_item_string(struct k_tree *tree);
/* �������ǿ(���������ݼ�)��һ����Ŀ�ӵ�����, k_tree_get_nth_item ���� �� n �� p_data */
int k_tree_insert_sorted_items_to_tree(struct k_tree **tree, void *p_sorted_items, int start, int end,
            int (*k_tree_comp)(void *a1, void *a2), void *(*k_tree_get_nth_item)(void *p_sorted_items, int n));

/* ���һ����Ŀ�� ��, ���� 1 ������Ŀ�Ѵ���, �� *tree Ϊ NULL ʱ, ����Ϊ������ڴ�, ����������Ϊ���ڵ� */
int k_tree_insert_handle(struct k_tree **tree, void *handle, struct k_data *k_handle_get_data(void *handle, void *parameter), 
            void *parameter, int (*k_data_comp)(struct k_data *a1, struct k_data *a2));
/* ɾ��һ�����ڵ�, ����Ϊ struct k_tree **node Ϊ�˿���ɾ�� ���ڵ�, ע�⺯���Ӳ���ɾ�� tree.p_data */
int k_tree_delete_handle(struct k_tree **node);
/* ��ӡ��� */
int k_tree_print_handle(struct k_tree *tree, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter);
/* ����������handle���в����ĺ���(�ݹ鷨, �ٶ���), ���ṩһ����Ե������ڵ�Ĵ����� */
int k_tree_ergodic_handle(struct k_tree *tree, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter, 
            int process_on_node(struct k_tree *tree, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter));
/* ������������, ����ֵΪָ�� ���ڵ��ָ��, û�в�ѯ������� NULL */
struct k_tree *k_tree_search_handle(struct k_tree *tree, struct k_data *to_find, struct k_data *k_handle_get_data(void *handle, void *parameter), 
            void *parameter, int (*k_data_comp)(struct k_data *a1, struct k_data *a2));
            
/*************************************************************/
            
/* ���һ����Ŀ�� ��, ���� 1 ������Ŀ�Ѵ���, �� *tree Ϊ NULL ʱ, ����Ϊ������ڴ�, ����������Ϊ���ڵ� */
int k_tree_insert_item_to_tree(struct k_tree **tree, void *data, int (*k_tree_comp)(void *a1, void *a2))
{
    struct k_tree *T, *last_T, *T_new;
    int temp;
    
    last_T = T = *tree;
    if(*tree == NULL){
        k_tree_create_node(tree,  data);
        (*tree) -> p_parent = NULL;

        return 0;
    }
    else{
        while(T != NULL){
            temp = k_tree_comp(data, T -> p_data);
            if(temp == 1){
                last_T = T;
                T = T -> p_right;
            }
            else if(temp == -1){
                last_T = T;
                T = T -> p_left;
            }
            else if(temp == 0){
                (T -> count) ++;
                k_list_insert_tail_malloc(&T -> record, data); 
                return 1;
            }
        }

        k_tree_create_node(&T_new, data);

        /* ���½ڵ���뵽 last_T ���ӽڵ��� */
        if((temp = k_tree_comp(data, last_T -> p_data)) > 0){
            last_T -> p_right = T_new;
        }else
            last_T -> p_left = T_new;

        /* ���½ڵ�ĸ��ڵ� ����Ϊ last_T */
        T_new -> p_parent = last_T;

    }
    return 0;
}

/* ɾ��һ�����ڵ�, ����Ϊ struct k_tree **node Ϊ�˿���ɾ�� ���ڵ�, ע�⺯���Ӳ���ɾ�� tree.p_data */
int k_tree_delete_node(struct k_tree **p_node)
{
    struct k_tree *node;
    struct k_tree *node_now, **node_check;
    void *p_data_temp;

    if(p_node == NULL || *p_node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : p_node or *p_node pointer to NULL !", __FUNCTION__);
        return -1;
    }

    node = *p_node;

    /* �� node ����һ���ӽڵ�ʱ */
    if(node -> p_left == NULL || node -> p_right == NULL){
        /* node_now Ϊ node ���ӽڵ� */
        node_now = node -> p_left != NULL ? node -> p_left : node -> p_right;

        /* �� *p_node ��Ϊ���ڵ�ʱ, ... */
        if(node -> p_parent != NULL){
            /* �� node �ĸ��ڵ�ָ�����ӽڵ� */
            if(node -> p_parent -> p_left == node)
                node_check = &(node -> p_parent -> p_left);
            else
                node_check = &(node -> p_parent -> p_right);
            *node_check = node_now;

            /* �� node ���ӽڵ�(����еĻ�), ָ���丸�ڵ� */
            if(node_now != NULL)
                node_now -> p_parent = node -> p_parent;
        }else{
            /* �� node_now(�����Ϊ NULL�Ļ�) �ĸ��ڵ� ��Ϊ NULL */
            if(node_now != NULL)
                node_now -> p_parent = NULL;
            *p_node = node_now;
        }

        /* �ͷ��ڴ� */
        tree_free_node(node);

        return 0;
    }

    /* �õ�ǰ���ڵ� */
    node_now = node -> p_left;
    while(node_now -> p_right != NULL){
        node_now = node_now -> p_right;
    }

    /* ���� node �� node_now �� p_data */
    p_data_temp = node -> p_data;
    node -> p_data = node_now -> p_data;
    node_now -> p_data = p_data_temp;

    /* ��ʱ�� node_now ������ node �� p_data ���� ֻ��һ���ӽڵ� */
    k_tree_delete_node(&node_now);

    return 0;
}

/* �ͷ������� */
int k_tree_free_all_node(struct k_tree *tree)
{
    if(tree == NULL)
        return 0;
    
    k_tree_free_all_node(tree -> p_left);
    k_tree_free_all_node(tree -> p_right);

    k_tree_delete_node(&tree);

    return 0;
}

/* ����һ�����ڵ�(�����ڴ�) */
int k_tree_create_node(struct k_tree **node,  void *data)
{
    *node = (struct k_tree *)malloc(sizeof(struct k_tree));
    (*node) -> p_data = data;
    (*node) -> count = 1;
    (*node) -> p_left = NULL;
    (*node) -> p_right = NULL;
    (*node) -> p_parent = NULL;
    
    /* ���뵽��¼�� */
    (*node) -> record = NULL;
    k_list_insert_tail_malloc(&(*node) -> record, data); 

    return 0;
}

/* ǿ���ͷ�һ�����ڵ���ռ�ڴ�, �������ͷ�ʱ���� -1 */
int tree_free_node(struct k_tree *node)
{
    if(node != NULL){
        k_list_free_all(&node -> record);
        free(node);
    }else{
        return -1;
    }
    return 0;
}


/* ��� int ���͵ıȽϺ��� */
int k_tree_comp_int(void *a1, void *a2)
{
    int r;
    
    r = *(int *)a1 - *(int *)a2;
    if(r == 0)
        return K_CMP_EQUAL;
    else
        return r > 0 ? K_CMP_BIGGER : K_CMP_SMALLER;
}

/* ��Գ��� �ַ���('\0' ��β) �ıȽϺ��� */
int k_tree_comp_string(void *a1, void *a2)
{
    int i;
    unsigned char *string_a, *string_b;
    
    if(a1 == NULL || a2 == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : some pointer is NULL !", __FUNCTION__);
        return K_CMP_UNKNOWN;
    }
    
    string_a = (unsigned char *)a1;
    string_b = (unsigned char *)a2;

    for(i = 0; string_a[i] != '\0'; i ++){
        if(string_a[i] != string_b[i])
            break;
    }

    if(string_a[i] == '\0' && string_b[i] != '\0')
        return K_CMP_SMALLER;    /* С�� */
    else if(string_a[i] - string_b[i] == 0)
        return K_CMP_EQUAL;
    else
        return string_a[i] - string_b[i] > 0 ? K_CMP_BIGGER : K_CMP_SMALLER;
}

/* ������������, ����ֵΪָ�� ���ڵ��ָ��, û�в�ѯ������� NULL */
struct k_tree *k_tree_search(struct k_tree *tree, void *data, int (*k_tree_comp)(void *a1, void *a2))
{
    struct k_tree *T = tree;
    int temp = 0;
    while(T != NULL){
        if((temp = k_tree_comp(data, T -> p_data)) > 0)
            T = T -> p_right;
        else if(temp < 0)
            T = T -> p_left;
        else{
            return T;
        }
    }

    return NULL;
}

/* ����������(�ݹ鷨, �ٶ���), ���ṩһ����Ե������ڵ�Ĵ����� */
int k_tree_ergodic(struct k_tree *tree, int (*k_tree_process_on_single_item)(struct k_tree *tree))
{
    if(tree != NULL){
        k_tree_ergodic(tree -> p_left, k_tree_process_on_single_item);
        k_tree_process_on_single_item(tree);
        k_tree_ergodic(tree -> p_right, k_tree_process_on_single_item);
    }

    return 0;
}

/* ����ڵ���Ϣ, �ڵ�����Ϊ int ���� */
int k_tree_print_single_item_int(struct k_tree *tree)
{
    printf("count = %4d, data = %d\n", tree -> count, *(int *)(tree -> p_data));

    return 0;
}

/* ����ڵ���Ϣ, �ڵ�����Ϊ����� �ַ������� */
int k_tree_print_single_item_string(struct k_tree *tree)
{
    printf("count = %4d, data = %s, father = %s\n", tree -> count, (char *)(tree -> p_data), tree -> p_parent != NULL ? (char *)(tree -> p_parent -> p_data) : NULL);
    //printf("count = %4d, data = %s\n", tree -> count, (char *)(tree -> p_data));
    return 0;
}

/* �������ǿ(���������ݼ�)��һ����Ŀ�ӵ����� */
int k_tree_insert_sorted_items_to_tree(struct k_tree **tree, void *p_sorted_items, int start, int end, int (*k_tree_comp)(void *a1, void *a2), void *(*k_tree_get_nth_item)(void *p_sorted_items, int n))
{
    int i, j;
    int num, zone, base, depth, level_start, step, head;
    void *data;

    num = (end - start) + 1;

    for(i = 1, base = 1; i < 32; i ++){
        base = base * 2;
        if(base - 1 > num)
            break;
    }
    depth = i - 1;
    zone = base / 2 - 1;

    head = (num - zone) / 2;

    /* ���� zone ���� */
    for(i = depth - 1; i >= 0; i --){
        level_start = (int)pow(2, i);
        step = (int)pow(2, i + 1);
        for(j = level_start; j <= zone - level_start + 1; j += step){
            data = k_tree_get_nth_item(p_sorted_items, j + start + head - 1);
            k_tree_insert_item_to_tree(tree, data, k_tree_comp);
        }
    }

    /* ����ͷ����Ŀ */
    for(i = 0; i < head; i ++){
        data = k_tree_get_nth_item(p_sorted_items, i);
        k_tree_insert_item_to_tree(tree, data, k_tree_comp);
    }
    /* ����β����Ŀ */
    for(i = start + head + zone; i <= end; i ++){
        data = k_tree_get_nth_item(p_sorted_items, i);
        k_tree_insert_item_to_tree(tree, data, k_tree_comp);
    }

    return 0;
}

void *k_tree_get_nth_item_string(void *p_sorted_items, int n)
{
    void *data;
    char **strings;

    strings = (char **)p_sorted_items;

    return strings[n];
}

void *k_tree_get_nth_item_int(void *p_sorted_items, int n)
{
    int *items;

    items = (int *)p_sorted_items;

    return &items[n];
}

/* ���һ����Ŀ�� ��, ���� 1 ������Ŀ�Ѵ���, �� *tree Ϊ NULL ʱ, ����Ϊ������ڴ�, ����������Ϊ���ڵ� */
int k_tree_insert_handle(struct k_tree **tree, void *handle, struct k_data *k_handle_get_data(void *handle, void *parameter), 
            void *parameter, int (*k_data_comp)(struct k_data *a1, struct k_data *a2))
{
    struct k_tree *T, *last_T, *T_new;
    struct k_data *a, *b;
    int r;
    
    last_T = T = *tree;
    if(*tree == NULL){
        k_tree_create_node(tree,  handle);
        (*tree) -> p_parent = NULL;
        return 0;
    }
    else{
        while(T != NULL){
            if(k_handle_get_data != NULL){
                a = k_handle_get_data(handle, parameter);
                b = k_handle_get_data(T -> p_data, parameter);
            }else{
                a = (struct k_data *)handle;
                b = (struct k_data *)T -> p_data;
            }
            r = k_data_comp(a, b);
            if(r == K_CMP_BIGGER){
                last_T = T;
                T = T -> p_right;
            }
            else if(r == K_CMP_SMALLER){
                last_T = T;
                T = T -> p_left;
            }
            else if(r == K_CMP_EQUAL){
                last_T = T;
                (T -> count) ++;
                k_list_insert_head_malloc(&T -> record, handle); 
                return 1;
            }else if(r == K_CMP_UNKNOWN){
                my_error(fp_error, MODE_ERROR, "<%s> : error when comparing, item do not been insert!", __FUNCTION__);
                return -1;    /* �Ƚϳ���, �����ǲ����ݵ����� */
            }
        }

        k_tree_create_node(&T_new, handle);

        /* ���½ڵ���뵽 last_T ���ӽڵ��� */
        if(k_handle_get_data != NULL){
            a = k_handle_get_data(handle, parameter);
            b = k_handle_get_data(last_T -> p_data, parameter);
        }else{
            a = (struct k_data *)handle;
            b = (struct k_data *)last_T -> p_data;
        }
        if((r = k_data_comp(a, b)) == K_CMP_BIGGER){
            last_T -> p_right = T_new;
        }else
            last_T -> p_left = T_new;

        /* ���½ڵ�ĸ��ڵ� ����Ϊ last_T */
        T_new -> p_parent = last_T;

    }
    return 0;
}

/* ɾ��һ�����ڵ�, ����Ϊ struct k_tree **node Ϊ�˿���ɾ�� ���ڵ�, ע�⺯���Ӳ���ɾ�� tree.p_data */
int k_tree_delete_handle(struct k_tree **node)
{
    k_tree_delete_node(node);
    
    return 0;
}

/* ������������, ����ֵΪָ�� ���ڵ��ָ��, û�в�ѯ������� NULL */
struct k_tree *k_tree_search_handle(struct k_tree *tree, struct k_data *to_find, struct k_data *k_handle_get_data(void *handle, void *parameter), 
            void *parameter, int (*k_data_comp)(struct k_data *a1, struct k_data *a2))
{
    struct k_tree *T = tree;
    struct k_data *now;
    int r = 0;
    
    while(T != NULL){
        if(k_handle_get_data != NULL){
            now = k_handle_get_data(T -> p_data, parameter);
        }else{
            now = (struct k_data *)T -> p_data;
        }
        r = k_data_comp(to_find, now);
        if(r == K_CMP_BIGGER)
            T = T -> p_right;
        else if(r == K_CMP_SMALLER)
            T = T -> p_left;
        else if(r == K_CMP_EQUAL){
            return T;
        }else if(r == K_CMP_UNKNOWN){
            my_error(fp_error, MODE_ERROR, "<%s> : error happens when comparing!", __FUNCTION__);
            return NULL;
        }
    }
    return NULL;
}

/* ��ӡ��� */
int k_tree_print_handle(struct k_tree *tree, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter)
{
    struct k_data *a;
    void *handle;
    char *str;
    
    handle = tree -> p_data;
    if(k_handle_get_data != NULL){
        a = k_handle_get_data(handle, parameter);
    }else{
        a = (struct k_data *)handle;
    }
    
    str = k_data_convert_to_string_malloc(a);
    if(str != NULL){
        if(tree -> count == 1){
            printf("    | %s\n", str);
            free(str);
        }else{
            printf("    | ", tree -> count);
            k_list_traversal_print_handle(tree -> record, k_handle_get_data, parameter);
        }
    }else{
        my_error(fp_error, MODE_ERROR, "<%s> : error happens!", __FUNCTION__);
        return -1;
    }
    
    return 0;
}

/* ����������handle���в����ĺ���(�ݹ鷨, �ٶ���), ���ṩһ����Ե������ڵ�Ĵ����� */
int k_tree_ergodic_handle(struct k_tree *tree, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter, 
            int process_on_node(struct k_tree *tree, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter))
{
    if(process_on_node == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : no process given !!", __FUNCTION__);
        return -1;
    }
    
    /* ���ڵݹ�ԭ�����ﲻ�ܶ� tree Ϊ NULL ���쳣����, �����м�� tree �Ƿ�Ϊ NULL */ 
    if(tree != NULL && process_on_node != NULL){
        k_tree_ergodic_handle(tree -> p_left, parameter, k_handle_get_data, process_on_node);
        process_on_node(tree, parameter, k_handle_get_data);
        k_tree_ergodic_handle(tree -> p_right, parameter, k_handle_get_data, process_on_node);
    }

    return 0;
}

#endif
