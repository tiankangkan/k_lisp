/**
    name      : k_tree.h
    date      ：2015.07.25
    describe  : 二叉树 操作函数族
    copyright : 你可以自由使用它, 但是不能在不变更版本号的情况下对其作出修改.
    note      : 请注意 所有的释放函数均不会对节点指向的实际数据做释放操作.
    updata    :
        1) 增加了树的释放函数 k_tree_free_all_node().
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
    void *p_data;            /* 这里一般假定 p_data 指向的地址的内存管理不归 k_tree.h 的管辖范围 */
    int   count;             /* 重复添加计数 */
    struct k_list *record;   /* record 用于对添加项做记录, 即使重复的项也会收录 */
    struct k_tree *p_left;   /* 左子节点, 一般是 "小于" 父节点 */
    struct k_tree *p_right;  /* 右子节点, 一般是 "大于" 父节点 */
    struct k_tree *p_parent; /* 父节点 */
};

/* 添加一个项目到 树, 返回 1 代表项目已存在, 当 *tree 为 NULL 时, 将会为其分配内存, 并将其设置为根节点 */
int k_tree_insert_item_to_tree(struct k_tree **tree, void *data, int (*k_tree_comp)(void *a1, void *a2));
/* 删除一个树节点, 参数为 struct k_tree **node 为了可以删除 根节点, 注意函数从不会删除 tree.p_data */
int k_tree_delete_node(struct k_tree **p_node);
/* 释放整棵树 */
int k_tree_free_all_node(struct k_tree *tree);
/* 创建一个树节点(分配内存) */
int k_tree_create_node(struct k_tree **node,  void *data);
/* 强行释放一个树节点所占内存, 当不可释放时返回 -1 */
int tree_free_node(struct k_tree *node);
/* 针对 int 类型的比较函数 */
int k_tree_comp_int(void *a1, void *a2);
/* 针对常规 字符串('\0' 结尾) 的比较函数 */
int k_tree_comp_string(void *a1, void *a2);
/* 树的搜索函数, 返回值为指向 树节点的指针, 没有查询结果返回 NULL */
struct k_tree * k_tree_search(struct k_tree *tree, void *data, int (*k_tree_comp)(void *a1, void *a2));
/* 树遍历函数(递归法, 速度慢, 有序递增执行 k_tree_process_on_single_item() 函数), 需提供一个针对单个树节点的处理方法 */
int k_tree_ergodic(struct k_tree *tree, int (*k_tree_process_on_single_item)(struct k_tree *tree));
/* 输出节点信息, 节点数据为 int 类型 */
int k_tree_print_single_item_int(struct k_tree *tree);
/* 输出节点信息, 节点数据为常规的 字符串类型 */
int k_tree_print_single_item_string(struct k_tree *tree);
/* 将相关性强(例如递增或递减)的一组项目加到树中, k_tree_get_nth_item 返回 第 n 个 p_data */
int k_tree_insert_sorted_items_to_tree(struct k_tree **tree, void *p_sorted_items, int start, int end,
            int (*k_tree_comp)(void *a1, void *a2), void *(*k_tree_get_nth_item)(void *p_sorted_items, int n));

/* 添加一个项目到 树, 返回 1 代表项目已存在, 当 *tree 为 NULL 时, 将会为其分配内存, 并将其设置为根节点 */
int k_tree_insert_handle(struct k_tree **tree, void *handle, struct k_data *k_handle_get_data(void *handle, void *parameter), 
            void *parameter, int (*k_data_comp)(struct k_data *a1, struct k_data *a2));
/* 删除一个树节点, 参数为 struct k_tree **node 为了可以删除 根节点, 注意函数从不会删除 tree.p_data */
int k_tree_delete_handle(struct k_tree **node);
/* 打印句柄 */
int k_tree_print_handle(struct k_tree *tree, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter);
/* 树遍历并对handle进行操作的函数(递归法, 速度慢), 需提供一个针对单个树节点的处理方法 */
int k_tree_ergodic_handle(struct k_tree *tree, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter, 
            int process_on_node(struct k_tree *tree, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter));
