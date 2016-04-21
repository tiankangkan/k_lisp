/**
@File      : k_array.h
@Author    : kangtian
@Date      : 15-09-25
@Describle : �ɱ�����
@Note      : ��� k_array_set(array, array -> pos, data) ��ʵ�ֽ� data �ŵ���һ��λ��. 
*/
#ifndef __k_ARRAY_H__
    #define __k_ARRAY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "k_error.h"


/* �˺겻��ȡ����ǰû�з���λ�õ�ָ�� */
#define k_ARRAY_ITEM(array,i)     ((void *)((i) >= ((array)-> item_num) ? NULL : ((char *)((array)->p_data) + ((array)->item_size) * (i))))

/* ��������ԵĿ��ǣ����ﲢû��Ԥ������������ʹָ���Զ���ת�� */
struct k_array{
    void *p_data;                         /* Я�������� */
    int pos;                              /* ��ʹ�õ���� index ֵ + 1 */
    int item_size;                        /* ��������Ԫ����ռ�ֽ��� */
    int item_num;                         /* ����Ԫ�ؿ��ø��� */    
};

/* �����ʼ�� */
int k_array_malloc(struct k_array **array, int item_size, int item_num); 
/* �����ͷ� */
int k_array_free(struct k_array **array);
/* �õ�����Ԫ��ָ��, ���ڰ�ȫ����, �㲻�ܵõ���δ�����ڴ��ָ�� */
void *k_array_get(struct k_array *array, int index);
/* ����Ԫ�ظ�ֵ */
int k_array_set(struct k_array *array, int index, void *data);
/* ��չ����, ��Ҫ�ĵ�ʱ����չ��������Ӧ index �Ĵ�С */
int k_array_expand(struct k_array *array, int index);

/******************************************************/ 

/* �����ʼ�� */
int k_array_malloc(struct k_array **array, int item_size, int item_num)
{
    struct k_array *a;
    
    if(array == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> :array point to NULL !", __FUNCTION__);
        return -1;
    }
    *array = (struct k_array *)malloc(sizeof(struct k_array));
    a = *array;
    a -> p_data = (void *)malloc(item_size * item_num);
    a -> pos = 0;
    a -> item_size = item_size;
    a -> item_num = item_num;
    
    return 0;
}

/* �����ͷ� */
int k_array_free(struct k_array **array)
{
    if(array != NULL && *array != NULL){
        free((*array) -> p_data);
        free(*array);
        *array = NULL;
    }else{
        my_error(fp_error, MODE_ERROR, "<%s> : try to free NULL !", __FUNCTION__);
        return -1;
    }

    return 0;
}

/* �õ�����Ԫ��ָ��, ���ڰ�ȫ����, �㲻�ܵõ���δ�����ڴ��ָ�� */
void *k_array_get(struct k_array *array, int index)
{
    if(index >= array -> pos){
        my_error(fp_error, MODE_ERROR, "<%s> : index out of range !", __FUNCTION__);
        return NULL;
    }
    return (void *)((char *)array -> p_data + index * array -> item_size);
}

/* ����Ԫ�ظ�ֵ */
int k_array_set(struct k_array *array, int index, void *data)
{
    void *p;
    
    if(index >= array -> item_num)
        k_array_expand(array, index);
    p = k_ARRAY_ITEM(array, index);
    if(p == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : get NULL pointer !", __FUNCTION__);
        return -1;
    }
    memcpy(p, data, array -> item_size);
    
    array -> pos = ( index >= array->pos ? index + 1 : array->pos );
    
    return 0;
}

/* ��չ����, ��Ҫ�ĵ�ʱ����չ��������Ӧ index �Ĵ�С */
int k_array_expand(struct k_array *array, int index)
{
    long long i, item_num;
    void *p;
    
    if(array == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> :array point to NULL !", __FUNCTION__);
        return -1;
    }
    
    if(index < array -> item_num){
        return 0;
    }
    
    i = index, item_num = array -> item_num;
    while(i >= item_num){
        item_num *= 2;
    }
    
    p = (void *)malloc(array -> item_size * item_num);
    if(p == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> :malloc memory failed !", __FUNCTION__);
        return -1;
    }
    memcpy(p, array -> p_data, array -> item_num * array -> item_size);
    free(array -> p_data);
    array -> p_data = p;
    array -> item_num = item_num;
    
    return 0;
}

#endif

