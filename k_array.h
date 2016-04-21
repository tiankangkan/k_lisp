/**
@File      : k_array.h
@Author    : kangtian
@Date      : 15-09-25
@Describle : 可变数组
@Note      : 语句 k_array_set(array, array -> pos, data) 可实现将 data 放到下一个位置. 
*/
#ifndef __k_ARRAY_H__
    #define __k_ARRAY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "k_error.h"


/* 此宏不能取到当前没有分配位置的指针 */
#define k_ARRAY_ITEM(array,i)     ((void *)((i) >= ((array)-> item_num) ? NULL : ((char *)((array)->p_data) + ((array)->item_size) * (i))))

/* 出于灵活性的考虑，这里并没有预置数据类型以使指针自动被转换 */
struct k_array{
    void *p_data;                         /* 携带的数据 */
    int pos;                              /* 已使用的最大 index 值 + 1 */
    int item_size;                        /* 单个数组元素所占字节数 */
    int item_num;                         /* 数组元素可用个数 */    
};

/* 数组初始化 */
int k_array_malloc(struct k_array **array, int item_size, int item_num); 
/* 数组释放 */
int k_array_free(struct k_array **array);
/* 得到数组元素指针, 出于安全考虑, 你不能得到尚未分配内存的指针 */
void *k_array_get(struct k_array *array, int index);
/* 数组元素赋值 */
int k_array_set(struct k_array *array, int index, void *data);
/* 扩展数组, 必要的的时候扩展数组以适应 index 的大小 */
int k_array_expand(struct k_array *array, int index);

/******************************************************/ 

/* 数组初始化 */
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

/* 数组释放 */
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

/* 得到数组元素指针, 出于安全考虑, 你不能得到尚未分配内存的指针 */
void *k_array_get(struct k_array *array, int index)
{
    if(index >= array -> pos){
        my_error(fp_error, MODE_ERROR, "<%s> : index out of range !", __FUNCTION__);
        return NULL;
    }
    return (void *)((char *)array -> p_data + index * array -> item_size);
}

/* 数组元素赋值 */
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

/* 扩展数组, 必要的的时候扩展数组以适应 index 的大小 */
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

