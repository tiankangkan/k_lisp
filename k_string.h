/**
@File      : k_tdw_file.h
@Author    : kangtian
@Date      : 15-09-27
@Describle : �ַ��������� 
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

/* �� string ���ַ��� sep ��Ϊ�ָ����ֿ�, ��������� result<char *>, cut ������������Ȥ��ͷβȥ��, cut ��Ϊ '\0' ����ʹ�����
 * do_combine_sep ����ָʾ�Ƿ� ���ڵķָ���ϲ� */
int k_string_split_malloc(char *str, char *sep, char cut, int do_combine_sep, struct k_array *result);
/* ���������� k_string_split_malloc(), ��ͬ�����䷵�طָ��Ӵ�����ʼ������(����ҿ�)�� index ֵ, start:2n end:2n+1 */
int k_string_split_get_index_malloc(char *str, char *sep, int do_combine_sep, struct k_array *result);
/* �� string ͷβ���ַ� cut �޳�, ����Ḳ��ԭ�ַ���, cut_mode ȡ�Ժ궨�� K_STRING_CUT_** */
int k_string_cut(char *str, char cut, int cut_mode);
/* �� string ͷβ���ַ����� func() ���ַ��޳�, ����Ḳ��ԭ�ַ���, cut_mode ȡ�Ժ궨�� K_STRING_CUT_** */
int k_string_cut_pattern(char *str, int cut_mode, int (*func)(int c));
/* �� string һ�Ա��(token_left and token_right)��Χ�������ַ��е� �ַ� src �滻Ϊ dest(������ű���), 
 * �� only_once ֵΪ iTRUE ʱ, �ǺŽ��ᱻ�滻һ�� */
int k_string_token_replace(char *str, char t_left, char t_right, char c_src, char c_dest, int only_once);
/* �� string �е��ַ� src �滻Ϊ dest */
int k_string_char_replace(char *str, char src, char dest);
/* �� string �еĿհ׷�(isspace() is TRUE)�滻Ϊ dest */
int k_string_replace_space_to_char(char *str, char dest);
/* ���ļ���ȡһ��, �ļ�ĩβ���� NULL, ��ȥ�� '\r', '\n' */
char *k_string_fgetline(FILE *fp, char *buf, int buf_size);

/*******************************************************/


/* �� string ���ַ��� sep ��Ϊ�ָ����ֿ�, ��������� result<char *>, cut ������������Ȥ��ͷβȥ��, cut ��Ϊ '\0' ����ʹ�����
 * do_combine_sep ����ָʾ�Ƿ� ���ڵķָ���ϲ� */
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
    
    result -> pos = 0;    /* ָ���ʼ�� */
    if(strlen(str) == 0){
        return 0;
    }
    
    k_array_malloc(&mark, sizeof(int), 128);        /* ��ʼ��Ϊ 128 ��Ԫ�� */
    
    sep_count = 0;
    length = strlen(str);
    len_sep = strlen(sep);
    /* ��ȥ len_sep ����˼��, λ��ĩβ�ķָ����ᱻ���� */
    for(i = 0; i < length; ){
        s = str + i;
        if(strncmp(s, sep, len_sep) == 0){
            j = 1;
            while(strncmp(s + j * len_sep, sep, len_sep) == 0 && do_combine_sep){
                j ++;    /* �ϲ��ظ��� sep Ϊ 1 �� */
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
    
    if(end == length){    /* �ų����һ���� sep ����� */
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

    
            /* ������ cut = '\0' ���� cut ���� */
            if(cut != '\0'){
                k_string_cut(buf, cut, K_STRING_CUT_ALL);
            }
            
            /* ���ŵ� sep ��õ� ���ַ��� */
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

/* ���������� k_string_split_malloc(), result<int>, ��ͬ�����䷵�طָ��Ӵ�����ʼ������(����ҿ�)�� index ֵ, start:2n end:2n+1 */
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
    
    result -> pos = 0;    /* ָ���ʼ�� */
    if(strlen(str) == 0){
        return 0;
    }

    k_array_malloc(&mark, sizeof(int), 128);        /* ��ʼ��Ϊ 128 ��Ԫ�� */
    r = result;
    
    sep_count = 0;
    length = strlen(str);
    len_sep = strlen(sep);
    /* ��ȥ len_sep ����˼��, λ��ĩβ�ķָ����ᱻ���� */
    for(i = 0; i < length - len_sep; ){
        s = str + i;
        if(strncmp(s, sep, len_sep) == 0){
            j = 1;
            while(strncmp(s + j * len_sep, sep, len_sep) == 0 && do_combine_sep){
                j ++;    /* �ϲ��ظ��� sep Ϊ 1 �� */
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
    
    if(end == length){    /* �ų����һ���� sep ����� */
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

/* �� string ͷβ���ַ����� func() ���ַ��޳�, ����Ḳ��ԭ�ַ���, cut_mode ȡ�Ժ궨�� K_STRING_CUT_** */
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

/* �� string һ�Ա��(token_left and token_right)��Χ�������ַ��е� �ַ� src �滻Ϊ dest(������ű���), 
 * �� only_once ֵΪ iTRUE ʱ, �ǺŽ��ᱻ�滻һ��
 * ����ֵΪ���һ��ƥ��Ǻ�ʱ token_right ��Ӧ�� str index */
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

/* �� string �е��ַ� src �滻Ϊ dest */
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

/* �� string �еĿհ׷�(isspace() is TRUE)�滻Ϊ dest */
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

/* ���ļ���ȡһ��, ��ȥ�� '\r', '\n' */
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
        return NULL;    /* �ļ�ĩβ���߶�ȡ����... */
    }
    
    k_string_cut(buf, '\r', K_STRING_CUT_TAIL);
    k_string_cut(buf, '\n', K_STRING_CUT_TAIL);
    
    return buf;
}

#endif

