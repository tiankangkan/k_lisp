/**
@File      : k_list.h
@Author    : kangtian
@Date      : 15-09-17
@Describle : ���� 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "k_error.h"
#include "k_data.h"

#ifndef __k_LIST_H__
    #define __k_LIST_H__

#define _DEBUG    0

struct k_list{
    void *p_data;                        /* Я�������� */
    struct k_list *pre_node;             /* ǰһ���ڵ� */ 
    struct k_list *next_node;            /* ��һ���ڵ� */ 
};

/* �ڴ���亯�� */
int k_list_node_malloc(struct k_list **node, void *p_data);
/* �� data ���� list_node ��ͷ�� */
int k_list_insert_head_malloc(struct k_list **list_node, void *p_data); 
/* �� data ���� list_node ��β�� */
int k_list_insert_tail_malloc(struct k_list **list_node, void *data);
/* �ͷ�(ɾ��)�ڵ��ڴ� */
int k_list_node_free(struct k_list *node);
/* �ͷŽڵ������б����нڵ� */
int k_list_free_all(struct k_list **node);
/* �õ��б�ͷ */
struct k_list *k_list_get_head(struct k_list *list);
/* �õ��б�β */
struct k_list *k_list_get_tail(struct k_list *list);
/* �� node ���� list_node ֮�� */
int k_list_insert_after(struct k_list *list_node, struct k_list *node);
/* �� node ���� list_node ֮ǰ */
int k_list_insert_before(struct k_list *list_node, struct k_list *node);
/* �� node ���� list_node ��β�� */
int k_list_insert_tail(struct k_list *list_node, struct k_list *node);
/* �� list ���� */
int k_list_traversal(struct k_list *list_node); 
/* �� list ��������ӡ handle */
int k_list_traversal_print_handle(struct k_list *list_node, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter); 

/*************************************************************/

/* �ڴ���亯�� */
int k_list_node_malloc(struct k_list **node, void *p_data)
{
    struct k_list *n;
    
    *node = (struct k_list *)malloc(sizeof(struct k_list));
    n = *node;
    
    n -> pre_node = NULL;
    n -> next_node = NULL;
    n -> p_data = p_data;
    
    return 0;
}

/* �� data ���� list_node ��ͷ�� */
int k_list_insert_head_malloc(struct k_list **list_node, void *p_data)
{
    struct k_list *head, *new_node;
    
    if(list_node != NULL && *list_node == NULL){
        k_list_node_malloc(list_node, p_data);
    }else if(list_node != NULL && *list_node != NULL){
        k_list_node_malloc(&new_node, p_data);
        head = k_list_get_head(*list_node);
        k_list_insert_before(head, new_node);
    }else{
        return -1;
    }
    
    return 0;
}
/* �� data ���� list_node ��β�� */
int k_list_insert_tail_malloc(struct k_list **list_node, void *p_data)
{
    struct k_list *tail, *new_node;
    
    if(list_node != NULL && *list_node == NULL){
        k_list_node_malloc(list_node, p_data);
    }else if(list_node != NULL && *list_node != NULL){
        k_list_node_malloc(&new_node, p_data);
        tail = k_list_get_tail(*list_node);
        k_list_insert_after(tail, new_node);
    }else{
        return -1;
    }
    
    return 0;
}


/* �ͷ�(ɾ��)�ڵ��ڴ� */
int k_list_node_free(struct k_list *node)
{
    struct k_list *n;
    
    if(node == NULL){
         return 0;
    }
    
    n = node;

    if(n -> pre_node != NULL){
        /* ָ��У�� */
        (n -> pre_node) -> next_node = n -> next_node;
        if(n -> next_node != NULL)
            (n -> next_node) -> pre_node = n -> pre_node;
    }else{
        if(n -> next_node != NULL)
            (n -> next_node) -> pre_node = NULL;
    }

    free(node);
    
    return 0;
}

