/**
@File      : k_link.h
@Author    : kangtian
@Date      : 15-09-17
@Describle : link 的含义是连接, 在本文件中，也被称为 list。
             其为一种自省类型的数据结构, 用来描述数据的关系。 
@Note      : 
*/
#ifndef __K_LINK_H__
    #define __K_LINK_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "k_error.h"
#include "k_data.h"
#include "k_string.h"

#define K_LINE_FREE_MODE_SAFE     0    /* 保留 p_data */
#define K_LINE_FREE_MODE_CLEAN    1    /* 释放 P_data */ 

struct k_link{
    void *p_data;                      /* 携带的数据 */
    int   type;                        /* 数据类型, 使用在 k_data.h 中的定义 */
    struct k_link *pre_node;           /* 前一个节点 */
    struct k_link *next_node;          /* 后一个节点 */
    struct k_link *this_node;          /* 子节点 */
};

/* 内存分配函数 */
int k_link_node_malloc(struct k_link **node, void *p_data, int type); 
/* 内存分配函数 */
struct k_link *k_link_node_just_malloc(void *p_data, int type);
/* 拷贝函数, 拷贝 k_link 和 k_data 及 link信息 */
struct k_link *k_link_node_copy(struct k_link *node);
/* 拷贝函数, 拷贝整个 list */
struct k_link *k_link_node_copy_list_all(struct k_link **dest, struct k_link *src);
/* 拷贝函数, 不拷贝 link 信息 */
struct k_link *k_link_node_copy_whithout_link(struct k_link *node); 
/* 拷贝函数, 将 src 的 k_data 及 link 信息拷贝到 list this_node 尾部 */
struct k_link *k_link_node_copy_to_this_tail(struct k_link *list, struct k_link *src); 
/* 拷贝函数, 拷贝 k_link 和 k_data */
struct k_link *k_link_node_copy_whithout_link(struct k_link *node);
/* 释放(删除)节点内存, 注意这里不释放节点指针 */
int k_link_free_node(struct k_link *node);
/* 释放节点所在列表所有节点 */
int k_link_free_all(struct k_link *node);
/* 释放节点所有子节点 */
int k_link_free_list(struct k_link *node);
/* 得到与 node 相比, 偏移 offset 的节点, next_node 为正偏移 */
struct k_link *k_link_get_node_offset(struct k_link *node, int offset);
/* 交换节点数据, 节点本身未涉及内存变更 */
int k_link_node_exchange(struct k_link *a, struct k_link *b);
/* 以 node 替换节点 list_node, 但是对替换下来的 list_node 没有任何处理 */
int k_link_node_substitute(struct k_link *list_node, struct k_link *node); 
/* 得到列表 Root */
struct k_link *k_link_get_root(struct k_link *list);
/* 得到列表头 */
struct k_link *k_link_get_head(struct k_link *list);
/* 得到列表尾 */
struct k_link *k_link_get_tail(struct k_link *list);
/* 得到列表节点个数 */
int k_link_nodes_in_list(struct k_link *list);
/* 将 node 插入 list_node 之后 */
int k_link_insert_after(struct k_link *list_node, struct k_link *node);
/* 将 node 插入 list_node 之前 */
int k_link_insert_before(struct k_link *list_node, struct k_link *node);
/* 将 node 插入 list_node 的头部 */
int k_link_insert_head(struct k_link *list_node, struct k_link *node);
/* 将 node 插入 list_node 的尾部 */
int k_link_insert_tail(struct k_link *list_node, struct k_link *node);
/* 将 node_src 插入 node_dest 的 this_node 分支尾部 */
int k_link_insert_this_tail(struct k_link *node_dest, struct k_link *node_src);
/* 将 data 插入 list_node 的头部并分配内存 */
struct k_link *k_link_insert_head_malloc(struct k_link **list_node, void *data, int type);
/* 将 data 插入 node 的后面并分配内存 */
struct k_link *k_link_insert_after_malloc(struct k_link **node, void *data, int type);
/* 将 data 插入 list_node 的尾部并分配内存 */
struct k_link *k_link_insert_tail_malloc(struct k_link **list_node, void *data, int type); 
/* 将 data 插入 list_node -> this_node 的头部并分配内存 */
struct k_link *k_link_insert_this_head_malloc(struct k_link *list_node, void *data, int type);
/* 将 data 插入 list_node -> this_node 的尾部并分配内存 */
struct k_link *k_link_insert_this_tail_malloc(struct k_link *list_node, void *data, int type);
/* 将 list 遍历打印, 将 list -> node 做 handle 处理 */
int k_link_traversal_print_list_handle(struct k_link *list_node, int indent_width, int indents, 
            struct k_data *k_data_get_value(void *handle, void *parameter), void *parameter);
