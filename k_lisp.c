/**
@File      : k_lisp.h
@Author    : kangtian
@Date      : 15-10-01
@Describle : 这是一份 Lisp方言 的 C 实现 
@Note      : 
*/

#ifndef __K_LISP_H__
    #define __K_LISP_H__

#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include "k_link.h"
#include "k_string.h"
#include "k_tree.h"
#include "k_timer.h"

#define K_LISP_MAX_SYMBOL_LEN       4096   /* symbol 最大长度 */
#define K_LISP_STRING_MARK          '\''   /* 字符串边界标识 */
#define K_LISP_STRING_MARK_RESERVE  '^'    /* 备用的字符串边界标识 */
#define K_LISP_LIST_LEFT_CHAR       '('    /* list 左边边界字符 */
#define K_LISP_LIST_RIGHT_CHAR      ')'    /* list 右边边界字符 */

#define K_LISP_DEFAULT_LIST         "_list_"
#define K_LISP_TRUE                 "T"
#define K_LISP_FALSE                "NIL"
#define K_LISP_QUOTE                "_quote"
#define K_LISP_LAMBDA               "_lambda"
#define K_LISP_DEFUN                "_defun"
#define K_LISP_IF                   "_if"
#define K_LISP_PARA_FERFIX          "$"

#define K_LISP_BUILD_IN_FUNCTIONS       k_lisp_build_in_function      /* 原生函数 */
#define K_LISP_BUILD_IN_FUNC_TREE       k_lisp_build_in_func_tree     /* 原生函数存放的树 */
#define K_LISP_COMPOSITE_FUNC_TREE      k_lisp_composite_func_tree    /* 普通函数存放的树 */
#define K_LISP_BUILD_IN_FUNCTION_NUM    4096

/* [integer], [decimal fraction], and [bool] */
#define K_LISP_INTEGER              iINT64
#define K_LISP_FLOAT                iFLOAT64

/* top type level : bit 0 ~ bit7 */
#define K_LISP_TYPE_ATOM            0x00000001    /* 原子 */
#define K_LISP_TYPE_LIST            0x00000002    /* 表 */

/* second type level : bit 8 ~ bit15 */
#define K_LISP_ATOM_SYMBOL          (0x00000100 | K_LISP_TYPE_ATOM)    /* 符号 */
#define K_LISP_ATOM_NUMBER          (0x00000200 | K_LISP_TYPE_ATOM)    /* 数值 */
#define K_LISP_ATOM_STRING          (0x00000400 | K_LISP_TYPE_ATOM)    /* 字符串 */

/* third type level : bit 16 ~ bit23 */
#define K_LISP_NUMBER_INTEGER       (0x00010000 | K_LISP_ATOM_NUMBER)    /* 整数, long */
#define K_LISP_NUMBER_FLOAT         (0x00020000 | K_LISP_ATOM_NUMBER)    /* 小数, double */
#define K_LISP_NUMBER_RATIOS        (0x00040000 | K_LISP_ATOM_NUMBER)    /* 分数(暂不支持), (long / long) */

/* third type level : bit 16 ~ bit23 */
#define K_LISP_SYMBOL_BOOL          (0x00010000 | K_LISP_ATOM_SYMBOL)    /* 布尔类型 */
/* fourth type level: bit 24 ~ bit31 */
/* None */

#define K_LISP_TYPE_NUM             8
struct {
    int type;
    char name[64];
} k_lisp_type_name[K_LISP_TYPE_NUM] = {
    K_LISP_TYPE_ATOM      , "atom", 
    K_LISP_TYPE_LIST      , "list", 
    K_LISP_ATOM_SYMBOL    , "symbol", 
    K_LISP_ATOM_NUMBER    , "number", 
    K_LISP_ATOM_STRING    , "string", 
    K_LISP_NUMBER_INTEGER , "interger", 
    K_LISP_NUMBER_FLOAT   , "float", 
    K_LISP_NUMBER_RATIOS  , "ratios",
};

struct k_tree *K_LISP_BUILD_IN_FUNC_TREE = NULL;
struct k_tree *K_LISP_COMPOSITE_FUNC_TREE = NULL;
struct k_lisp_function_map{
    char *func_name;
    void *func_address;
};

/* 将 list 解析 */
struct k_link *k_lisp_parser_list_malloc(const char *list);
/* 解析 list 中单个 word 语法单元 */
struct k_link *k_lisp_parser_word_malloc(const char *word);
/* 将 lisp 对象存储于 struct k_data */
struct k_data *k_lisp_store_in_k_data_malloc(int lisp_type, void *lisp_data);
/* 释放 atom 内存 */
int k_lisp_node_free(struct k_link *list);
/* 将 lisp list 遍历释放节点内存 */
int k_lisp_list_free_all(struct k_link *list_node);
/* 得到 list 对应的字符串, 注意可能溢出 */
char *k_lisp_list_sprint_malloc(struct k_link *list, int indent_width, int indent);
/* 得到 lisp 类型对应的字符串名称 */
char *k_lisp_get_type_name();
/* 得到 list 的函数名, 如果有的话... */
char *k_lisp_get_func_name(const struct k_link *list);
/* lisp eval: 计算函数 */
struct k_link *k_lisp_eval_function(struct k_link *list);
/* 计算由 defun(lambda) 定义的函数 */
struct k_link *k_lisp_eval_lambda_function(struct k_link *list, void *func);
/* k_lisp eval 函数, 使用内置函数求值 */
struct k_link *k_lisp_eval_build_in_function(struct k_link *func_list, void *func);
/* 将内置函数以函数名为键值 添加到树中 */
int k_lisp_load_build_in_functions_to_tree(struct k_tree **func_tree, struct k_lisp_function_map *func_map);
/* 以函数名为键值, 从树中取函数地址 */
void *k_lisp_get_function_address_from_tree(struct k_tree *func_tree, char *func_name);
/* 得到符号 NIL  */
struct k_link *k_lisp_get_false();
/* 得到符号 T  */
struct k_link *k_lisp_get_true();
/* lisp eval 函数 */
struct k_link *k_lisp_eval(struct k_link *list);
/* k_lisp build-in function: add */
struct k_link *k_lisp_arithmetic_add(struct k_link *list);
/* k_lisp build-in function: sub */
struct k_link *k_lisp_arithmetic_sub(struct k_link *list);


/* 打印树节点的函数名称 */
int k_lisp_load_function_tree_print_func_name(struct k_tree *tree); 
/*********************************************************/

/* 将 list 解析 */
struct k_link *k_lisp_parser_list_malloc(const char *str_list)
{
    int i, j, c, len, offset;
    int start, end;
    int deep, type, pos, last_pos;
    char log[K_STRING_MAX_BYTES]; 
    char *plist, *buf, *word;
    struct k_array *mark_atom, *mark_list;
    struct k_link *list, *node;
    
    if(str_list == NULL || strlen(str_list) <= 0){
        if(str_list == NULL)
            my_error(fp_error, MODE_ERROR, "<%s> : some pointer is NULL!", __FUNCTION__);
        return NULL;
    }

    log[0] = '\0';
    mark_atom = mark_list = NULL;
    list = node = NULL;
    len = strlen(str_list);
    plist = (char *)malloc(len + 1);
    buf = (char *)malloc(len + 1);
    word = (char *)malloc(len + 1);
    memcpy(plist, str_list, len + 1);

    sprintf(log + strlen(log), "---------------------------------- lisp parser log start -----\nlist: \n    %s\n", str_list);
    sprintf(log + strlen(log), "----------------\n");
    
    /* 把所有的空白符转换成空格 */
    k_string_replace_space_to_char(plist, ' ');
  
    /* 消除 lisp 字符串中的空格 */
    for(i = 0; i < len; i ++){
        c = plist[i];
        /* 标志开始位置需要有一个空格(当然第一个字符就不需要了) */
        if((c == K_LISP_STRING_MARK || c == K_LISP_STRING_MARK_RESERVE) &&
                (i == 0 ? 1 : (isspace(plist[i - 1]) || 
                plist[i - 1] == K_LISP_LIST_LEFT_CHAR || plist[i - 1] == K_LISP_LIST_RIGHT_CHAR))){
            /* 替换 字符串中的空格为 '_' 以避免 string.split 错误 */
            offset = k_string_token_replace(plist + i, c, c, ' ', '_', iTRUE);
            /* 替换 字符串中的列表左边界字符为 '_' 以避免断定括号匹配错误 */
            offset = k_string_token_replace(plist + i, c, c, K_LISP_LIST_LEFT_CHAR, '_', iTRUE);
            /* 替换 字符串中的列表右边界字符为 '_' 以避免断定括号匹配错误 */
            offset = k_string_token_replace(plist + i, c, c, K_LISP_LIST_RIGHT_CHAR, '_', iTRUE);
            if(offset > 0){
                i += offset;
            }
        }
    }
    //printf("1. plist = %s\n", plist);
    i = 0;
    while(isspace(plist[i])){    /* 跳过空白符 */
        i ++;
    }
    if(plist[i] != K_LISP_LIST_LEFT_CHAR){
        my_error(fp_error, MODE_ERROR, "<%s> : miss list left boundary character : %c !", __FUNCTION__, K_LISP_LIST_LEFT_CHAR);
        free(plist);
        free(buf);
        return NULL;
    }
    deep = 0;
    k_array_malloc(&mark_list, sizeof(int), 64);    /* 数组是动态的, 这里只是指定了初始大小 */
    for(i = i; i < len; i ++){
        c = plist[i];
        if(c == K_LISP_LIST_LEFT_CHAR){
            if(deep < 2){         /* 仅考率 一级子列表 */
                type = 0;         /* 0 代表 left */
                k_array_set(mark_list, mark_list -> pos, &type);        /* 3*n */
                k_array_set(mark_list, mark_list -> pos, &deep);        /* 3*n + 1 */
                k_array_set(mark_list, mark_list -> pos, &i);           /* 3*n + 2 */
            }
            deep ++;
        }else if(c == K_LISP_LIST_RIGHT_CHAR){
            deep --;
            if(deep < 0){
                my_error(fp_error, MODE_ERROR, "<%s> : list boundary character can't match : '%c'\n<position> : char[%d]", 
                                 __FUNCTION__, K_LISP_LIST_RIGHT_CHAR, i);
                k_array_free(&mark_list);
                free(plist);
                free(buf);
                return NULL;
            }
            if(deep < 2 && deep >= 0){    /* 仅考率 一级子列表 */
                type = 1;                 /* 1 代表 right */
                k_array_set(mark_list, mark_list -> pos, &type);        /* 3*n */
                k_array_set(mark_list, mark_list -> pos, &deep);        /* 3*n + 1 */
                k_array_set(mark_list, mark_list -> pos, &i);           /* 3*n + 2 */
            }
        }
    }
    if(deep != 0){
        my_error(fp_error, MODE_ERROR, "<%s> : list boundary character can't match : %c or %c !", 
                __FUNCTION__, K_LISP_LIST_LEFT_CHAR, K_LISP_LIST_RIGHT_CHAR);
        k_array_free(&mark_list);
        free(plist);
        free(buf);
        return NULL;
    }
    