/* �ͷŽڵ������б����нڵ� */
int k_list_free_all(struct k_list **node)
{
    struct k_list *tail, *temp;
    
    if(node == NULL || *node == NULL){
         return 0;
    }
    
    tail = k_list_get_tail(*node);
    /* �Ƶ��б��ײ� */
    while(tail != NULL){
        /* �ͷŽڵ��ڴ� */
        temp = tail -> pre_node;
        k_list_node_free(tail);
        //printf("free ~\n");
        tail = temp;
    }
    
    *node = NULL;
    return 0;
}

/* �õ��б�ͷ */
struct k_list *k_list_get_head(struct k_list *list)
{
    struct k_list *head;
    head = NULL;
    
    if(list == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : list point to NULL !", __FUNCTION__);
        return NULL;
    }
    
    /* �Ƶ��б��ײ� */
    while(list -> pre_node != NULL){
        list = list -> pre_node;
    }
    
    head = list;
    return head;
}

/* �õ��б�β */
struct k_list *k_list_get_tail(struct k_list *list)
{
    struct k_list *tail;
    tail = NULL;
    
    if(list == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : list point to NULL !", __FUNCTION__);
        return NULL;
    }
    
    /* �Ƶ��б�β�� */
    while(list -> next_node != NULL){
        // printf("get tail: %s\n", list -> identify);
        list = list -> next_node;
    }
    
    tail = list;
    return tail;
}

/* �� node ���� list_node ֮�� */
int k_list_insert_after(struct k_list *list_node, struct k_list *node)
{
    struct k_list *temp;
    
    if(list_node == NULL || node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : list_node or node point to NULL !", __FUNCTION__);
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

/* �� node ���� list_node ֮ǰ */
int k_list_insert_before(struct k_list *list_node, struct k_list *node)
{
    struct k_list *temp;
    
    if(list_node == NULL || node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : list_node or node point to NULL !", __FUNCTION__);
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

/* �� node ���� list_node ��ͷ�� */
int k_list_insert_head(struct k_list *list_node, struct k_list *node)
{
    if(list_node == NULL || node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : list_node or node point to NULL !", __FUNCTION__);
        return -1;
    }
    k_list_insert_before(k_list_get_head(list_node), node);
    
    return 0; 
}

/* �� node ���� list_node ��β�� */
int k_list_insert_tail(struct k_list *list_node, struct k_list *node)
{
    if(list_node == NULL || node == NULL){
        my_error(fp_error, MODE_ERROR, "error: <%s> : list_node or node point to NULL !", __FUNCTION__);
        return -1;
    }
    k_list_insert_after(k_list_get_tail(list_node), node);
    
    return 0; 
}

/* �� list ��������ӡ handle */
int k_list_traversal_print_handle(struct k_list *list_node, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter)
{
    struct k_list *tail;
    struct k_data *data;
    char *str;
    
    if(list_node == NULL){
        return 0;
    }
    
    tail = k_list_get_tail(list_node);
    
    while(tail != NULL){
        if(k_handle_get_data != NULL){
            data = k_handle_get_data(tail -> p_data, parameter);
        }else{
            data = (struct k_data *)tail -> p_data;
        }
        str = k_data_convert_to_string_malloc(data);
        printf("%s -> ", str);
        tail = tail -> pre_node;
    }
    printf("nil\n");
    
    return 0;
}

/* �� list �������ͷ� handle */
int k_list_traversal_free_handle(struct k_list *list_node, struct k_data *k_handle_get_data(void *handle, void *parameter), void *parameter)
{
    struct k_list *tail;
    struct k_data *data;
    char *str;
    
    if(list_node == NULL){
        return 0;
    }
    
    tail = k_list_get_tail(list_node);
    
    while(tail != NULL){
        if(k_handle_get_data != NULL){
            data = k_handle_get_data(tail -> p_data, parameter);
        }else{
            data = (struct k_data *)tail -> p_data;
        }
        k_data_free(data);
        tail = tail -> pre_node;
    }
    
    return 0;
}

/* �� list ����ģ�� */
int k_list_print_string(struct k_list *list_node)
{
    struct k_list *head;
    
    if(list_node == NULL){
        return 0;
    }

    head = k_list_get_head(list_node);
    
    while(head != NULL){
        printf("%s -> ", head -> p_data);
        head = head -> next_node;
    }
    printf("nil\n");
    
    return 0;
}

#endif

