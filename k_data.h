/**
    name      : k_data.h
    date      ：2015.09.24
    describe  : 自省数据表达
    copyright : 你可以自由使用它, 但是不能在不变更版本号的情况下对其作出修改.
    note      : 其数据类型有些定义来自于 k_base.h
    updata    :
        1) 
*/

#ifndef __K_DATA_H__
    #define  __K_DATA_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "k_error.h"
#include "k_base.h"

#define KDATA_MAGIC                0x940425

/* 赋值必须是连续的, 并且 KDATA_TYPE_KDATA 位于最后一个, 增加时更新函数列表 !! */
#define KDATA_TYPE_NONE            0
#define KDATA_TYPE_STRING          1
#define KDATA_TYPE_CHAR            2
#define KDATA_TYPE_UCHAR           3
#define KDATA_TYPE_INT32           4
#define KDATA_TYPE_UINT32          5
#define KDATA_TYPE_INT64           6
#define KDATA_TYPE_UINT64          7
#define KDATA_TYPE_FLOAT32         8
#define KDATA_TYPE_FLOAT64         9
#define KDATA_TYPE_KDATA           10
/* 128 ~ 256 为预留标识号 */

/* 取得数据类型对应的指针 */ 
#define KDATA_P_STRING(x)          ( (iINT8 **)(x) )
#define KDATA_P_CHAR(x)            ( (iINT8 *)(x) )
#define KDATA_P_INT32(x)           ( (iINT32 *)(x) )
#define KDATA_P_INT64(x)           ( (iINT64 *)(x) ) 
#define KDATA_P_FLOAT32(x)         ( (iFLOAT32 *)(x) )
#define KDATA_P_FLOAT64(x)         ( (iFLOAT64 *)(x) )
#define KDATA_P_KDATA(x)           ( (struct k_data *)(x) )

struct k_data{
    int   magic;    /* magic 标识值 : 0x940425 */
	void *p_data;
	int   type;
};

/* k_data 初始化 */
int k_data_malloc(struct k_data **d, void *p_data, int type);
/* k_data 初始化并返回指针 */
struct k_data *k_data_just_malloc(void *p_data, int type);
/* k_data 释放*/
int k_data_free(struct k_data *d); 
/* k_data 拷贝*/
int k_data_copy(struct k_data **dest, struct k_data *src);
/* 值比较函数 */
int k_data_comp(struct k_data *data_a, struct k_data *data_b);
/* 值比较函数(string) */
int k_data_comp_string(struct k_data *data_a, struct k_data *data_b);
/* 值比较函数(int32) */
int k_data_comp_int32(struct k_data *data_a, struct k_data *data_b);
/* 值比较函数(int64) */
int k_data_comp_int64(struct k_data *data_a, struct k_data *data_b);
/* 值比较函数(float64) */
int k_data_comp_float64(struct k_data *data_a, struct k_data *data_b);
/* 注意释放 返回指针, 注意释放 返回指针, 注意释放 返回指针 */
char *k_data_convert_to_string_malloc(struct k_data *data);

/* 比较函数映射, 注意保持 KDATA_TYPE_KDATA 为合适值 */
struct {
    int type;
    int (*k_data_comp)(struct k_data *a, struct k_data *b);
} k_data_cmp_func[KDATA_TYPE_KDATA + 1] = {
    /* 0 ~ 5 */
    KDATA_TYPE_NONE     , NULL,
    KDATA_TYPE_STRING   , k_data_comp_string,
    KDATA_TYPE_CHAR     , NULL,
    KDATA_TYPE_UCHAR    , NULL,
    KDATA_TYPE_INT32    , k_data_comp_int32,
    
    /* 6 ~ 11 */
    KDATA_TYPE_UINT32   , NULL,
    KDATA_TYPE_INT64    , k_data_comp_int64,
    KDATA_TYPE_UINT64   , NULL,
    KDATA_TYPE_FLOAT32  , NULL,
    KDATA_TYPE_FLOAT64  , k_data_comp_float64,
    KDATA_TYPE_KDATA    , k_data_comp,
};


/************************************************************/ 