    k_link_node_malloc(&list, k_data_just_malloc(K_LISP_DEFAULT_LIST, KDATA_TYPE_STRING), K_LISP_TYPE_LIST); /* list 分配内存 */
    k_array_malloc(&mark_atom, sizeof(int), 64);    /* 数组是动态的, 这里只是指定了初始大小 */
    last_pos = 0;
    for(j = 0; j < mark_list -> pos; j += 3){
        type = *(int *)k_array_get(mark_list, j);
        deep = *(int *)k_array_get(mark_list, j + 1);
        pos = *(int *)k_array_get(mark_list, j + 2);
        
        if((type == 0 && deep == 1) || (type == 1 && deep == 0)){
            buf[0] = '\0';
            strncpy(buf, plist + last_pos + 1, pos - last_pos - 1);
            buf[pos - last_pos - 1] = '\0';
            //printf("1. buf = %s\n", buf);
            k_string_split_get_index_malloc(buf, " ", iTRUE, mark_atom);
            for(i = 0; i < mark_atom -> pos; i += 2){
                start = *(int *)k_array_get(mark_atom, i);
                end = *(int *)k_array_get(mark_atom, i + 1);
                strncpy(word, str_list + last_pos + 1 + start, end - start);
                word[end - start] = '\0';
                //printf("word = %s\nstr_list = %s\n", word, str_list);
                if(strlen(word) > 0){
                    node = k_lisp_parser_word_malloc(word);
                    if(node != NULL){
                        sprintf(log + strlen(log), "        %16.16s | %-10.10s\n", word, k_lisp_get_type_name(node -> type));
                        //printf("&node = %d\n", node);
                        //k_link_print_node_handle(node, NULL, NULL);
                        k_link_insert_this_tail(list, node);
                        //k_link_print_node_handle(node, NULL, NULL);
                    }else{
                        sprintf(log + strlen(log), "        %16.16s | %-10.10s\n", word, "unfit ...");
                        my_error(fp_error, MODE_ERROR, "<%s> : unsuppotr atom: %s\n", __FUNCTION__, word);
                        // TODO : free list.
                        k_array_free(&mark_list);
                        k_array_free(&mark_atom);
                        free(plist);
                        free(buf);
                        free(word);
                        return NULL;
                    }
                }
                //printf("atom: %s\n", *(char **)k_array_get(mark_atom, i));
            }
            //printf("part_atom: <%s>\n", buf);
        }else if(type == 1 && deep == 1){
            buf[0] = '\0';
            strncpy(buf, str_list + last_pos, pos - last_pos + 1);
            buf[pos - last_pos + 1] = '\0';
            //printf("2. buf = %s\n", buf);
            /* 递归调用 */
            node = k_lisp_parser_list_malloc(buf);
            if(node != NULL){
                sprintf(log + strlen(log), "        %13.13s... | %-10.10s\n", buf, "sub_list");
                k_link_insert_this_tail(list, node);
                //k_link_traversal_print_list_handle(node, 4, 0, NULL, NULL);
            }else{
                sprintf(log + strlen(log), "        %13.13s... | %-10.10s\n", buf, "failed ...");
                my_error(fp_error, MODE_ERROR, "<%s> : can't accept list: %s\n", __FUNCTION__, buf);
                // TODO : free list.
                k_array_free(&mark_list);
                k_array_free(&mark_atom);
                free(plist);
                free(buf);
                free(word);
                return NULL;
            }
            //printf("part_list: <%s>\n", buf);
        }
        last_pos = pos;
    }
    
    sprintf(log + strlen(log), "---------------------------------- lisp parser log end   -----\n\n");    
    //printf("%s", log);
    k_array_free(&mark_atom);
    k_array_free(&mark_list);
    free(plist);
    free(buf);
    free(word);
    
    return list;
}

struct k_link *k_lisp_parser_word_malloc(const char *word)
{
    int is_fit, fit_num;
    char unfit[K_LISP_MAX_SYMBOL_LEN];
    char fit_symbol[K_LISP_MAX_SYMBOL_LEN];
    char fit_string[K_LISP_MAX_SYMBOL_LEN];
    struct k_link *lisp_node;
    struct k_data *lisp_node_data;
    K_LISP_INTEGER fit_int;
    K_LISP_FLOAT   fit_float;
    
    is_fit = iFALSE;
    if(word == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : some pointer is NULL !", __FUNCTION__);
        return NULL;
    }
    
    if(isalpha(word[0]) || word[0] == '_'){
        fit_symbol[0] = unfit[0] = '\0';
        sscanf(word, "%[_a-zA-Z0-9]%s", fit_symbol, unfit);
        if(strlen(unfit) == 0 && strlen(fit_symbol) != 0){
            //printf("%16s : %-8s | %s\n", word, "SYMBOL", fit_symbol);
            lisp_node_data = k_lisp_store_in_k_data_malloc(K_LISP_ATOM_SYMBOL, fit_symbol);
            if(lisp_node_data == NULL)
                return NULL;
            k_link_node_malloc(&lisp_node, lisp_node_data, K_LISP_ATOM_SYMBOL);
            is_fit = iTRUE;
        }
    }else if(word[0] == K_LISP_STRING_MARK || word[0] == K_LISP_STRING_MARK_RESERVE){
        if(word[strlen(word) - 1] == word[0]){
            strncpy(fit_string, word + 1, strlen(word) - 2);
            fit_string[strlen(word) - 2] = '\0';
            //printf("%16s : %-8s | %s\n", word, "STRING", fit_string);
            lisp_node_data = k_lisp_store_in_k_data_malloc(K_LISP_ATOM_STRING, fit_string);
            if(lisp_node_data == NULL)
                return NULL;
            k_link_node_malloc(&lisp_node, lisp_node_data, K_LISP_ATOM_STRING);
            is_fit = iTRUE;
        }
    }else{
        unfit[0] = '\0';
        /* Warning: Non portable code */
        if(sizeof(K_LISP_INTEGER) == 8){    /* there integer is 32-bits or 64-bits */
            fit_num = sscanf(word, "%lld%s", &fit_int, unfit);
        }else if(sizeof(K_LISP_INTEGER) == 4){
            fit_num = sscanf(word, "%d%s", &fit_int, unfit);
        }else{
            my_error(fp_error, MODE_ERROR, "<%s> : can't accpet sizeof(K_LISP_INTEGER)!", __FUNCTION__);
            return NULL;
        }
        if(fit_num > 0 && strlen(unfit) == 0){
            if(sizeof(K_LISP_INTEGER) == 8){
                //printf("%16s : %-8s | %lld\n", word, "INTEGER", fit_int);
            }else if(sizeof(K_LISP_INTEGER) == 4){
                //printf("%16s : %-8s | %d\n", word, "INTEGER", fit_int);
            }else{
                my_error(fp_error, MODE_ERROR, "<%s> : can't accpet sizeof(K_LISP_INTEGER)!", __FUNCTION__);
                return NULL;
            }
            lisp_node_data = k_lisp_store_in_k_data_malloc(K_LISP_NUMBER_INTEGER, &fit_int);
            if(lisp_node_data == NULL)
                return NULL;
            k_link_node_malloc(&lisp_node, lisp_node_data, K_LISP_NUMBER_INTEGER);
            is_fit = iTRUE;
        }
        
        /* 整型不匹配匹配浮点型 */
        if(is_fit == iFALSE){
            unfit[0] = '\0';
            if(sizeof(fit_float) == 4)
                fit_num = sscanf(word, "%f%s", &fit_float, unfit);
            else if(sizeof(fit_float) == 8) 
                fit_num = sscanf(word, "%lf%s", &fit_float, unfit);
            else{
                my_error(fp_error, MODE_ERROR, "<%s> : can't accpet sizeof(K_LISP_FLOAT)!", __FUNCTION__);
                return NULL;
            }
            if(fit_num > 0 && strlen(unfit) == 0){
                //printf("%16s : %-8s | %g\n", word, "FLOAT", fit_float);
                lisp_node_data = k_lisp_store_in_k_data_malloc(K_LISP_NUMBER_FLOAT, &fit_float);
                if(lisp_node_data == NULL)
                    return NULL;
                k_link_node_malloc(&lisp_node, lisp_node_data, K_LISP_NUMBER_FLOAT);
                is_fit = iTRUE;
            }
        }
    }
    
    if(is_fit != iTRUE){
        my_error(fp_error, MODE_ERROR, "<%s> : can't accpet word: %s!", __FUNCTION__, word);
        return NULL;
    }
    
    return lisp_node;
}

/* 将 lisp 对象存储于 struct k_data */
struct k_data *k_lisp_store_in_k_data_malloc(int lisp_type, void *lisp_data)
{
    struct k_data *data;
    