/* 将 list 遍历打印, 仅打印子节点*/
int k_link_traversal_print_child_handle(struct k_link *list_node, int indent_width, int indents, 
            struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter);
/* 打印 list_node, 将 list -> node 做 handle 处理 */
int k_link_print_node_handle(struct k_link *list_node, 
            struct k_data *k_data_get_value(void *handle, void *parameter), void *parameter);


/*****************************************************/

/* 内存分配函数 */
int k_link_node_malloc(struct k_link **node, void *p_data, int type)
{
    struct k_link *n;
    
    if(node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return -1;
    }
    
    *node = (struct k_link *)malloc(sizeof(struct k_link));
    n = *node;
    
    n -> pre_node = NULL;
    n -> next_node = NULL;
    n -> this_node = NULL;
    n -> p_data = p_data;
    n -> type = type;
    
    return 0;
}

/* 内存分配函数 */
struct k_link *k_link_node_just_malloc(void *p_data, int type)
{
    struct k_link *n;
    
    n = (struct k_link *)malloc(sizeof(struct k_link));
    
    n -> pre_node = NULL;
    n -> next_node = NULL;
    n -> this_node = NULL;
    n -> p_data = p_data;
    n -> type = type;

    return n;
}

/* 拷贝函数, 拷贝 k_link 和 k_data 及 link信息 */
struct k_link *k_link_node_copy(struct k_link *node)
{
    struct k_link *dest;
    
    dest = k_link_node_just_malloc(node -> p_data, node -> type);
    *dest = *node;    /* copy k_link 节点关系信息 */
    k_data_copy((struct k_data **)&dest -> p_data, (struct k_data *)node -> p_data);
    
    return dest;
}

/* 拷贝函数, 拷贝整个 list */
struct k_link *k_link_node_copy_list_all(struct k_link **dest, struct k_link *src)
{
    struct k_link *now, *last, *temp;
    struct k_data *data;
    
    if(src == NULL){
        return NULL;
    }
    
    now = src;
    while(now != NULL){
        //printf("~~~~~~~~~~~~\n");
        last = k_link_insert_tail_malloc(dest, now -> p_data, now -> type); 
        k_data_copy(&data, (struct k_data *)now -> p_data);
        last -> p_data = data;
        k_link_node_copy_list_all(&last -> this_node, now -> this_node);

        now = now -> next_node;
    }
    
    return 0;
}

/* 拷贝函数, 将 src 的 k_data 及 link 信息拷贝到 list this_node 尾部 */
struct k_link *k_link_node_copy_to_this_tail(struct k_link *list, struct k_link *src)
{
    struct k_link *dest;
    
    if(list == NULL || src == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return NULL;
    }
    
    dest = k_link_node_copy(src);
    k_link_insert_this_tail(list, dest);
    
    return dest;
}

/* 拷贝函数, 不拷贝 link 信息 */
struct k_link *k_link_node_copy_whithout_link(struct k_link *node)
{
    struct k_link *dest;
    
    dest = k_link_node_just_malloc(node -> p_data, node -> type);
    k_data_copy((struct k_data **)&dest -> p_data, (struct k_data *)node -> p_data);
    
    return dest;
}

/* 释放(删除)节点内存 */
int k_link_free_node(struct k_link *node)
{
    struct k_link *n;
    
    if(node == NULL){
        return 0;
    }
    
    n = node;
    if(n -> pre_node != NULL){
        /* 指针校正 */
        n -> pre_node -> next_node = n -> next_node;
        if(n -> next_node != NULL){
            (n -> next_node) -> pre_node = n -> pre_node;
        }
    }else{
        if(n -> next_node != NULL)
            (n -> next_node) -> pre_node = NULL;
    }
    
    k_link_free_list(n -> this_node);
    free(n);
    
    return 0;
}