/* 树的搜索函数, 返回值为指向 树节点的指针, 没有查询结果返回 NULL */
struct k_tree *k_tree_search_handle(struct k_tree *tree, struct k_data *to_find, struct k_data *k_handle_get_data(void *handle, void *parameter), 
            void *parameter, int (*k_data_comp)(struct k_data *a1, struct k_data *a2));
            
/*************************************************************/
            
/* 添加一个项目到 树, 返回 1 代表项目已存在, 当 *tree 为 NULL 时, 将会为其分配内存, 并将其设置为根节点 */
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

        /* 将新节点加入到 last_T 的子节点中 */
        if((temp = k_tree_comp(data, last_T -> p_data)) > 0){
            last_T -> p_right = T_new;
        }else
            last_T -> p_left = T_new;

        /* 将新节点的父节点 设置为 last_T */
        T_new -> p_parent = last_T;

    }
    return 0;
}

/* 删除一个树节点, 参数为 struct k_tree **node 为了可以删除 根节点, 注意函数从不会删除 tree.p_data */
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

    /* 当 node 仅有一个子节点时 */
    if(node -> p_left == NULL || node -> p_right == NULL){
        /* node_now 为 node 的子节点 */
        node_now = node -> p_left != NULL ? node -> p_left : node -> p_right;

        /* 当 *p_node 不为根节点时, ... */
        if(node -> p_parent != NULL){
            /* 将 node 的父节点指向其子节点 */
            if(node -> p_parent -> p_left == node)
                node_check = &(node -> p_parent -> p_left);
            else
                node_check = &(node -> p_parent -> p_right);
            *node_check = node_now;

            /* 将 node 的子节点(如果有的话), 指向其父节点 */
            if(node_now != NULL)
                node_now -> p_parent = node -> p_parent;
        }else{
            /* 将 node_now(如果不为 NULL的话) 的父节点 设为 NULL */
            if(node_now != NULL)
                node_now -> p_parent = NULL;
            *p_node = node_now;
        }

        /* 释放内存 */
        tree_free_node(node);

        return 0;
    }

    /* 得到前驱节点 */
    node_now = node -> p_left;
    while(node_now -> p_right != NULL){
        node_now = node_now -> p_right;
    }

    /* 交换 node 和 node_now 的 p_data */
    p_data_temp = node -> p_data;
    node -> p_data = node_now -> p_data;
    node_now -> p_data = p_data_temp;

    /* 此时的 node_now 包含了 node 的 p_data 并且 只有一个子节点 */
    k_tree_delete_node(&node_now);

    return 0;
}

/* 释放整棵树 */
int k_tree_free_all_node(struct k_tree *tree)
{
    if(tree == NULL)
        return 0;
    
    k_tree_free_all_node(tree -> p_left);
    k_tree_free_all_node(tree -> p_right);

    k_tree_delete_node(&tree);

    return 0;
}

/* 创建一个树节点(分配内存) */
int k_tree_create_node(struct k_tree **node,  void *data)
{
    *node = (struct k_tree *)malloc(sizeof(struct k_tree));
    (*node) -> p_data = data;
    (*node) -> count = 1;
    (*node) -> p_left = NULL;
    (*node) -> p_right = NULL;
    (*node) -> p_parent = NULL;
    
    /* 插入到记录中 */
    (*node) -> record = NULL;
    k_list_insert_tail_malloc(&(*node) -> record, data); 

    return 0;
}

/* 强行释放一个树节点所占内存, 当不可释放时返回 -1 */
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


/* 针对 int 类型的比较函数 */
int k_tree_comp_int(void *a1, void *a2)
{
    int r;
    
    r = *(int *)a1 - *(int *)a2;
    if(r == 0)
        return K_CMP_EQUAL;
    else
        return r > 0 ? K_CMP_BIGGER : K_CMP_SMALLER;
}