    switch(lisp_type){
        case K_LISP_ATOM_SYMBOL:
            k_data_malloc(&data, lisp_data, KDATA_TYPE_STRING);    break;
        case K_LISP_ATOM_STRING:
            k_data_malloc(&data, lisp_data, KDATA_TYPE_STRING);    break;
        case K_LISP_NUMBER_INTEGER:
            if(sizeof(K_LISP_INTEGER) == 8){
                k_data_malloc(&data, lisp_data, KDATA_TYPE_INT64);
            }else if(sizeof(K_LISP_INTEGER) == 4){
                k_data_malloc(&data, lisp_data, KDATA_TYPE_INT32);
            }else{
                my_error(fp_error, MODE_ERROR, "<%s> : can't accpet sizeof(K_LISP_INTEGER)!", __FUNCTION__);
                return NULL;
            }
            break;
        case K_LISP_NUMBER_FLOAT:
            if(sizeof(K_LISP_FLOAT) == 8){
                k_data_malloc(&data, lisp_data, KDATA_TYPE_FLOAT64);
            }else if(sizeof(K_LISP_FLOAT) == 4){
                k_data_malloc(&data, lisp_data, KDATA_TYPE_FLOAT32);
            }else{
                my_error(fp_error, MODE_ERROR, "<%s> : can't accpet sizeof(K_LISP_FLOAT)!", __FUNCTION__);
                return NULL;
            }
            break;
        default:
            my_error(fp_error, MODE_ERROR, "<%s> : can't accpet lisp_type: 0x%x!", __FUNCTION__, lisp_type);
            return NULL;
            break;
    }

    return data;
}

/* 暴力释放 单一节点 内存 */
int k_lisp_node_free(struct k_link *list)
{
    if(list != NULL){
        k_data_free(list -> p_data);
        free(list);
    }
    
    return 0;
}

/* 将 lisp list 遍历释放节点内存 */
int k_lisp_list_free_all(struct k_link *list_node)
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
        k_lisp_list_free_all(now -> this_node);
        k_lisp_node_free(now);
        now = temp;
    }
    
    return 0;
}

/* 释放 list 一级子节点的所有内存及 list 节点本身的内存 */
int k_lisp_list_free_level_1(struct k_link *list)
{
    struct k_link *now, *temp;
    
    now = list -> this_node;
    while(now != NULL){
        temp = now -> next_node;
        k_lisp_node_free(now);
        now = temp;
    }
    k_lisp_node_free(list);
    
    return 0;
}

/* 得到 list 对应的字符串, 注意可能溢出 */
char *k_lisp_list_sprint_malloc(struct k_link *list, int indent_width, int indent)
{
    char *buf;
    
    buf = (char *)malloc(K_STRING_MAX_BYTES);
    buf[0] = '\0';
    k_link_traversal_sprint_child_handle(list, buf, indent_width, indent, NULL, NULL);
    
    return buf;
}

/* 算术运算 ARITHMETIC start */
/* 双目运算, 形式 ARITHMETIC(a, b) */ 
#define K_LISP_ARITHMETIC_ADD           1     /* 加法 */
#define K_LISP_ARITHMETIC_SUB           2     /* 减法 */
#define K_LISP_ARITHMETIC_MUL           3     /* 乘法 */
#define K_LISP_ARITHMETIC_DIV           4     /* 除法, 对于整数来说, 为去尾数的除法 */
#define K_LISP_ARITHMETIC_POW           5     /* 指数函数, 值为 a^b */
#define K_LISP_ARITHMETIC_MOD           6     /* 取模, 对整型和小数均有效 */

/* 单目运算, 形式为 ARITHMETIC(a) */
#define K_LISP_ARITHMETIC_SQRT          7     /* 取平方根, a >= 0 */
#define K_LISP_ARITHMETIC_LOG           8     /* 以 e 为底的自然对数 */
#define K_LISP_ARITHMETIC_ABS           9     /* 绝对值 */
#define K_LISP_ARITHMETIC_SIN           10    /* 正弦值 */
#define K_LISP_ARITHMETIC_COS           11    /* 余弦值 */
#define K_LISP_ARITHMETIC_TAN           12    /* 正切值 */
#define K_LISP_ARITHMETIC_CEIL          13    /* 不小于 a 的最小整数 */
#define K_LISP_ARITHMETIC_FLOOR         14    /* 不大于 a 的最大整数 */
#define K_LISP_ARITHMETIC_ADD1          15    /* 自减 1 */
#define K_LISP_ARITHMETIC_SUB1          16    /* 自增 1 */
#define K_LISP_ARITHMETIC_INTEGER       17    /* 强转为 integer */
#define K_LISP_ARITHMETIC_FLOAT         18    /* 强转为 float */

/* 逻辑运算 LOGIC start */
#define K_LISP_LOGIC_AND                1    /* 逻辑 <与> */
#define K_LISP_LOGIC_OR                 2    /* 逻辑 <或> */
#define K_LISP_LOGIC_NOT                3    /* 逻辑 <非> */

/* 断言 */
#define K_LISP_PREDICATE_ATOMP          1    /* 断言: 为原子(atom) */ 
#define K_LISP_PREDICATE_LISTP          2    /* 断言: 为列表(list) */
#define K_LISP_PREDICATE_EQ             3    /* 断言: 等于(equal) */
#define K_LISP_PREDICATE_LT             4    /* 断言: 小于(less than) */
#define K_LISP_PREDICATE_LE             5    /* 断言: 小于(less equal) */

/* 条件分支 */ 
#define K_LISP_CONDITION_IF             1    /* if / else 语句 */
#define K_LISP_CONDITION_COND           2    /* switch 语句 */

/* 得到 list 的函数名, 如果有的话... */
char *k_lisp_get_func_name(const struct k_link *list)
{
    struct k_link *temp;
    
    if(list == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : list point to NULL!", __FUNCTION__);
        return NULL;
    }
    temp = list -> this_node;
    if(temp == NULL || !(temp -> type & K_LISP_ATOM_SYMBOL) == K_LISP_ATOM_SYMBOL){
        return NULL;
    }
    return ((struct k_data *)temp -> p_data) -> p_data;
}

/* 得到 node 的 symbol, 如果为 symbol 的话... */
char *k_lisp_get_node_symbol(const struct k_link *node)
{
    if(node == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : some pointer is NULL!", __FUNCTION__);
        return NULL;
    }
    if(!(node -> type & K_LISP_ATOM_SYMBOL) == K_LISP_ATOM_SYMBOL){
        return NULL;
    }
    
    return ((struct k_data *)node -> p_data) -> p_data;
}

/* 条件分支 - if语句 */
struct k_link *k_lisp_condition_if(const struct k_link *list)
{
    int r;
    struct k_link *judge, *case_true, *case_false, *judge_result, *ret;
    if(list == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : list point to NULL!", __FUNCTION__);
        return NULL;
    }
    
    if(list -> this_node == NULL || (judge = list -> this_node -> next_node) == NULL || (case_true = judge -> next_node) == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : Too few parameters!", __FUNCTION__);
        return NULL;
    }
    case_false = case_true -> next_node;
    
    judge_result = k_lisp_eval(judge);
    r = k_lisp_convert_bool_symbol_to_int(judge_result);
    if(r == iTRUE){
        ret = k_lisp_eval(case_true);
    }else if(r == iFALSE){
        ret = case_false == NULL ? k_lisp_get_false() : k_lisp_eval(case_false);
    }else{
        ret = k_lisp_get_false();
    }
    
    k_lisp_node_free(judge_result);
    
    return ret;
}

/* 引用, 这里返回值为 _quote() 的参数本身 */
struct k_link *k_lisp_quote(const struct k_link *list)
{
    if(list == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : list point to NULL!", __FUNCTION__);
        return NULL;
    }
    if(list -> this_node == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : the list don't have quote symbol !", __FUNCTION__);
        return NULL;
    }
    if(list -> this_node -> next_node == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : quote must have one parameter at least!", __FUNCTION__);
        return NULL;
    }
    
    return k_link_node_copy(list -> this_node -> next_node);
}

/* lambda ... */
// TODO: lambda 修改了 list..., 实际上不鼓励这么做. 内存未完全释放 ！！！！！！！！ 
struct k_link *k_lisp_lambda(struct k_link *list)
{
    int index, para_num, cmp;
    char *func_name, para_name[128];
    struct k_link *now, *root, *para, *temp;
    int stack_pos;
    void *stack[1024];
    
    if(list == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : symbol point to NULL!", __FUNCTION__);
        return NULL;
    }
    func_name = k_lisp_get_func_name(list);
    if(func_name == NULL || strcmp(func_name, K_LISP_LAMBDA) != 0){
        my_error(fp_error, MODE_ERROR, "<%s> : can't get the symbol of '_lambda'!", __FUNCTION__);
        return NULL;
    }
    if(k_link_nodes_in_list(list) != 3){    /* 包括 '_lambda', 绑定列表, 主体 共三个列表 */
        my_error(fp_error, MODE_ERROR, "<%s> : parameter num can't suit!", __FUNCTION__);
        return NULL;
    }
    
    para = list -> this_node -> next_node;
    para_num = k_link_nodes_in_list(para);
    root = list -> this_node -> next_node -> next_node;
    now = root;
    //"(_lambda (x y) (_add (_mul 2 x) (_mul 3 y)))";
    //printf("root: \n");
    //k_link_traversal_print_list_handle(root, 4, 0, NULL, NULL);
//    printf("&&&&  check root is :\n");
//    k_link_print_node_handle(root -> this_node -> pre_node, NULL, NULL);
//    k_link_traversal_print_child_handle(root -> this_node -> pre_node, 4, 0, NULL, NULL);
//    char *str_list = k_lisp_list_sprint_malloc(list, 4, 0);
//    my_error(fp_error, MODE_ERROR, "<%s> : \nroot ---------------\n%s", __FUNCTION__, str_list);
//    free(str_list);
    
    
    stack_pos = 0;
    
