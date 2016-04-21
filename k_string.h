/**
@File      : k_tdw_file.h
@Author    : kangtian
@Date      : 15-09-27
@Describle : 字符串处理函数 
*/

#ifndef __K_STRING_H__
    #define __K_STRING_H__

#include <string.h>
#include <ctype.h>
#include "k_base.h"
#include "k_array.h"

#define K_STRING_MAX_BYTES   (1024*32)

#define K_STRING_CUT_HEAD    1
#define K_STRING_CUT_TAIL    2
#define K_STRING_CUT_ALL     (K_STRING_CUT_HEAD | K_STRING_CUT_TAIL)

/* 将 string 以字符串 sep 作为分隔符分开, 结果保存在 result<char *>, cut 用来将不感兴趣的头尾去除, cut 设为 '\0' 即可使其忽略
 * do_combine_sep 用来指示是否将 相邻的分割符合并 */
int k_string_split_malloc(char *str, char *sep, char cut, int do_combine_sep, struct k_array *result);
/* 功能类似于 k_string_split_malloc(), 不同的是其返回分割子串的起始及结束(左闭右开)的 index 值, start:2n end:2n+1 */
int k_string_split_get_index_malloc(char *str, char *sep, int do_combine_sep, struct k_array *result);
/* 将 string 头尾的字符 cut 剔除, 结果会覆盖原字符串, cut_mode 取自宏定义 K_STRING_CUT_** */
int k_string_cut(char *str, char cut, int cut_mode);
/* 将 string 头尾的字符满足 func() 的字符剔除, 结果会覆盖原字符串, cut_mode 取自宏定义 K_STRING_CUT_** */
int k_string_cut_pattern(char *str, int cut_mode, int (*func)(int c));
/* 将 string 一对标号(token_left and token_right)包围起来的字符中的 字符 src 替换为 dest(包括标号本身), 
 * 当 only_once 值为 iTRUE 时, 记号仅会被替换一次 */
int k_string_token_replace(char *str, char t_left, char t_right, char c_src, char c_dest, int only_once);
/* 将 string 中的字符 src 替换为 dest */
int k_string_char_replace(char *str, char src, char dest);
/* 将 string 中的空白符(isspace() is TRUE)替换为 dest */
int k_string_replace_space_to_char(char *str, char dest);
/* 从文件读取一行, 文件末尾返回 NULL, 已去除 '\r', '\n' */
char *k_string_fgetline(FILE *fp, char *buf, int buf_size);

/*******************************************************/


/* 将 string 以字符串 sep 作为分隔符分开, 结果保存在 result<char *>, cut 用来将不感兴趣的头尾去除, cut 设为 '\0' 即可使其忽略
 * do_combine_sep 用来指示是否将 相邻的分割符合并 */
int k_string_split_malloc(char *str, char *sep, char cut, int do_combine_sep, struct k_array *result)
{
    int i, j;
    int length, len_sep, len_buf, fact_end;
    int start, end, sep_count;
    struct k_array *mark;
    char buf[K_STRING_MAX_BYTES];
    char *s;
    
    if(str == NULL || result == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : some pointer is NULL!", __FUNCTION__);
        return -1;
    }
    
    result -> pos = 0;    /* 指针初始化 */
    if(strlen(str) == 0){
        return 0;
    }
    
    k_array_malloc(&mark, sizeof(int), 128);        /* 初始化为 128 个元素 */
    
    sep_count = 0;
    length = strlen(str);
    len_sep = strlen(sep);
    /* 减去 len_sep 的意思是, 位于末尾的分隔符会被忽略 */
    for(i = 0; i < length; ){
        s = str + i;
        if(strncmp(s, sep, len_sep) == 0){
            j = 1;
            while(strncmp(s + j * len_sep, sep, len_sep) == 0 && do_combine_sep){
                j ++;    /* 合并重复的 sep 为 1 个 */
            }
            start = i, end = i + j * len_sep;
            k_array_set(mark, mark -> pos, &start);    /* index = sep_count * 2 */
            k_array_set(mark, mark -> pos, &end);      /* index = sep_count * 2 + 1 */
            sep_count ++;
            i += j * len_sep;
            //printf("find [%d, %d]\n", start, end);
        }else{
            i ++;
        }
    }
    
    if(end == length){    /* 排除最后一个是 sep 的情况 */
        sep_count --;
    }
    
    j = 1;
    fact_end = length;
    while(strncmp(str + length - j*len_sep, sep, len_sep) == 0){
        fact_end = length - j*len_sep;
        j ++;
    }
    
    if(sep_count > 0){
        for(i = 0; i < sep_count + 1; i ++){
            start = ( i == 0 ? 0 : *(int *)k_array_get(mark, (i-1) * 2 + 1) );
            end = ( i == sep_count ? fact_end: (*(int *)k_array_get(mark, i * 2)));
            buf[0] = '\0';
            if((start < length && start >= 0) && (end <= length && end >= 0)){
                memcpy(buf, str + start, end - start);
                buf[end - start] = '\0';
            }
            //printf("get [%d, %d]\n", start, end);

    
            /* 可设置 cut = '\0' 避免 cut 操作 */
            if(cut != '\0'){
                k_string_cut(buf, cut, K_STRING_CUT_ALL);
            }
            
            /* 连着的 sep 会得到 空字符串 */
            len_buf = strlen(buf);
            if(len_buf >= 0){
                s = (char *)malloc(len_buf + 1);
                memcpy(s, buf, len_buf);
                s[len_buf] = '\0';
                k_array_set(result, result -> pos, &s);
                //printf("|%s|\n", s);
            }
        }
    }
    
    k_array_free(&mark);
    
    return 0;
}