/* 将 list 遍历释放 node */
int k_link_traversal_free(struct k_link *list_node)
{
    struct k_link *now, *temp;
    struct k_data *data;
    
    if(list_node != NULL){
        now = k_link_get_head(list_node);
    }else{
        return 0;
    }
    
    while(now != NULL){
        temp = now -> next_node;
        k_link_traversal_free(now -> this_node);
        free(now);
        now = temp;
    }
    
    return 0;
}

/* 释放节点所在列表所有节点 */
int k_link_free_all(struct k_link *node)
{
    struct k_link *root;
    
    if(node == NULL){
        return 0;
    }
    
    root = k_link_get_root(node);
    k_link_traversal_free(root);
    
    return 0;
}

/* 释放节点所有子节点 */
int k_link_free_list(struct k_link *node)
{
    struct k_link *tail, *head, *tmp;
    
    if(node == NULL){
        return 0;
    }
    
    tail = k_link_get_tail(node);
    head = k_link_get_head(node);
    while(tail != head){
        /* 释放节点内存 */
        tmp = tail -> pre_node;
        k_link_free_node(tail);
        tail = tmp;
    }
    k_link_free_node(head);
    
    return 0;
}

/* 得到与 node 相比, 偏移 offset 的节点, next_node 为正偏移 */
struct k_link *k_link_get_node_offset(struct k_link *node, int offset)
{
    int i;
    
    if(node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return NULL;
    }
    
    if(offset > 0){
        for(i = 0; i < offset; i ++){
            if(node == NULL)
                break;
            node = node -> next_node;
        }
    }else{
        for(i = 0; i < -offset; i ++){
            if(node == NULL)
                break;
            node = node -> pre_node;
        }
    }
    
    return node;
}

/* 得到 list head 相比, offset 必须大于 0 */
struct k_link *k_link_get_head_offset(struct k_link *node, int index)
{
    struct k_link *n, *head;
    
    if(index < 0){
        my_error(fp_error, MODE_ERROR, "error: <%s> : index must >= 0 !", __FUNCTION__);
        return NULL;
    }
    if(node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return NULL;
    }
    
    head = k_link_get_head(node);
    return k_link_get_node_offset(head, index);
}

/* 交换节点数据, 节点本身未涉及内存变更 */
int k_link_node_exchange(struct k_link *a, struct k_link *b)
{
    void *pdata_temp;
    int type_temp;
    
    if(a == NULL || b == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return -1;
    }
    
    pdata_temp = a -> p_data;
    a -> p_data = b -> p_data;
    b -> p_data = pdata_temp;
    
    type_temp = a -> type;
    a -> type = b -> type;
    b -> type = type_temp;
    
    return 0;
}
/*****************  TODO !!!!!!!!!!!!!!!!!!!!!  ***************/
/* 以 node 替换节点 list_node(注意参数位置不能颠倒), 但是对替换下来的 list_node 没有任何处理 */
int k_link_node_substitute(struct k_link *list_node, struct k_link *node)
{
    void *pdata_temp;
    int type_temp;
    
    if(list_node == NULL || node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return -1;
    }
    
    if(list_node -> pre_node != NULL && list_node -> pre_node -> next_node == list_node){
        list_node -> pre_node -> next_node = node;
        node -> pre_node = list_node -> pre_node;
    }else if(list_node -> pre_node != NULL && list_node -> pre_node -> this_node == list_node){
        list_node -> pre_node -> this_node = node;
        node -> pre_node = list_node -> pre_node;
    }else{
        node -> pre_node = NULL;
    }
//    else{
//        node -> pre_node = list_node -> pre_node;
//    }
    
    if(list_node -> next_node != NULL){
        list_node -> next_node -> pre_node = node;
    }
    node -> next_node = list_node -> next_node;

    
    return 0;
}

/* 得到列表 Root */
struct k_link *k_link_get_root(struct k_link *list)
{
    struct k_link *root;
    
    if(list == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return NULL;
    }
    
    /* 移到列表首部 */
    while(list -> pre_node != NULL){
        list = list -> pre_node;
    }
    root = list;
    
    return root;
}

/* 得到列表头 */
struct k_link *k_link_get_head(struct k_link *list)
{
    struct k_link *head;
    
    if(list == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return NULL;
    }
    
    /* 移到列表首部 */
    while(list -> pre_node != NULL && list -> pre_node -> next_node == list){
        list = list -> pre_node;
    }
    head = list;
    
    return head;
}

/* 得到列表尾 */
struct k_link *k_link_get_tail(struct k_link *list)
{
    struct k_link *tail;
    