    while(iTRUE){
        if(now == NULL){
            break;
        }
        /* 进入谷底... */
        while(now -> this_node != NULL){
            stack[stack_pos ++] = now;
            now = now -> this_node;
        }
        
//        printf("---------\n");
//        k_link_print_node_handle(now, NULL, NULL);
        
        if((now -> type & K_LISP_ATOM_SYMBOL) == K_LISP_ATOM_SYMBOL){    /* 仅替换 symbol */
            for(index = 0, temp = para -> this_node; index < para_num; index ++){
                cmp = k_data_comp((struct k_data *)temp -> p_data, (struct k_data *)now -> p_data);
                if(cmp == 0){
                    sprintf(para_name, "%s_%d", K_LISP_PARA_FERFIX, index);
                    //printf("suit: %s\n", para_name);
                    temp = k_link_node_just_malloc(k_lisp_store_in_k_data_malloc(K_LISP_ATOM_SYMBOL, para_name), K_LISP_ATOM_SYMBOL);
                    k_link_node_substitute(now, temp);
                    k_lisp_node_free(now);    /* Warning: 这里的释放有风险 */
                    now = temp;
                    break;
                }
                temp = temp -> next_node;
            }
        }
        
        if(now -> next_node != NULL){
            now = now -> next_node;
        }else{
            while(stack_pos > 0){
                now = stack[-- stack_pos];
                if(now -> next_node != NULL)
                    break;
            }
            if(now -> next_node != NULL)
                now = now -> next_node;
            if(stack_pos == 0){
                break;
            }
        }
    }
    
    //k_lisp_node_free(list -> this_node);
    //root -> this_node -> pre_node -> pre_node -> next_node = NULL;
    //root -> pre_node = NULL;
    
    return root;    /* 返回原子... */    /* TODO */ 
}

/* lisp eval: 计算函数 */
struct k_link *k_lisp_eval_function(struct k_link *list)
{
    struct k_link *result;
    char *func_name;
    void *func;
    
    func_name = k_lisp_get_func_name(list);
    if(func_name == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : can't get the func_name(symbol)!", __FUNCTION__);
        return NULL;
    }
    func = k_lisp_get_function_address_from_tree(K_LISP_COMPOSITE_FUNC_TREE, func_name);
    if(func == NULL){
        func = k_lisp_get_function_address_from_tree(K_LISP_BUILD_IN_FUNC_TREE, func_name);
        if(func == NULL){
            printf("func:%s    addr:%p\n", func_name, func);
            k_tree_ergodic(K_LISP_BUILD_IN_FUNC_TREE, k_lisp_load_function_tree_print_func_name);
            my_error(fp_error, MODE_ERROR, "<%s> : can't find function: <%s>!", __FUNCTION__, func_name);
            return NULL;
        }
        result = k_lisp_eval_build_in_function(list, func);
    }else{
        result = k_lisp_eval_lambda_function(list, func);
    }

    return result;
}

/* 计算由 defun(lambda) 定义的函数 */
struct k_link *k_lisp_eval_lambda_function(struct k_link *list, void *func)
{
    int i, index, record_pos;
    char *func_name, *symbol;
    struct k_link *now, *para_head, *temp, *ret;
    struct k_link *function = NULL;
    struct k_link *record[1024];
    int stack_pos;
    void *stack[1024];
    char format[128];
    
    if(list == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : symbol point to NULL!", __FUNCTION__);
        return NULL;
    }
    
    func_name = k_lisp_get_func_name(list);
    if(func_name == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : can't get the func_name(symbol)!", __FUNCTION__);
        return NULL;
    }
    
    /* TODO: 释放 function */
    k_link_node_copy_list_all(&function, func);
    //function = (struct k_link *)func;
//    char *str_list = k_lisp_list_sprint_malloc(function, 4, 0);
//    my_error(fp_error, MODE_ERROR, "<%s> : \n function------\n%s", __FUNCTION__, str_list);
//    free(str_list);
    //getchar();
    if(function == NULL){
        //k_tree_ergodic(K_LISP_COMPOSITE_FUNC_TREE, k_lisp_load_function_tree_print_func_name);
        //my_error(fp_error, MODE_ERROR, "<%s> : can't get the function: <%s>!", __FUNCTION__, func_name);
        return NULL;
    }
//    printf("## child:\n");
//    k_link_print_node_handle(function -> this_node -> pre_node, NULL, NULL);

    now = function;
    para_head = list -> this_node -> next_node;

    record_pos = 0;
    sprintf(format, "%s_%%d", K_LISP_PARA_FERFIX);
    stack_pos = 0;
    while(iTRUE){
        if(now == NULL){
            break;
        }
        /* 进入谷底... */
        while(now -> this_node != NULL){
            stack[stack_pos ++] = now;
            now = now -> this_node;
        }
//        printf("---------\n");
//        k_link_print_node_handle(now, NULL, NULL);   
        
        if((now -> type & K_LISP_ATOM_SYMBOL) == K_LISP_ATOM_SYMBOL){    /* 仅替换 symbol */
            symbol = (char *)((struct k_data *)now -> p_data) -> p_data;
            //printf("symbol = %s\n", symbol);
            index = -1;
            if(sscanf(symbol, format, &index) == 1 && index != -1){
                //printf("find parameter !!\n");
                temp = k_link_get_node_offset(para_head, index);
                temp = k_link_node_copy(temp);    /* 已经拷贝了新的一份 */
                record[record_pos ++] = now;
                record[record_pos ++] = temp;
                k_link_node_substitute(now, temp);    /* 交换 now 和 temp */
                now = temp;    /* 这一步很重要 */
            }
        }
        
        if(now -> next_node != NULL){
            now = now -> next_node;
        }else{
            while(stack_pos > 0){
                now = stack[-- stack_pos];
                if(now -> next_node != NULL)
                    break;
            }
            if(now -> next_node != NULL)
                now = now -> next_node;
            if(stack_pos == 0){
                break;
            }
        }
    }
    
    After_All:

    ret = k_lisp_eval(function);

    for(i = 0; i < record_pos; i += 2){
        now = record[i];
        temp = record[i + 1];
        k_link_node_substitute(temp, now);
        k_lisp_node_free(temp);
    }
    
    k_lisp_list_free_all(function);
    
    return ret;    /* 返回原子... */
}

/* 绑定参数到函数 */
struct k_link *k_lisp_function_define(struct k_link *list)
{
    struct k_link *lambda, *lambda_func, *t;
    char *func_name, *list_name;
    
    if(list == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : symbol point to NULL!", __FUNCTION__);
        return NULL;
    }
    list_name = k_lisp_get_func_name(list);
    if(list_name == NULL || strcmp(list_name, K_LISP_DEFUN) != 0){
        my_error(fp_error, MODE_ERROR, "<%s> : can't get the symbol of %s!", __FUNCTION__, K_LISP_DEFUN);
        return NULL;
    }
    if(k_link_nodes_in_list(list) != 4){    /* 包括 '_defun', 函数名, 绑定列表, 主体 共三个列表 */
        my_error(fp_error, MODE_ERROR, "<%s> : parameter num can't suit!", __FUNCTION__);
        return NULL;
    }
    char *str_list = k_lisp_list_sprint_malloc(list, 4, 0);
    my_error(fp_error, MODE_ERROR, "<%s> : \n define list ---------------\n%s", __FUNCTION__, str_list);
    free(str_list);
    
    lambda = k_link_node_just_malloc(k_data_just_malloc(K_LISP_DEFAULT_LIST, KDATA_TYPE_STRING), K_LISP_TYPE_LIST); /* list 分配内存 */
    t = k_link_node_just_malloc(k_lisp_store_in_k_data_malloc(K_LISP_ATOM_SYMBOL, K_LISP_LAMBDA), K_LISP_ATOM_SYMBOL);
    k_link_node_copy_to_this_tail(lambda, t);
    k_link_node_copy_to_this_tail(lambda, list -> this_node -> next_node -> next_node);
    k_link_node_copy_to_this_tail(lambda, list -> this_node -> next_node -> next_node -> next_node);
    
//    str_list = k_lisp_list_sprint_malloc(lambda, 4, 0);
//    my_error(fp_error, MODE_ERROR, "<%s> : \n lambda ---------------\n%s", __FUNCTION__, str_list);
//    free(str_list);
    
    lambda_func = k_lisp_lambda(lambda);
    
//    printf("## lambda_func:\n");
//    k_link_print_node_handle(lambda_func -> this_node -> pre_node, NULL, NULL);

    func_name = k_lisp_get_node_symbol(list -> this_node -> next_node);
    if(func_name != NULL){
        k_lisp_load_composite_functions_to_tree(&K_LISP_COMPOSITE_FUNC_TREE, func_name, lambda_func);
    }else{
        my_error(fp_error, MODE_ERROR, "<%s> : function name must be symbol!", __FUNCTION__);
        return NULL;
    }
    
    //k_lisp_list_free_level_1(list);
    //k_link_traversal_print_all_handle(lambda_func, 4, 0, NULL, NULL);
    
    return lambda_func;    /* 返回原子... */
}

/* 执行命令 */
struct k_link *k_lisp_master(char *command)
{
    char *print;
    struct k_link *list, *result;
    char buf[K_STRING_MAX_BYTES];
    
    result = NULL;
    buf[0] = '\0';
    sprintf(buf + strlen(buf), "------------------------------------------ Master start ----\nCommand is: %s\n", command);
    list = k_lisp_parser_list_malloc(command);
    if(list == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : command can't accept\ncommand: %s!", __FUNCTION__, command);
        sprintf(buf + strlen(buf), "Error: error occurs when resolve list ...\n");
    }else{
        print = k_lisp_list_sprint_malloc(list, 4, 1);
        sprintf(buf + strlen(buf), "List is: \n%s\n", print);
        free(print);
        result = k_lisp_eval(list);
        if(result != NULL){
            print = k_lisp_list_sprint_malloc(result, 4, 1);
            sprintf(buf + strlen(buf), "Result is: \n%s\n", print);
            free(print);
        }else{
            sprintf(buf + strlen(buf), "Error... \n", print);
        }
    }
    sprintf(buf + strlen(buf), "------------------------------------------ Master end   ----\n\n\n", command);
    
    printf("%s", buf);
 
    return result;
}

/* 得到 symbol 对应的符号 */
struct k_link *k_lisp_get_symbol(char *symbol)
{
    char *s;
    struct k_link *r;
    