/* 功能类似于 k_string_split_malloc(), result<int>, 不同的是其返回分割子串的起始及结束(左闭右开)的 index 值, start:2n end:2n+1 */
int k_string_split_get_index_malloc(char *str, char *sep, int do_combine_sep, struct k_array *result)
{
    int i, j;
    int length, len_sep, fact_end;
    int start, end, sep_count;
    struct k_array *r, *mark;
    char *s;
    
    if(str == NULL || result == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : some pointer is NULL!", __FUNCTION__);
        return -1;
    }
    
    result -> pos = 0;    /* 指针初始化 */
    if(strlen(str) == 0){
        return 0;
    }

    k_array_malloc(&mark, sizeof(int), 128);        /* 初始化为 128 个元素 */
    r = result;
    
    sep_count = 0;
    length = strlen(str);
    len_sep = strlen(sep);
    /* 减去 len_sep 的意思是, 位于末尾的分隔符会被忽略 */
    for(i = 0; i < length - len_sep; ){
        s = str + i;
        if(strncmp(s, sep, len_sep) == 0){
            j = 1;
            while(strncmp(s + j * len_sep, sep, len_sep) == 0 && do_combine_sep){
                j ++;    /* 合并重复的 sep 为 1 个 */
            }
            start = i, end = i + j * len_sep;
            k_array_set(mark, mark -> pos, &start);    /* index = sep_count * 2 */
            k_array_set(mark, mark -> pos, &end);      /* index = sep_count * 2 + 1 */
            sep_count ++;
            i += j * len_sep;
            //printf("find [%d, %d]\n", start, end);
        }else{
            i ++;
        }
    }
    
    if(end == length){    /* 排除最后一个是 sep 的情况 */
        sep_count --;
    }
    
    j = 1;
    fact_end = length;
    while(strncmp(str + length - j*len_sep, sep, len_sep) == 0){
        fact_end = length - j*len_sep;
        j ++;
    }

    if(sep_count >= 0){
        for(i = 0; i < sep_count + 1; i ++){
            start = ( i == 0 ? 0 : *(int *)k_array_get(mark, (i-1) * 2 + 1) );
            end = ( i == sep_count ? fact_end: (*(int *)k_array_get(mark, i * 2)));
            k_array_set(r, r -> pos, &start);
            k_array_set(r, r -> pos, &end);
        }
    }
    
    k_array_free(&mark);
    
    return 0;
}