    if(list == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return NULL;
    }
    
    /* 移到列表尾部 */
    while(list -> next_node != NULL){
        list = list -> next_node;
    }
    tail = list;
    
    return tail;
}

/* 得到列表节点个数 */
int k_link_nodes_in_list(struct k_link *list)
{
    int count;
    struct k_link *head;
    
    if(list == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return -1;
    }
    
    head = list -> this_node;
    count = 0;
    while(head != NULL){
        head = head -> next_node;
        count ++;
    }
    
    return count;
}

/* 将 node 插入 list_node 之后 */
int k_link_insert_after(struct k_link *list_node, struct k_link *node)
{
    struct k_link *temp;
    
    if(list_node == NULL || node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return -1;
    }
    
    temp = list_node -> next_node;
    list_node -> next_node = node;
    node -> pre_node = list_node;
    node -> next_node = temp;
    if(temp != NULL){
        temp -> pre_node = node;
    }
    
    return 0; 
}

/* 将 node 插入 list_node 之前 */
int k_link_insert_before(struct k_link *list_node, struct k_link *node)
{
    struct k_link *temp;
    
    if(list_node == NULL || node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return -1;
    }
    
    temp = list_node -> pre_node;
    list_node -> pre_node = node;
    node -> pre_node = temp;
    node -> next_node = list_node;
    if(temp != NULL){
        temp -> next_node = node;
    }
    
    return 0;
}

/* 将 node 插入 list_node 的头部 */
int k_link_insert_head(struct k_link *list_node, struct k_link *node)
{
    if(list_node == NULL || node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return -1;
    }
    k_link_insert_before(k_link_get_head(list_node), node);
    
    return 0; 
}

/* 将 node_src 插入 node_dest 的 this_node 分支尾部 */
int k_link_insert_this_tail(struct k_link *node_dest, struct k_link *node_src)
{
    if(node_dest == NULL || node_src == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return -1;
    }
    
    if(node_dest -> this_node != NULL){
        k_link_insert_after(k_link_get_tail(node_dest -> this_node), node_src);
    }else{
        node_dest -> this_node = node_src;
        node_src -> pre_node = node_dest;
    }
    
    return 0; 
}

/* 将 node 插入 list_node 的尾部 */
int k_link_insert_tail(struct k_link *list_node, struct k_link *node)
{
    if(list_node == NULL || node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return -1;
    }
    
    k_link_insert_after(k_link_get_tail(list_node), node);
    
    return 0; 
}

/* 将 data 插入 list_node 的头部并分配内存 */
struct k_link *k_link_insert_head_malloc(struct k_link **list_node, void *data, int type)
{
    struct k_link *head, *node;
    
    if(list_node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return NULL;
    }
    
    if(*list_node == NULL){
        k_link_node_malloc(list_node, data, type);
    }else{
        k_link_node_malloc(&node, data, type);
        head = k_link_get_head(*list_node);
        k_link_insert_before(head, node);
    }
    
    return node;
}

/* 将 data 插入 node 的后面并分配内存 */
struct k_link *k_link_insert_after_malloc(struct k_link **node, void *data, int type)
{
    struct k_link *new_node;
    
    if(node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return NULL;
    }
    
    if(*node == NULL){
        k_link_node_malloc(node, data, type);
    }else{
        k_link_node_malloc(&new_node, data, type);
        k_link_insert_after(*node, new_node);
    }
    
    return new_node;
}

/* 将 data 插入 list_node 的尾部并分配内存 */
struct k_link *k_link_insert_tail_malloc(struct k_link **list_node, void *data, int type)
{
    struct k_link *tail, *node;
    
    if(list_node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return NULL;
    }
    
    if(*list_node == NULL){
        k_link_node_malloc(list_node, data, type);
        return *list_node;
    }else{
        k_link_node_malloc(&node, data, type);
        tail = k_link_get_tail(*list_node);
        k_link_insert_after(tail, node);
    }
    
    return node; 
}

/* 将 data 插入 list_node -> this_node 的头部并分配内存 */
struct k_link *k_link_insert_this_head_malloc(struct k_link *list_node, void *data, int type)
{
    struct k_link *node, *head, *temp_next;
    
    if(list_node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return NULL;
    }
    
    k_link_node_malloc(&node, data, type);
    