    if(symbol == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : symbol point to NULL!", __FUNCTION__);
        return NULL;
    }
    
    s = (char *)malloc(strlen(symbol) + 1);
    strcpy(s, symbol);
    k_link_node_malloc(&r, k_lisp_store_in_k_data_malloc(K_LISP_ATOM_SYMBOL, s), K_LISP_ATOM_SYMBOL);
    
    return r;
}

/* 得到符号 T  */
struct k_link *k_lisp_get_true()
{
    return k_lisp_get_symbol("T");
}

/* 得到符号 NIL  */
struct k_link *k_lisp_get_false()
{
    return k_lisp_get_symbol("NIL");
}

/* 将 T or NIL 转换为 iTRUE, iFALSE, iUNKNOWN */
int k_lisp_convert_bool_symbol_to_int(const struct k_link *list)
{
    char *s;
    
    if(list == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : list point to NULL!", __FUNCTION__);
        return iUNKNOWN;
    }
    if(list -> type != K_LISP_ATOM_SYMBOL){
        my_error(fp_error, MODE_ERROR, "<%s> : can't accept lisp_type: %#x!", __FUNCTION__, list -> type);
        return iUNKNOWN;
    }
    s = (char *)((struct k_data *)list -> p_data) -> p_data;
    if(strcmp(s, K_LISP_TRUE) == 0){
        return iTRUE;
    }else if(strcmp(s, K_LISP_FALSE) == 0){
        return iFALSE;
    }else{
        return iUNKNOWN;
    }
    
    return iUNKNOWN;
}


/* 断言 */
struct k_link *k_lisp_predicate(struct k_link *list, int type)
{
    int cmp;
    char *func_name;
    struct k_link *r, *temp, *para_1, *para_2, *ret;
    K_LISP_FLOAT f1;
    K_LISP_INTEGER d1;
    
    if(list == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : list point to NULL!", __FUNCTION__);
        return NULL;
    }
    temp = list -> this_node;
    if(!(temp -> type & K_LISP_ATOM_SYMBOL) == K_LISP_ATOM_SYMBOL){
        my_error(fp_error, MODE_ERROR, "<%s> : first of predicate-list must be SYMBOL!", __FUNCTION__);
        return NULL;
    }
    func_name = (char *)((struct k_data *)temp -> p_data) -> p_data;
    
    switch(type){
        case K_LISP_PREDICATE_ATOMP:
            if(list -> this_node == NULL){
                return k_lisp_get_true();
            }else{
                return k_lisp_get_false();
            }
            break;
        case K_LISP_PREDICATE_LISTP:
            if(list -> this_node != NULL){
                return k_lisp_get_true();
            }else{
                return k_lisp_get_false();
            }
            break;
        case K_LISP_PREDICATE_EQ:    case K_LISP_PREDICATE_LT:    case K_LISP_PREDICATE_LE:
            if((para_1 = temp -> next_node) == NULL || (para_2 = para_1 -> next_node) == NULL){
                my_error(fp_error, MODE_ERROR, "<%s> : Too few parameters: <%s>!", __FUNCTION__, func_name);
                return NULL;
            }
            if((para_1 -> type & K_LISP_ATOM_NUMBER) == K_LISP_ATOM_NUMBER && 
                    (para_2 -> type & K_LISP_ATOM_NUMBER) == K_LISP_ATOM_NUMBER){
                r = k_lisp_arithmetic_sub(list);    /* 这里有风险, 如果 _sub() 检查参数就会发现不匹配 */
                if(r -> type == K_LISP_NUMBER_INTEGER){
                    d1 = *(K_LISP_INTEGER *)(((struct k_data *)r -> p_data) -> p_data);
                    if(d1 == 0){
                        cmp = K_CMP_EQUAL;
                    }else if(d1 > 0){
                        cmp = K_CMP_BIGGER;
                    }else if(d1 < 0){
                        cmp = K_CMP_SMALLER;
                    }
                }else if(r -> type == K_LISP_NUMBER_FLOAT){
                    f1 = *(K_LISP_FLOAT *)(((struct k_data *)r -> p_data) -> p_data);
                    if(f1 == 0.0){
                        cmp = K_CMP_EQUAL;
                    }else if(f1 > 0.0){
                        cmp = K_CMP_BIGGER;
                    }else if(f1 < 0.0){
                        cmp = K_CMP_SMALLER;
                    }
                }
            }else if((para_1 -> type & K_LISP_ATOM_STRING) == K_LISP_ATOM_STRING && 
                    (para_2 -> type & K_LISP_ATOM_STRING) == K_LISP_ATOM_STRING){
                cmp = k_data_comp((struct k_data *)para_1 -> p_data, (struct k_data *)para_2 -> p_data);
            }else if((para_1 -> type & K_LISP_ATOM_SYMBOL) == K_LISP_ATOM_SYMBOL && 
                    (para_2 -> type & K_LISP_ATOM_SYMBOL) == K_LISP_ATOM_SYMBOL){
                cmp = k_data_comp((struct k_data *)para_1 -> p_data, (struct k_data *)para_2 -> p_data);
            }
            if(cmp == K_CMP_EQUAL){
                if(type == K_LISP_PREDICATE_EQ)         return k_lisp_get_true();
                else if(type == K_LISP_PREDICATE_LT)    return k_lisp_get_false();
                else if(type == K_LISP_PREDICATE_LE)    return k_lisp_get_true();
            }else if(cmp == K_CMP_BIGGER){
                if(type == K_LISP_PREDICATE_EQ)         return k_lisp_get_false();
                else if(type == K_LISP_PREDICATE_LT)    return k_lisp_get_false();
                else if(type == K_LISP_PREDICATE_LE)    return k_lisp_get_false();
            }else if(cmp == K_CMP_SMALLER){
                if(type == K_LISP_PREDICATE_EQ)         return k_lisp_get_false();
                else if(type == K_LISP_PREDICATE_LT)    return k_lisp_get_true();
                else if(type == K_LISP_PREDICATE_LE)    return k_lisp_get_true();
            }
        default:
            my_error(fp_error, MODE_ERROR, "<%s> : can't accpet K_LISP_PREDICATE type: %d or parameter type can't suit!", __FUNCTION__, type);
            return NULL;
            break;
    }
    
    return ret;
}

/* lisp 断言, 为原子 */
struct k_link *k_lisp_predicate_atomp(struct k_link *list)
{
    struct k_link *r;
    
    r = k_lisp_predicate(list, K_LISP_PREDICATE_ATOMP);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* lisp 断言, 为列表 */
struct k_link *k_lisp_predicate_listp(struct k_link *list)
{
    struct k_link *r;
    
    r = k_lisp_predicate(list, K_LISP_PREDICATE_LISTP);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* lisp 断言, 相等 */
struct k_link *k_lisp_predicate_eq(struct k_link *list)
{
    struct k_link *r;
    
    r = k_lisp_predicate(list, K_LISP_PREDICATE_EQ);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* lisp 断言, 小于 */
struct k_link *k_lisp_predicate_lt(struct k_link *list)
{
    struct k_link *r;
    
    r = k_lisp_predicate(list, K_LISP_PREDICATE_LT);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* lisp 断言, 小于等于 */
struct k_link *k_lisp_predicate_le(struct k_link *list)
{
    struct k_link *r;
    
    r = k_lisp_predicate(list, K_LISP_PREDICATE_LE);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* k_lisp 算术双目运算 */
struct k_link *k_lisp_arithmetic_two(const struct k_link *list, int type)
{
    struct k_link *r;
    struct k_link *a, *b, *temp;
    char *func_name;
    K_LISP_FLOAT f1, f2;
    K_LISP_INTEGER d1, d2;
    
    if(list == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : list point to NULL!", __FUNCTION__);
        return NULL;
    }
    if(list -> this_node == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : list is empty!", __FUNCTION__);
        return NULL;
    }
    
    temp = list -> this_node;
    if(!(temp -> type & K_LISP_ATOM_SYMBOL) == K_LISP_ATOM_SYMBOL){
        my_error(fp_error, MODE_ERROR, "<%s> : first of list must be SYMBOL!", __FUNCTION__);
        return NULL;
    }
    func_name = (char *)((struct k_data *)temp -> p_data) -> p_data;
    if((a = temp -> next_node) == NULL || (b = temp -> next_node -> next_node) == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : Too few parameter be given: <%s>!", __FUNCTION__, func_name);
        return NULL;
    }
    
    if(a -> type != K_LISP_NUMBER_FLOAT && a -> type != K_LISP_NUMBER_INTEGER &&
            b -> type != K_LISP_NUMBER_FLOAT && b -> type != K_LISP_NUMBER_INTEGER){
        my_error(fp_error, MODE_ERROR, "<%s> : can't accpet lisp_type for parameter: <%s>!", __FUNCTION__, func_name);
        return NULL;
    }
    