int k_string_cut(char *str, char cut, int cut_mode)
{
    int i, len, start, end;
    char *copy;
    
    if(str == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : some pointer is NULL!", __FUNCTION__);
        return -1;
    }
    
    len = strlen(str);
    copy = (char *)malloc(len + 1);
    memcpy(copy, str, len + 1);
    
    start = 0, end = len;
    if(cut_mode & K_STRING_CUT_HEAD){
        for(i = 0; str[i] != '\0'; i ++){
            if(str[i] != cut)
                break;
        }
        start = i;
    }
    if(cut_mode & K_STRING_CUT_TAIL){
        for(i = end - 1; i >= 0; i --){
            if(str[i] != cut)
                break;
        }
        end = i + 1;
    }
    
    if(end - start > 0){
        memcpy(str, copy + start, end - start);
        str[end - start] = '\0';
    }else{
        str[0] = '\0';
    }
    
    free(copy);
    
    return 0;
}

/* 将 string 头尾的字符满足 func() 的字符剔除, 结果会覆盖原字符串, cut_mode 取自宏定义 K_STRING_CUT_** */
int k_string_cut_pattern(char *str, int cut_mode, int (*func)(int c))
{
    int i, len, start, end;
    char *copy;
    
    if(str == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : some pointer is NULL!", __FUNCTION__);
        return -1;
    }
    
    len = strlen(str);
    copy = (char *)malloc(len + 1);
    memcpy(copy, str, len + 1);
    
    start = 0, end = len;
    if(cut_mode & K_STRING_CUT_HEAD){
        for(i = 0; str[i] != '\0'; i ++){
            if(!func(str[i]))
                break;
        }
        start = i;
    }
    if(cut_mode & K_STRING_CUT_TAIL){
        for(i = end - 1; i >= 0; i --){
            if(!func(str[i]))
                break;
        }
        end = i + 1;
    }
    
    if(end - start > 0){
        memcpy(str, copy + start, end - start);
        str[end - start] = '\0';
    }else{
        str[0] = '\0';
    }
    
    free(copy);
    
    return 0;
}

/* 将 string 一对标号(token_left and token_right)包围起来的字符中的 字符 src 替换为 dest(包括标号本身), 
 * 当 only_once 值为 iTRUE 时, 记号仅会被替换一次
 * 返回值为最后一次匹配记号时 token_right 对应的 str index */
int k_string_token_replace(char *str, char t_left, char t_right, char c_src, char c_dest, int only_once)
{
    int i, j, c, pos_left, pos_right;
    
    if(str == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : some pointer is NULL!", __FUNCTION__);
        return -1;
    }
    
    pos_left = -1;
    pos_right = -1;
    for(i = 0; (c = str[i]) != '\0'; i ++){
        if(c == t_left && pos_left < 0){
            pos_left = i;
        }
        if(c == t_right && pos_left >= 0 && pos_left != i){
            for(j = pos_left; j <= i; j ++){
                if(str[j] == c_src)
                    str[j] = c_dest;
            }
            pos_left = -1;
            pos_right = i;
            if(only_once == iTRUE){
                return pos_right;
            }
        }
    }
    
    return pos_right;
}

/* 将 string 中的字符 src 替换为 dest */
int k_string_char_replace(char *str, char src, char dest)
{
    int i;
    
    if(str == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : some pointer is NULL!", __FUNCTION__);
        return -1;
    }
    
    for(i = 0; str[i] != '\0'; i ++){
        if(str[i] == src)
            str[i] = dest;
    }
    
    return 0; 
}

/* 将 string 中的空白符(isspace() is TRUE)替换为 dest */
int k_string_replace_space_to_char(char *str, char dest)
{
    int i;
    
    if(str == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : some pointer is NULL!", __FUNCTION__);
        return -1;
    }
    
    for(i = 0; str[i] != '\0'; i ++){
        if(isspace(str[i]))
            str[i] = dest;
    }
    
    return 0; 
} 

/* 从文件读取一行, 已去除 '\r', '\n' */
char *k_string_fgetline(FILE *fp, char *buf, int buf_size)
{
    if(fp == NULL || buf == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : some pointer is NULL!", __FUNCTION__);
        return NULL;
    }
    
    if(fgets(buf, buf_size, fp) == NULL){
        if(!feof(fp)){
            my_error(fp_error, MODE_ERROR, "<%s> : fgets() failed!", __FUNCTION__);
        }
        return NULL;    /* 文件末尾或者读取出错... */
    }
    
    k_string_cut(buf, '\r', K_STRING_CUT_TAIL);
    k_string_cut(buf, '\n', K_STRING_CUT_TAIL);
    
    return buf;
}

#endif