    if(list_node -> this_node == NULL){
        list_node -> this_node = node;
        node -> pre_node = list_node;
    }else{
        head = list_node -> this_node;
        temp_next = list_node -> next_node;
        k_link_insert_before(head, node);
        list_node -> this_node = list_node -> next_node;
        list_node -> next_node = temp_next;
        
    }

    return node;
}

/* 将 data 插入 list_node -> this_node 的尾部并分配内存 */
struct k_link *k_link_insert_this_tail_malloc(struct k_link *list_node, void *data, int type)
{
    struct k_link *node, *tail;
    
    if(list_node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return NULL;
    }
    
    k_link_node_malloc(&node, data, type);
    
    if(list_node -> this_node == NULL){
        list_node -> this_node = node;
        node -> pre_node = list_node;
    }else{
        tail = k_link_get_tail(list_node -> this_node);
        k_link_insert_after(tail, node);
    }

    return node;
}

/* 将 list_node 所在的 list 节点遍历打印, 将 list -> node 做 handle 处理 */
int k_link_traversal_print_list_handle(struct k_link *list_node, int indent_width, int indents, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter)
{
    struct k_data *data;
    struct k_link *now;
    char indent_buf[1024];
    char *str;
    
    memset(indent_buf, ' ', indents * indent_width);
    indent_buf[indents * indent_width] = '\0';
    
    if(list_node != NULL)
        now = k_link_get_head(list_node);
    else
        return 0;
    
    while(now != NULL){
        if(k_handle_get_data != NULL){
            data = k_handle_get_data(now -> p_data, parameter);
        }else{
            data = (struct k_data *)now -> p_data;
        }
        str = k_data_convert_to_string_malloc(data);
        if(str != NULL){
            printf("  %s| %s\n", indent_buf, str);
            free(str);
        }else{
            my_error(fp_error, MODE_ERROR, "error: <%s> : convert return NULL !", __FUNCTION__);
        }
        k_link_traversal_print_list_handle(now -> this_node, indent_width, indents + 1, k_handle_get_data, parameter);

        now = now -> next_node;
    }
    
    return 0;
}

/* 将 list 全部节点遍历打印, 将 list -> node 做 handle 处理 */
int k_link_traversal_print_all_handle(struct k_link *list_node, int indent_width, int indents, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter)
{
    struct k_data *data;
    struct k_link *now;
    char indent_buf[1024];
    char *str;
    
    memset(indent_buf, ' ', indents * indent_width);
    indent_buf[indents * indent_width] = '\0';
    
    if(list_node != NULL)
        now = k_link_get_root(list_node);
    else
        return 0;
    
    while(now != NULL){
        if(k_handle_get_data != NULL){
            data = k_handle_get_data(now -> p_data, parameter);
        }else{
            data = (struct k_data *)now -> p_data;
        }
        str = k_data_convert_to_string_malloc(data);
        if(str != NULL){
            printf("  %s| %s\n", indent_buf, str);
            free(str);
        }else{
            my_error(fp_error, MODE_ERROR, "error: <%s> : convert return NULL !", __FUNCTION__);
        }
        k_link_traversal_print_child_handle(now, indent_width, indents + 1, k_handle_get_data, parameter);

        now = now -> next_node;
    }
    
    return 0;
}

/* 将 list 的子节点遍历打印, 仅打印子节点*/
int k_link_traversal_print_child_handle(struct k_link *list_node, int indent_width, int indents, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter)
{
    struct k_data *data;
    struct k_link *now;
    char indent_buf[1024];
    char *str;
    
    memset(indent_buf, ' ', indents * indent_width);
    indent_buf[indents * indent_width] = '\0';
    
    if(list_node != NULL)
        now = list_node -> this_node;
    else
        return 0;
    
    while(now != NULL){
        if(k_handle_get_data != NULL){
            data = k_handle_get_data(now -> p_data, parameter);
        }else{
            data = (struct k_data *)now -> p_data;
        }
        str = k_data_convert_to_string_malloc(data);
        if(str != NULL){
            printf("  %s| %s\n", indent_buf, str);
            free(str);
        }else{
            my_error(fp_error, MODE_ERROR, "error: <%s> : convert return NULL !", __FUNCTION__);
        }
        k_link_traversal_print_child_handle(now, indent_width, indents + 1, k_handle_get_data, parameter);

        now = now -> next_node;
    }
    
    return 0;
}