/* 针对常规 字符串('\0' 结尾) 的比较函数 */
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
        return K_CMP_SMALLER;    /* 小于 */
    else if(string_a[i] - string_b[i] == 0)
        return K_CMP_EQUAL;
    else
        return string_a[i] - string_b[i] > 0 ? K_CMP_BIGGER : K_CMP_SMALLER;
}

/* 树的搜索函数, 返回值为指向 树节点的指针, 没有查询结果返回 NULL */
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

/* 树遍历函数(递归法, 速度慢), 需提供一个针对单个树节点的处理方法 */
int k_tree_ergodic(struct k_tree *tree, int (*k_tree_process_on_single_item)(struct k_tree *tree))
{
    if(tree != NULL){
        k_tree_ergodic(tree -> p_left, k_tree_process_on_single_item);
        k_tree_process_on_single_item(tree);
        k_tree_ergodic(tree -> p_right, k_tree_process_on_single_item);
    }

    return 0;
}

/* 输出节点信息, 节点数据为 int 类型 */
int k_tree_print_single_item_int(struct k_tree *tree)
{
    printf("count = %4d, data = %d\n", tree -> count, *(int *)(tree -> p_data));

    return 0;
}

/* 输出节点信息, 节点数据为常规的 字符串类型 */
int k_tree_print_single_item_string(struct k_tree *tree)
{
    printf("count = %4d, data = %s, father = %s\n", tree -> count, (char *)(tree -> p_data), tree -> p_parent != NULL ? (char *)(tree -> p_parent -> p_data) : NULL);
    //printf("count = %4d, data = %s\n", tree -> count, (char *)(tree -> p_data));
    return 0;
}

/* 将相关性强(例如递增或递减)的一组项目加到树中 */
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

    /* 加入 zone 部分 */
    for(i = depth - 1; i >= 0; i --){
        level_start = (int)pow(2, i);
        step = (int)pow(2, i + 1);
        for(j = level_start; j <= zone - level_start + 1; j += step){
            data = k_tree_get_nth_item(p_sorted_items, j + start + head - 1);
            k_tree_insert_item_to_tree(tree, data, k_tree_comp);
        }
    }

    /* 加入头部项目 */
    for(i = 0; i < head; i ++){
        data = k_tree_get_nth_item(p_sorted_items, i);
        k_tree_insert_item_to_tree(tree, data, k_tree_comp);
    }
    /* 加入尾部项目 */
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

/* 添加一个项目到 树, 返回 1 代表项目已存在, 当 *tree 为 NULL 时, 将会为其分配内存, 并将其设置为根节点 */
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
                return -1;    /* 比较出错, 可能是不兼容的类型 */
            }
        }

        k_tree_create_node(&T_new, handle);

        /* 将新节点加入到 last_T 的子节点中 */
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

        /* 将新节点的父节点 设置为 last_T */
        T_new -> p_parent = last_T;

    }
    return 0;
}

/* 删除一个树节点, 参数为 struct k_tree **node 为了可以删除 根节点, 注意函数从不会删除 tree.p_data */
int k_tree_delete_handle(struct k_tree **node)
{
    k_tree_delete_node(node);
    
    return 0;
}

/* 树的搜索函数, 返回值为指向 树节点的指针, 没有查询结果返回 NULL */
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

/* 打印句柄 */
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

/* 树遍历并对handle进行操作的函数(递归法, 速度慢), 需提供一个针对单个树节点的处理方法 */
int k_tree_ergodic_handle(struct k_tree *tree, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter, 
            int process_on_node(struct k_tree *tree, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter))
{
    if(process_on_node == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : no process given !!", __FUNCTION__);
        return -1;
    }
    
    /* 出于递归原因，这里不能对 tree 为 NULL 做异常处理, 请自行检查 tree 是否为 NULL */ 
    if(tree != NULL && process_on_node != NULL){
        k_tree_ergodic_handle(tree -> p_left, parameter, k_handle_get_data, process_on_node);
        process_on_node(tree, parameter, k_handle_get_data);
        k_tree_ergodic_handle(tree -> p_right, parameter, k_handle_get_data, process_on_node);
    }

    return 0;
}

#endif