    if(a -> type == K_LISP_NUMBER_FLOAT || b -> type == K_LISP_NUMBER_FLOAT){
        if(a -> type == K_LISP_NUMBER_FLOAT){
            f1 = *(K_LISP_FLOAT *)(((struct k_data *)a -> p_data) -> p_data);
        }else if(a -> type == K_LISP_NUMBER_INTEGER){
            f1 = *(K_LISP_INTEGER *)(((struct k_data *)a -> p_data) -> p_data);
        }else{    /* 其实前面已经检查过了 */
            my_error(fp_error, MODE_ERROR, "<%s> : can't accpet lisp_type : %#x : <%s>!", __FUNCTION__, a -> type, func_name);
            return NULL;
        }
        
        if(b -> type == K_LISP_NUMBER_FLOAT){
            f2 = *(K_LISP_FLOAT *)(((struct k_data *)b -> p_data) -> p_data);
        }else if(b -> type == K_LISP_NUMBER_INTEGER){
            f2 = *(K_LISP_INTEGER *)(((struct k_data *)b -> p_data) -> p_data);;
        }else{
            my_error(fp_error, MODE_ERROR, "<%s> : can't accpet lisp_type : %#x : <%s>!", __FUNCTION__, a -> type, func_name);
            return NULL;
        }
        
        switch(type){
            case K_LISP_ARITHMETIC_ADD:
                f1 = f1 + f2;    break;
            case K_LISP_ARITHMETIC_SUB:
                f1 = f1 - f2;    break;
            case K_LISP_ARITHMETIC_MUL:
                f1 = f1 * f2;    break;
            case K_LISP_ARITHMETIC_DIV:
                if(f2 == 0){
                    my_error(fp_error, MODE_ERROR, "<%s> : The divisor is zero!", __FUNCTION__);
                    return NULL;
                }else{
                    f1 = f1 / f2;
                }
                break;
            case K_LISP_ARITHMETIC_POW:
                /* TODO: 定义域 */
                f1 = pow(f1, f2);    break;
            case K_LISP_ARITHMETIC_MOD:
                f1 = fmod(f1, f2);    break;
            default:
                my_error(fp_error, MODE_ERROR, "<%s> : can't accpet K_LISP_ARITHMETIC type: %d!", __FUNCTION__, type);
                return NULL;
        } 
        k_link_node_malloc(&r, k_lisp_store_in_k_data_malloc(K_LISP_NUMBER_FLOAT, &f1), K_LISP_NUMBER_FLOAT);
    }else if(a -> type == K_LISP_NUMBER_INTEGER && b -> type == K_LISP_NUMBER_INTEGER){
        d1 = *(K_LISP_INTEGER *)(((struct k_data *)a -> p_data) -> p_data);
        d2 = *(K_LISP_INTEGER *)(((struct k_data *)b -> p_data) -> p_data);
        switch(type){
            case K_LISP_ARITHMETIC_ADD:
                d1 = d1 + d2;    break;
            case K_LISP_ARITHMETIC_SUB:
                d1 = d1 - d2;    break;
            case K_LISP_ARITHMETIC_MUL:
                d1 = d1 * d2;    break;
            case K_LISP_ARITHMETIC_DIV:
                if(d2 == 0){
                    my_error(fp_error, MODE_ERROR, "<%s> : The divisor is zero!", __FUNCTION__);
                    return NULL;
                }else{
                    d1 = d1 / d2;
                }
                break;
            case K_LISP_ARITHMETIC_POW:
                /* TODO: 定义域 */
                d1 = pow(d1, d2);    break;
            case K_LISP_ARITHMETIC_MOD:
                d1 = d1 - (d1 / d2) * d2;    break;    /* 对负数的支持就 呵呵 了...  */
            default:
                my_error(fp_error, MODE_ERROR, "<%s> : can't accpet K_LISP_ARITHMETIC type: %d!", __FUNCTION__, type);
                return NULL;
        }
        k_link_node_malloc(&r, k_lisp_store_in_k_data_malloc(K_LISP_NUMBER_INTEGER, &d1), K_LISP_NUMBER_INTEGER); 
    }
    
    return r;
}

/* k_lisp 算术单目运算 */
struct k_link *k_lisp_arithmetic_one(struct k_link *list, int type)
{
    int mark;    /* mark = 0, f 有效, mark = 1, d 有效, mark = -1, error */ 
    char *func_name;
    struct k_link *r, *a, *temp;
    K_LISP_FLOAT f;
    K_LISP_INTEGER d;
    
    if(list == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : list point to NULL!", __FUNCTION__);
        return NULL;
    }
    if(list -> this_node == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : list is empty!", __FUNCTION__);
        return NULL;
    }
    
    temp = list -> this_node;
    if(!(temp -> type & K_LISP_ATOM_SYMBOL) == K_LISP_ATOM_SYMBOL){
        my_error(fp_error, MODE_ERROR, "<%s> : first of list must be SYMBOL!", __FUNCTION__);
        return NULL;
    }
    func_name = (char *)((struct k_data *)temp -> p_data) -> p_data;
    if((a = temp -> next_node) == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : Too few parameter be given: <%s>!", __FUNCTION__, func_name);
        return NULL;
    }

    if(a -> type != K_LISP_NUMBER_FLOAT && a -> type != K_LISP_NUMBER_INTEGER){
        my_error(fp_error, MODE_ERROR, "<%s> : can't accpet lisp_type: %#x : <%s>!", __FUNCTION__, a -> type, func_name);
        return NULL;
    }
    
    mark = -1;
    if(a -> type == K_LISP_NUMBER_FLOAT){
        f = *(K_LISP_FLOAT *)(((struct k_data *)a -> p_data) -> p_data);
        d = (K_LISP_INTEGER)*(K_LISP_FLOAT *)(((struct k_data *)a -> p_data) -> p_data);    /* 强转 */
    }
    else if(a -> type == K_LISP_NUMBER_INTEGER){
        f = (K_LISP_FLOAT)*(K_LISP_INTEGER *)(((struct k_data *)a -> p_data) -> p_data);    /* 强转 */
        d = *(K_LISP_INTEGER *)(((struct k_data *)a -> p_data) -> p_data);
    }
    switch(type){
        case K_LISP_ARITHMETIC_SQRT:
            if(f < 0){
                my_error(fp_error, MODE_ERROR, "<%s> : sqrt(x) error,  x must >= 0!", __FUNCTION__);
                mark = -1;
            }else{
                f = sqrt(f);    mark = 0;
            }
            break;
        case K_LISP_ARITHMETIC_LOG:
            f = log(f);    mark = 0;    /* 以 e 为底 */
            break;
        case K_LISP_ARITHMETIC_ABS:
            if(a -> type == K_LISP_NUMBER_FLOAT){
                f = fabs(f);    mark = 0;
            }else{
                d = abs(d);    mark = 1;}
            break;
        case K_LISP_ARITHMETIC_SIN:
            f = sin(f);    mark = 0;
            break;
        case K_LISP_ARITHMETIC_COS:
            f = cos(f);    mark = 0;
            break;
        case K_LISP_ARITHMETIC_TAN:
            f = tan(f);    mark = 0;
            break;
        case K_LISP_ARITHMETIC_CEIL:
            d = ceil(f);    mark = 1;
            break;
        case K_LISP_ARITHMETIC_FLOOR:
            d = floor(f);    mark = 1;
            break;
        case K_LISP_ARITHMETIC_ADD1:
            if(a -> type == K_LISP_NUMBER_FLOAT){
                my_error(fp_error, MODE_ERROR, "<%s> : can't accpet lisp_type for ADD1, must be interger: 0x%x!", __FUNCTION__, a -> type);
                mark = -1;
            }else{
                ++ d;    mark = 1;}
            break;
        case K_LISP_ARITHMETIC_SUB1:
            if(a -> type == K_LISP_NUMBER_FLOAT){
                my_error(fp_error, MODE_ERROR, "<%s> : can't accpet lisp_type for SUB1, must be interger: 0x%x!", __FUNCTION__, a -> type);
                mark = -1;
            }else{
                -- d;    mark = 1;}
            break;
        case K_LISP_ARITHMETIC_INTEGER:
            d = d;    mark = 1;
            break;
        case K_LISP_ARITHMETIC_FLOAT:
            f = f;    mark = 0;
            break;
        default:
            my_error(fp_error, MODE_ERROR, "<%s> : can't accpet K_LISP_ARITHMETIC type: %d : <%s>!", __FUNCTION__, type, func_name);
            mark = -1;
    }
    if(mark == 0)
        k_link_node_malloc(&r, k_lisp_store_in_k_data_malloc(K_LISP_NUMBER_FLOAT, &f), K_LISP_NUMBER_FLOAT);
    else if(mark == 1)
        k_link_node_malloc(&r, k_lisp_store_in_k_data_malloc(K_LISP_NUMBER_INTEGER, &d), K_LISP_NUMBER_INTEGER);
    else{
        return NULL;
    }
    
    return r;
}

/* k_lisp build-in function: add */
struct k_link *k_lisp_arithmetic_add(struct k_link *list)
{
    struct k_link *r;