/* 打印 list_node, 将 list -> node 做 handle 处理 */
int k_link_print_node_handle(struct k_link *list_node, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter)
{
    struct k_data *data;
    char *str;
    
    //printf("print_node = %d\n", list_node);
    if(list_node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return -1;
    }

    if(k_handle_get_data != NULL){
        data = k_handle_get_data(list_node -> p_data, parameter);
    }else{
        data = (struct k_data *)(list_node -> p_data);
    }
    str = k_data_convert_to_string_malloc(data);
    if(str != NULL){
        printf("node: %s\n", str);
        free(str);
    }else{
        my_error(fp_error, MODE_ERROR, "error: <%s> : convert return NULL !", __FUNCTION__);
    }

    return 0;
}

/* 将 list 的子节点遍历打印, 仅打印子节点*/
int k_link_traversal_sprint_child_handle(struct k_link *list_node, char *buf, int indent_width, int indents, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter)
{
    struct k_data *data;
    struct k_link *now;
    char indent_buf[1024];
    char *str_data;
    char temp[K_STRING_MAX_BYTES];
    
    memset(indent_buf, ' ', indents * indent_width);
    indent_buf[indents * indent_width] = '\0';
    
    if(list_node != NULL)
        now = list_node;
    else
        return 0;
    
    while(now != NULL){
        if(k_handle_get_data != NULL){
            data = k_handle_get_data(now -> p_data, parameter);
        }else{
            data = (struct k_data *)now -> p_data;
        }
        str_data = k_data_convert_to_string_malloc(data);
        if(str_data != NULL){
            if(now -> this_node == NULL)
                sprintf(temp, "  %s| %s\n", indent_buf, str_data);
            else
                sprintf(temp, "  %s+ %s\n", indent_buf, str_data);
            strcat(buf, temp);
            free(str_data);
        }else{
            my_error(fp_error, MODE_ERROR, "error: <%s> : convert return NULL !", __FUNCTION__);
        }
        k_link_traversal_sprint_child_handle(now -> this_node, buf, indent_width, indents + 1, k_handle_get_data, parameter);

        now = now -> next_node;
    }
    
    return 0;
}

/* 打印 list_node, 将结果输出到 buf 尾部  */
int k_link_print_sprint_node_handle(struct k_link *list_node, char *buf, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter)
{
    struct k_data *data;
    char *str;
    
    //printf("print_node = %d\n", list_node);
    if(list_node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : some pointer is NULL !", __FUNCTION__);
        return -1;
    }

    if(k_handle_get_data != NULL){
        data = k_handle_get_data(list_node -> p_data, parameter);
    }else{
        data = (struct k_data *)(list_node -> p_data);
    }
    str = k_data_convert_to_string_malloc(data);
    if(str != NULL){
        //sprintf(temp, "  %s| %s\n", indent_buf, str_data);
        printf("node: %s\n", str);
        free(str);
    }else{
        my_error(fp_error, MODE_ERROR, "error: <%s> : convert return NULL !", __FUNCTION__);
    }

    return 0;
}

/* 将 list 遍历释放 handle */
int k_link_traversal_free_handle(struct k_link *list_node, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter)
{
    struct k_link *now;
    struct k_data *data;
    
    if(list_node != NULL){
        now = k_link_get_head(list_node);
    }else{
        return 0;
    }
    
    while(now != NULL){
        if(k_handle_get_data != NULL){
            data = k_handle_get_data(now -> p_data, parameter);
        }else{
            data = (struct k_data *)now -> p_data;
        }
        
        k_data_free(data);
        k_link_traversal_free_handle(now -> this_node, k_handle_get_data, parameter);
        now = now -> next_node;
    }
    
    return 0;
}

int k_link_get_index(int *record, int *num, int *index_get, int *offset)
{
    int last_insert;
    int index;
    
    if(*num <= 0){
        return 0;
    }
    
    index = *index_get;
    
    last_insert = record[*num - 1];
    (*num) --;

    //printf("i = %d, get = [%d, %d]\n",index, *index_get, *offset);

    if(index < last_insert){
        k_link_get_index(record, num, index_get, offset);;
    }else if(index > last_insert){
        (*index_get) --;
        k_link_get_index(record, num, index_get, offset);
    }else if(index == last_insert){
        (*offset) --;
        k_link_get_index(record, num, index_get, offset);
    }
    return 0;
}

#endif