/* k_data 初始化*/
int k_data_malloc(struct k_data **d, void *p_data, int type)
{
    struct k_data *data;
    int length;
    
    if(d == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : some pointer is NULL!", __FUNCTION__);
        return -1;
    }
    
    *d = (struct k_data *)malloc(sizeof(struct k_data));
    data = *d;
    
    data -> type = type;
    data -> magic = KDATA_MAGIC;
    
    length = k_data_sizeof_data(type, p_data);
    data -> p_data = (void *)malloc(length);
    memcpy(data -> p_data, p_data, length);

    return 0;
}

/* k_data 初始化并返回指针 */
struct k_data *k_data_just_malloc(void *p_data, int type)
{
    struct k_data *data;
    
    k_data_malloc(&data, p_data, type);
    
    return data;
} 

/* k_data 释放*/
int k_data_free(struct k_data *data)
{
    if(data == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : data point to NULL!", __FUNCTION__);
        return -1;
    }    
    if(data -> magic != KDATA_MAGIC){
        my_error(fp_error, MODE_ERROR, "<%s> : magic number can't suit !", __FUNCTION__);
        return -1;
    }
    
    free(data -> p_data);
    free(data);
    
    return 0;
} 


/* 数据类型大小 */
int k_data_sizeof_data(int type, void *data)
{
    switch(type){
        case KDATA_TYPE_STRING:
            return strlen((char *)data) + 1;
        case KDATA_TYPE_CHAR:     case KDATA_TYPE_UCHAR:
            return sizeof(char);
        case KDATA_TYPE_INT32:    case KDATA_TYPE_UINT32:
            return sizeof(iINT32);
        case KDATA_TYPE_INT64:    case KDATA_TYPE_UINT64:
            return sizeof(iINT64);
        case KDATA_TYPE_FLOAT64:
            return sizeof(iFLOAT64);
    }
    
    return -1;
}

/* k_data 拷贝*/
int k_data_copy(struct k_data **dest, struct k_data *src)
{
    if(src == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : some pointer is NULL!", __FUNCTION__);
        return -1;
    }
    if(src -> magic != KDATA_MAGIC){
        my_error(fp_error, MODE_ERROR, "<%s> : magic number can't suit !", __FUNCTION__);
        return -1;
    }
    k_data_malloc(dest, src -> p_data, src -> type);
    
    return 0;
}

/* 值比较函数 */
int k_data_comp(struct k_data *a, struct k_data *b)
{
    int type;
    int (*compare_func)(struct k_data *a, struct k_data *b);
    
	if(a == NULL || b == NULL){
	    my_error(fp_error, MODE_ERROR, "<%s> : a or b point to NULL!", __FUNCTION__);
	    return K_CMP_UNKNOWN;
	}
    if(b -> type != a -> type){
	    my_error(fp_error, MODE_ERROR, "<%s> : try to compare diff type!", __FUNCTION__);
        return K_CMP_UNKNOWN;
    }
    
    type = a -> type;
    if(k_data_cmp_func[type].type != type){
        my_error(fp_error, MODE_ERROR, "<%s> : please check the array: k_data_cmp_func[] !", __FUNCTION__);
        return K_CMP_UNKNOWN;
    }
    compare_func = k_data_cmp_func[type].k_data_comp;
    if(compare_func == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : unsupport compare data type!", __FUNCTION__);
        return K_CMP_UNKNOWN;
    }else{
        return compare_func(a, b);
    }
    
    return K_CMP_UNKNOWN;
}