    r = k_lisp_arithmetic_two(list, K_LISP_ARITHMETIC_ADD);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* k_lisp build-in function: sub */
struct k_link *k_lisp_arithmetic_sub(struct k_link *list)
{
    struct k_link *r;
    r = k_lisp_arithmetic_two(list, K_LISP_ARITHMETIC_SUB);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* k_lisp build-in function: mul */
struct k_link *k_lisp_arithmetic_mul(struct k_link *list)
{
    struct k_link *r;
    r = k_lisp_arithmetic_two(list, K_LISP_ARITHMETIC_MUL);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* k_lisp build-in function: div */
struct k_link *k_lisp_arithmetic_div(struct k_link *list)
{
    struct k_link *r;
    r = k_lisp_arithmetic_two(list, K_LISP_ARITHMETIC_DIV);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* k_lisp build-in function: pow */
struct k_link *k_lisp_arithmetic_pow(struct k_link *list)
{
    struct k_link *r;
    r = k_lisp_arithmetic_two(list, K_LISP_ARITHMETIC_POW);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* k_lisp build-in function: mod */
struct k_link *k_lisp_arithmetic_mod(struct k_link *list)
{
    struct k_link *r;
    r = k_lisp_arithmetic_two(list, K_LISP_ARITHMETIC_MOD);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* k_lisp build-in function: sqrt */
struct k_link *k_lisp_arithmetic_sqrt(struct k_link *list)
{
    struct k_link *r;
    r = k_lisp_arithmetic_one(list, K_LISP_ARITHMETIC_SQRT);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* k_lisp build-in function: log */
struct k_link *k_lisp_arithmetic_log(struct k_link *list)
{
    struct k_link *r;
    r = k_lisp_arithmetic_one(list, K_LISP_ARITHMETIC_LOG);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* k_lisp build-in function: abs */
struct k_link *k_lisp_arithmetic_abs(struct k_link *list)
{
    struct k_link *r;
    r = k_lisp_arithmetic_one(list, K_LISP_ARITHMETIC_ABS);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* k_lisp build-in function: sin */
struct k_link *k_lisp_arithmetic_sin(struct k_link *list)
{
    struct k_link *r;
    r = k_lisp_arithmetic_one(list, K_LISP_ARITHMETIC_SIN);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* k_lisp build-in function: cos */
struct k_link *k_lisp_arithmetic_cos(struct k_link *list)
{
    struct k_link *r;
    r = k_lisp_arithmetic_one(list, K_LISP_ARITHMETIC_COS);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* k_lisp build-in function: tan */
struct k_link *k_lisp_arithmetic_tan(struct k_link *list)
{
    struct k_link *r;
    r = k_lisp_arithmetic_one(list, K_LISP_ARITHMETIC_TAN);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* k_lisp build-in function: ceil */
struct k_link *k_lisp_arithmetic_ceil(struct k_link *list)
{
    struct k_link *r;
    r = k_lisp_arithmetic_one(list, K_LISP_ARITHMETIC_CEIL);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* k_lisp build-in function: floor */
struct k_link *k_lisp_arithmetic_floor(struct k_link *list)
{
    struct k_link *r;
    r = k_lisp_arithmetic_one(list, K_LISP_ARITHMETIC_FLOOR);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* k_lisp build-in function: add1 */
struct k_link *k_lisp_arithmetic_add1(struct k_link *list)
{
    struct k_link *r;
    r = k_lisp_arithmetic_one(list, K_LISP_ARITHMETIC_ADD1);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* k_lisp build-in function: sub1 */
struct k_link *k_lisp_arithmetic_sub1(struct k_link *list)
{
    struct k_link *r;
    r = k_lisp_arithmetic_one(list, K_LISP_ARITHMETIC_SUB1);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* k_lisp build-in function: integer */
struct k_link *k_lisp_arithmetic_integer(struct k_link *list)
{
    struct k_link *r;
    r = k_lisp_arithmetic_one(list, K_LISP_ARITHMETIC_INTEGER);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

/* k_lisp build-in function: float */
struct k_link *k_lisp_arithmetic_float(struct k_link *list)
{
    struct k_link *r;
    r = k_lisp_arithmetic_one(list, K_LISP_ARITHMETIC_FLOAT);
    if(r == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : error happends!", __FUNCTION__);
        return NULL;
    }
    
    return r;
}

struct k_lisp_function_map K_LISP_BUILD_IN_FUNCTIONS[K_LISP_BUILD_IN_FUNCTION_NUM] = {
    /* 算术运算 */
    "_add"      , k_lisp_arithmetic_add, 
    "_sub"      , k_lisp_arithmetic_sub, 
    "_mul"      , k_lisp_arithmetic_mul, 
    "_div"      , k_lisp_arithmetic_div, 
    "_pow"      , k_lisp_arithmetic_pow, 
    "_mod"      , k_lisp_arithmetic_mod, 
    "_sqrt"     , k_lisp_arithmetic_sqrt, 
    "_log"      , k_lisp_arithmetic_log, 
    "_abs"      , k_lisp_arithmetic_abs, 
    "_sin"      , k_lisp_arithmetic_sin, 
    "_cos"      , k_lisp_arithmetic_cos, 
    "_tan"      , k_lisp_arithmetic_tan, 
    "_ceil"     , k_lisp_arithmetic_ceil,
    "_floor"    , k_lisp_arithmetic_floor,
    "_add1"     , k_lisp_arithmetic_add1, 
    "_sub1"     , k_lisp_arithmetic_sub1, 
    "_integer"  , k_lisp_arithmetic_integer, 
    "_float"    , k_lisp_arithmetic_float, 
    
    /* 断言 */ 
    "_atomp"    , k_lisp_predicate_atomp,
    "_listp"    , k_lisp_predicate_listp,
    "_eq"       , k_lisp_predicate_eq,
    "_lt"       , k_lisp_predicate_lt,
    "_le"       , k_lisp_predicate_le,
    
    /* 条件分支 */ 
    "_if"       , k_lisp_condition_if,
    
    /* others */
    K_LISP_QUOTE    , k_lisp_quote, 
    K_LISP_DEFUN    , k_lisp_function_define,
    K_LISP_LAMBDA   , k_lisp_lambda,
    NULL            , NULL
};


/* 得到 lisp 类型对应的字符串名称 */
char *k_lisp_get_type_name(int type)
{
    int i;
    
    for(i = 0; i < K_LISP_TYPE_NUM; i ++){
        if(type == k_lisp_type_name[i].type){
            return k_lisp_type_name[i].name;
        }
    }
    
    my_error(fp_error, MODE_ERROR, "<%s> : can't accpet type : %x!", __FUNCTION__, type);
    return NULL;
}

/* 参数个数超过 K_LISP_MAX_PARAMETER_WITH_BUILD_IN, 参数直接使用 func_list */
#define K_LISP_MAX_PARAMETER_WITH_BUILD_IN    7
/* k_lisp eval 函数, 使用内置函数求值 */
struct k_link *k_lisp_eval_build_in_function_use_address(struct k_link *func_list, void *func_build_in)
{
    struct k_link *result;
    struct k_link *(*func_build_in_default)(struct k_link *list);
    
    if(func_list == NULL || func_build_in == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : some pointer is NULL !", __FUNCTION__);
        return NULL;
    }
    
    func_build_in_default = func_build_in;
    result = func_build_in_default(func_list);

    return result;
}

/* k_lisp eval 函数, 使用内置函数求值 */
struct k_link *k_lisp_eval_build_in_function(struct k_link *func_list, void *func)
{
    void *func_address;
    struct k_link *result;
    
    if(func_list == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : func_list point to NULL !", __FUNCTION__);
        return NULL;
    }

    
    if(!(func_list -> this_node -> type & K_LISP_ATOM_SYMBOL) == K_LISP_ATOM_SYMBOL){
        my_error(fp_error, MODE_ERROR, "<%s> : car of func_list must be SYMBOL !", __FUNCTION__);
        return NULL;
    }

    func_address = func;
    if(func_address == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : func point to NULL!", __FUNCTION__);
        return NULL;
    }
    
    result = k_lisp_eval_build_in_function_use_address(func_list, func_address);
    
    return result;
}

/* 打印树节点的函数名称 */
int k_lisp_load_function_tree_print_func_name(struct k_tree *tree)
{
    if(tree == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : some pointer is NULL !", __FUNCTION__);
        return K_CMP_UNKNOWN;
    }
    printf("    func_name: %s\n", ((struct k_lisp_function_map *)tree -> p_data) -> func_name);
    
    return 0;
}
/* k_lisp bulid_in functoin load... */
/* 比较函数 */
int k_lisp_load_function_tree_cmp(void *a, void *b)
{
    if(a == NULL || b == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : some pointer is NULL !", __FUNCTION__);
        return K_CMP_UNKNOWN;
    }
    
    return k_tree_comp_string(((struct k_lisp_function_map *)a) -> func_name, 
                              ((struct k_lisp_function_map *)b) -> func_name); 
}

/* 将内置函数以函数名为键值 添加到树中 */
int k_lisp_load_build_in_functions_to_tree(struct k_tree **func_tree, struct k_lisp_function_map *func_map)
{
    int i;
    
    if(func_tree == NULL || func_map == NULL){
        my_error(fp_error, MODE_ERROR, " <%s> : some pointer is NULL !", __FUNCTION__);
        return -1;
    }
    
    while(func_map -> func_name != NULL){
        printf("[S] load    : %-16s | address: %p\n", func_map -> func_name, func_map -> func_address);
        k_tree_insert_item_to_tree(func_tree, func_map, k_lisp_load_function_tree_cmp);
        func_map ++;
    }
    
    return 0;
}

/* 将以函数名为键值 将复合函数添加到树中 */
int k_lisp_load_composite_functions_to_tree(struct k_tree **func_tree, char *func_name, struct k_link *lambda)
{
    int i;
    struct k_lisp_function_map *func;
    
    if(func_tree == NULL || lambda == NULL || func_name == NULL){
        my_error(fp_error, MODE_ERROR, " <%s> : some pointer is NULL !", __FUNCTION__);
        return -1;
    }
    
    func = (struct k_lisp_function_map *)malloc(sizeof(struct k_lisp_function_map));
    func -> func_name = (char *)malloc(strlen(func_name) + 1);
    strcpy(func -> func_name, func_name);
    func -> func_address = lambda;
    printf("\n\n[A] load    : %-16s | address: %p\n\n", func -> func_name, func -> func_address);
    //printf("load function defined: %12s | address: %lld\n", func -> func_name, func -> func_address);
    k_tree_insert_item_to_tree(func_tree, func, k_lisp_load_function_tree_cmp);
    
    return 0;
}

/* 以函数名为键值, 从树中取函数地址 */
void *k_lisp_get_function_address_from_tree(struct k_tree *func_tree, char *func_name)
{
    int i;
    struct k_lisp_function_map func_map;
    struct k_tree *find;
    
    if(func_name == NULL){
        my_error(fp_error, MODE_ERROR, " <%s> : some pointer is NULL !", __FUNCTION__);
        return NULL;
    }
    if(func_tree == NULL){
        //my_error(fp_error, MODE_ERROR, " <%s> : some pointer is NULL !", __FUNCTION__);
        return NULL;
    }
    
    func_map.func_name = func_name;
    find = k_tree_search(func_tree, &func_map, k_lisp_load_function_tree_cmp);
    
    if(find == NULL){
        return NULL;
    }
    
    return (void *)((struct k_lisp_function_map *)find -> p_data) -> func_address;
}

/* k_lisp 初始化函数 */
int k_lisp_init()
{
    k_lisp_load_build_in_functions_to_tree(&K_LISP_BUILD_IN_FUNC_TREE, K_LISP_BUILD_IN_FUNCTIONS);
    
    return 0;
}

/* lisp eval 函数 */
struct k_link *k_lisp_eval(struct k_link *list)
{
    int i, record_pos;
    int status;
    char *func_name, *str_list;
    struct k_link *sub_list, *r, *result;
    struct k_link *record[1024];
    