/* 值比较函数(string) */
int k_data_comp_string(struct k_data *data_a, struct k_data *data_b)
{
    int i;
    unsigned char *string_a, *string_b;

    if(data_a -> magic != KDATA_MAGIC || data_b -> magic != KDATA_MAGIC){
        my_error(fp_error, MODE_ERROR, "<%s> : magic number can't suit !", __FUNCTION__);
        return -1;
    }

    string_a = (unsigned char *)data_a -> p_data;
    string_b = (unsigned char *)data_b -> p_data;

   	if(string_a == NULL || string_a == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : a or b point to NULL!", __FUNCTION__);
        return K_CMP_UNKNOWN;
	}

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

/* 值比较函数(int32) */
int k_data_comp_int32(struct k_data *data_a, struct k_data *data_b)
{
    iINT32 a, b;
    
    if(data_a -> magic != KDATA_MAGIC || data_b -> magic != KDATA_MAGIC){
        my_error(fp_error, MODE_ERROR, "<%s> : magic number can't suit !", __FUNCTION__);
        return -1;
    }
    
    a = *(iINT32 *)data_a -> p_data;
    b = *(iINT32 *)data_b -> p_data;
    
    if(a == b){
		return K_CMP_EQUAL;
	}else if(a > b){
		return K_CMP_BIGGER;
    }else{
		return K_CMP_SMALLER;
    }
}

/* 值比较函数(int64) */
int k_data_comp_int64(struct k_data *data_a, struct k_data *data_b)
{
    iINT64 a, b;
    
    if(data_a -> magic != KDATA_MAGIC || data_b -> magic != KDATA_MAGIC){
        my_error(fp_error, MODE_ERROR, "<%s> : magic number can't suit !", __FUNCTION__);
        return -1;
    }
    
    a = *(iINT64 *)data_a -> p_data;
    b = *(iINT64 *)data_b -> p_data;
    
    if(a == b){
		return K_CMP_EQUAL;
	}else if(a > b){
		return K_CMP_BIGGER;
    }else{
		return K_CMP_SMALLER;
    }
}

/* 值比较函数(float64) */
int k_data_comp_float64(struct k_data *data_a, struct k_data *data_b)
{
    iFLOAT64 a, b;
    
    if(data_a -> magic != KDATA_MAGIC || data_b -> magic != KDATA_MAGIC){
        my_error(fp_error, MODE_ERROR, "<%s> : magic number can't suit !", __FUNCTION__);
        return -1;
    }
    
    a = *(iFLOAT64 *)data_a -> p_data;
    b = *(iFLOAT64 *)data_b -> p_data;
    
    if(fabs(a - b) < DBL_MIN * 10){    /* 差距在 double 最小值 10 倍之内, 认为其相等 */
		return K_CMP_EQUAL;
	}else if(a > b){
		return K_CMP_BIGGER;
    }else{
		return K_CMP_SMALLER;
    }
}

/* 注意释放 返回指针, 注意释放 返回指针, 注意释放 返回指针 */
char *k_data_convert_to_string_malloc(struct k_data *data)
{
    char *str;
    int length;
    
    if(data == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : data point to NULL!", __FUNCTION__);
        return NULL;
    }    
    if(data -> magic != KDATA_MAGIC){
        my_error(fp_error, MODE_ERROR, "<%s> : magic number can't suit !", __FUNCTION__);
        return NULL;
    }

    if(data -> type == KDATA_TYPE_STRING){
        if(data -> p_data == NULL){
            my_error(fp_error, MODE_ERROR, "<%s> : string point to NULL!", __FUNCTION__);
            return NULL;
        }else{
            length = strlen((char *)data -> p_data);
            str = (char *)malloc(length + 1);
            memcpy(str, data -> p_data, length);
            str[length] = '\0';
        }
    }else if(data -> type == KDATA_TYPE_INT32){
        str = (char *)malloc(128);
        sprintf(str, "%d", *(iINT32 *)data -> p_data);
        return str;
    }else if(data -> type == KDATA_TYPE_INT64){
        str = (char *)malloc(128);
        /* 注意不同平台的语句此处不一样
         * sprintf(str, "%i64d", data -> d.data); */
        sprintf(str, "%lld", *(iINT64 *)data -> p_data);
        return str;
    }else if(data -> type == KDATA_TYPE_FLOAT32){
        str = (char *)malloc(128);
        sprintf(str, "%g", *(iFLOAT32 *)data -> p_data);
        return str;
    }else if(data -> type == KDATA_TYPE_FLOAT64){
        str = (char *)malloc(128);
        sprintf(str, "%g", *(iFLOAT64 *)data -> p_data);
        return str;
    }else if(data -> type == KDATA_TYPE_KDATA){
        return k_data_convert_to_string_malloc(data -> p_data);
    }else{
        my_error(fp_error, MODE_ERROR, "<%s> : unknown data_type: %d!", __FUNCTION__, data -> type);
        return NULL;
    }
    
    return str;
}

#endif