    if(list == NULL){
        my_error(fp_error, MODE_ERROR, "<%s> : list point to NULL!", __FUNCTION__);
        return NULL;
    }
    
    if(list -> this_node == NULL){    /* 原子, 直接返回副本 */
        return k_link_node_just_malloc(k_lisp_store_in_k_data_malloc(list->type, 
                ((struct k_data*)list->p_data)->p_data), list->type);
    }
    
    record_pos = 0;
    status = iTRUE;
    func_name = k_lisp_get_func_name(list);
    if(func_name == NULL){
        str_list = k_lisp_list_sprint_malloc(list, 4, 0);
        my_error(fp_error, MODE_ERROR, "<%s> : can't eval the list, forget use quote ?\n------\n%s", __FUNCTION__, str_list);
        free(str_list);
        return NULL;
    }else if(strcmp(func_name, K_LISP_DEFUN) == 0 || strcmp(func_name, K_LISP_LAMBDA) == 0 || strcmp(func_name, K_LISP_IF) == 0){
        goto After_All;
    }
    
    sub_list = list -> this_node;
    while(sub_list != NULL){
        if(sub_list -> this_node != NULL){    /* 不为原子, 为函数调用 */
//            str_list = k_lisp_list_sprint_malloc(list, 4, 0);
//            my_error(fp_error, MODE_ERROR, "<%s> : list ------\n%s", __FUNCTION__, str_list);
//            free(str_list);
//            printf("--------------- start\n");
//            k_link_traversal_print_child_handle(sub_list, 4, 0, NULL, NULL);
//            printf("--------------- end\n");

            r = k_lisp_eval(sub_list);
            if(r == NULL){
                my_error(fp_error, MODE_ERROR, "<%s> : error when eval sub_list...", __FUNCTION__);
                k_link_traversal_print_child_handle(sub_list, 4, 0, NULL, NULL);
                status = iFALSE;
                goto After_All;
            }
            record[record_pos ++] = sub_list;
            record[record_pos ++] = r;
            k_link_node_substitute(sub_list, r);    /* 交换 r 和 sub_list */
            sub_list = r;
        }
        sub_list = sub_list -> next_node;
    }
    
    After_All:
    if(status == iTRUE){
        result = k_lisp_eval_function(list);
        if(result == NULL){
            str_list = k_lisp_list_sprint_malloc(list, 4, 0);
            my_error(fp_error, MODE_ERROR, "<%s> : can't eval the list, check the func_name...\n------\n%s", __FUNCTION__, str_list);
            free(str_list);
        }
    }else{
        result = NULL;
    }
    for(i = 0; i < record_pos; i += 2){
        sub_list = record[i];
        r = record[i + 1];
        k_link_node_substitute(r, sub_list);
        k_lisp_node_free(r);
    }

    //printf("return ...\n");
    
    return result;
}

#endif

int test_k_lisp_parser_word_malloc()
{
    int i;
    struct k_link *result;
    char *test[32] = {"_hello", "age", "bigger?", "REASON", "status_2", "123", "-123", "-123456789012", "123.45", 
                      "123e3", "2.0", "'string'", "'string space'"};
    for(i = 0; i < 13; i ++){
        result = k_lisp_parser_word_malloc(test[i]);
    }
    
    return 0;
}

int test_k_lisp_parser_list_malloc()
{
    char *str_list = "(_sub (_mul 4 ^hello, world^) (_add 2 (_add1 2)))";
    struct k_link *list, *r;
    list = k_lisp_parser_list_malloc(str_list);
    k_link_traversal_print_list_handle(list, 4, 0, NULL, NULL);
    
//    r = k_lisp_predicate_atomp(list);
//    k_link_traversal_print_list_handle(r, 4, 0, NULL, NULL);
//    
//    r = k_lisp_predicate_atomp(list -> this_node);
//    k_link_traversal_print_list_handle(r, 4, 0, NULL, NULL);
//    
//    r = k_lisp_predicate_atomp(list -> this_node -> next_node -> next_node);
//    k_link_traversal_print_list_handle(r, 4, 0, NULL, NULL);
    
    return 0;
}

int test_k_lisp_arithmetic()
{
    char *str_list = "(func 3 3.2)";
    char *single = "(_floor 3.14159)";
    struct k_link *list, *result, *list_single;
    iINT64 a, b;
    
    list = k_lisp_parser_list_malloc(str_list);
    k_link_traversal_print_list_handle(list, 4, 0, NULL, NULL);
    
    result = k_lisp_arithmetic_add(list);
    k_link_traversal_print_list_handle(result, 4, 0, NULL, NULL);
    
    result = k_lisp_arithmetic_sub(list);
    k_link_traversal_print_list_handle(result, 4, 0, NULL, NULL);
    
    result = k_lisp_arithmetic_mul(list);
    k_link_traversal_print_list_handle(result, 4, 0, NULL, NULL);
    
    result = k_lisp_arithmetic_div(list);
    k_link_traversal_print_list_handle(result, 4, 0, NULL, NULL);
    
    
    list_single = k_lisp_parser_list_malloc(single);
    result = k_lisp_arithmetic_floor(list_single);
    k_link_traversal_print_list_handle(result, 4, 0, NULL, NULL);
    return 0;
}

int test_k_lisp_eval_build_in_function()
{
    int i, times, a;
    double time_cost;
    char *str_list = "(_floor 3.1415926)";
    struct k_link *list, *result;
    struct k_timer timer;
    
    list = k_lisp_parser_list_malloc(str_list);
    
    k_timer_start(&timer);
    
    times = 1000000;
    for(i = 0; i < times; i ++){
        //result = k_lisp_eval_build_in_function_use_address(list, k_lisp_arithmetic_floor);
        result = k_lisp_eval_build_in_function(list, k_lisp_arithmetic_floor);
    }
    
    k_link_traversal_print_list_handle(result, 4, 0, NULL, NULL);
    
    time_cost = k_timer_stop(&timer);
    printf("computer %d functions per sec.\n", (int)(times / time_cost));
    return 0;
}

int test_k_lisp_eval()
{
    int i, times;
    char *str_list_0 = "(_sub (_mul 4 (_quote 6)) (_add 2 (_add1 2)))";
    char *str_list_1 = "(_if (_lt 2 3) \
                             (_add 2 3) \
                             (_mul 3 4))";
    struct k_link *list, *r;
    struct k_timer timer;
    double time_cost;
    
    list = k_lisp_parser_list_malloc(str_list_1);
    k_link_traversal_print_list_handle(list, 4, 0, NULL, NULL);
    
    k_timer_start(&timer);
    
    times = 100000;
    for(i = 0; i < times; i ++){
        r = k_lisp_eval(list);
        //k_lisp_node_free(r);
    }
    
    r = k_lisp_eval(list);
    printf("result = \n");
    if(r)
        k_link_traversal_print_list_handle(r, 4, 0, NULL, NULL);
    
    time_cost = k_timer_stop(&timer);
    k_link_traversal_print_list_handle(list, 4, 0, NULL, NULL);
    
    printf("computer %d functions per sec.\n", (int)(times / time_cost));
    printf("hello, world\n");
    
    return 0;
}

int test_k_lisp_lambda()
{
    char *str_list_0 = "(_lambda (x y) (_add (_mul 2 x) (_mul 3 y)))";

    struct k_link *list, *r;

    list = k_lisp_parser_list_malloc(str_list_0);
    k_link_traversal_print_list_handle(list, 4, 0, NULL, NULL);
    k_lisp_lambda(list);
    k_link_traversal_print_list_handle(list, 4, 0, NULL, NULL);
    return 0;
}

int test_k_lisp_eval_lambda_function()
{
    char *str_list_0 = "(_defun area_of_circle (r) (_mul (_mul 2 3.14159) (_mul r r) ))";
    char *str_list_1 = "(area_of_circle 1)";

    struct k_link *list, *list_func, *lambda, *r;

    list = k_lisp_parser_list_malloc(str_list_0);
    list_func = k_lisp_parser_list_malloc(str_list_1);
    k_link_traversal_print_list_handle(list, 4, 0, NULL, NULL);
    lambda = k_lisp_eval(list);
    k_link_traversal_print_list_handle(lambda, 4, 0, NULL, NULL);
    r = k_lisp_eval(list_func);
    k_link_print_node_handle(r, NULL, NULL);
    return 0;
}

int test_k_lisp_function_define()
{
    char *str_list_1 = "(_defun func (x y) (_add (_mul 2 x) (_mul 3 y)))";
    char *str_list_2 = "( _defun _Pi () (_quote 3.1415926) )";
    char *print;
    struct k_link *list_func, *lambda;

    list_func = k_lisp_parser_list_malloc(str_list_1);
    k_link_traversal_print_list_handle(list_func, 4, 0, NULL, NULL);
    lambda = k_lisp_function_define(list_func);
    printf("function:\n");
    print = k_lisp_list_sprint_malloc(lambda, 4, 0);
    printf("%s", print);
    free(print);
    return 0;
}

/* top-level function */
int test_k_lisp_master()
{
    k_lisp_master("(_defun _Pi () (_quote 3.14159265358979))");
    k_lisp_master("(_defun area_of_circle (r) (_mul (_mul 2 (_Pi)) (_mul r r) ))");
    k_lisp_master("(area_of_circle 1)");

    k_lisp_master("                         \n\
    (_defun Fibonacci (n)                   \n\
        (_if (_lt n 2)                      \n\
             1                              \n\
             (_add (Fibonacci (_sub n 1))   \n\
                   (Fibonacci (_sub n 2)))) \n\
        )");
    k_lisp_master("(Fibonacci 20)");
    return 0;
}

int main()
{
//    freopen("lisp_out.txt", "w", stdout);
    k_lisp_init();
    //test_k_lisp_parser_list_malloc();
    //test_k_lisp_arithmetic();

    //test_k_lisp_eval_build_in_function();
    //test_k_lisp_eval();
    //test_k_lisp_lambda();
    //test_k_lisp_eval_lambda_function();
    //test_k_lisp_function_define();
    test_k_lisp_master();
    printf("Done!\n");
    getchar();
    return 0;
}
