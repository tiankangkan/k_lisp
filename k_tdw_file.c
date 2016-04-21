/**
@File      : k_tdw.h
@Author    : kangtian
@Date      : 15-09-27
@Describle : TDW 查询结果缓存文件解析 
*/

#ifndef __K_TDW_H__
    #define __K_TDW_H__

#include <string.h>
#include "k_array.h"
#include "k_link.h"
#include "k_string.h"
#include "k_timer.h" 
#include "k_tree.h"

#define K_TDW_MAX_LINE_BYTES    (1024*32)
#define K_TDW_SEP_TITLE               ','
#define K_TDW_SEP_BODY                  1    /* SOH */

/* 读取 tdw query_file 到 p_table */
int k_tdw_read_query_file(FILE *fp, struct k_link **p_table);


/*******************************************************/

/* 读取 tdw query_file 到 p_table */
int k_tdw_read_query_file(FILE *fp, struct k_link **p_table)
{
    int i, index;
    char line[K_TDW_MAX_LINE_BYTES + 1];
    char sep[32];
    char *str;
    struct k_array *row;
    struct k_link *root, *link_row, *link_item;
    struct k_data *field;
    struct k_timer timer;
    struct k_tree *step_13_reason = NULL;
    
    int c_phone, c_phone_86, c_short, c_null;
    char *step1_result, *step10_result, *step13_reason, *param_peeruin, *platform;
    char type[32];
    c_phone = c_phone_86 = c_short = c_null = 0;
    
    index = 0;
    k_data_malloc(&field, "table:", KDATA_TYPE_STRING);
    k_link_node_malloc(&root, field, KDATA_TYPE_STRING);
    k_array_malloc(&row, sizeof(char *), 128);    /* 初始化为 64 个元素 */
    
    while(k_string_fgetline(fp, line, K_TDW_MAX_LINE_BYTES) != NULL){
        if(index == 0){
            sep[0] = K_TDW_SEP_TITLE, sep[1] = '\0';
            k_string_split_malloc(line, sep, ' ', iFALSE, row);
            printf("row_num = %d\n", row -> pos);
        }else{
            sep[0] = K_TDW_SEP_BODY, sep[1] = '\0';
            k_string_split_malloc(line, sep, ' ', iFALSE, row);
        }
        
        k_data_malloc(&field, "row:", KDATA_TYPE_STRING);
        if(root -> this_node == NULL){
            link_row = k_link_insert_this_tail_malloc(root, field, KDATA_TYPE_KDATA);
        }else{
            link_row = k_link_insert_after_malloc(&link_row, field, KDATA_TYPE_KDATA);
        }
        index ++; 
        
        step1_result = *(char **)k_ARRAY_ITEM(row, 36);
        step10_result = *(char **)k_ARRAY_ITEM(row, 72);
        step13_reason = *(char **)k_ARRAY_ITEM(row, 85);
        param_peeruin = *(char **)k_ARRAY_ITEM(row, 32);
        platform = *(char **)k_ARRAY_ITEM(row, 20);
        
//
//        if(index != 1){
//            if(!(strcmp(step1_result, "1") == 0 &&
//               strcmp(step10_result, "1") == 0))
//               continue;
//        }
        
//        type[0] = '\0';
//        if(index != 1){
//            if(strcmp(step10_result, "1") == 0 && strcmp(step1_result, "1") == 0 && strcmp(step13_reason, "991246") == 0)
//                strcpy(type, "t_991246");
//            if(strcmp(step10_result, "1") == 0 && strcmp(step1_result, "1") == 0 && 
//                    (strcmp(step13_reason, "991233") == 0 || strcmp(step13_reason, "991234") == 0))
//                strcpy(type, "t_991233|234");
//            if(strcmp(step10_result, "1") == 0 && strcmp(step1_result, "1") == 0 && strcmp(step13_reason, "991235") == 0)
//                strcpy(type, "t_991235");
//        }

//        if(index != 1){
//            if(strcmp(step10_result, "1") == 0 ||
//               strlen(param_peeruin) < 1 ||
//                (
//                   strcmp(step1_result, "1") == 0 &&(
//                   strcmp(step13_reason, "991246") == 0 ||
//                   strcmp(step13_reason, "991233") == 0 ||
//                   strcmp(step13_reason, "991234") == 0 ||
//                   strcmp(step13_reason, "991235") == 0)
//                ))
//                continue;
//        }

        if(index != 1){
            if( !(
                   strcmp(step1_result, "1") == 0 &&(
                   strcmp(step13_reason, "65540") == 0
//                    ||
//                   strcmp(step13_reason, "991240") == 0 ||
//                   strcmp(step13_reason, "991746") == 0
                )))
                continue;
        }
        

//        if(index != 1){
//            if(!(strcmp(step1_result, "1") == 0 &&
//                    (
//                       strcmp(step13_reason, "991246") == 0 ||
//                       strcmp(step13_reason, "991233") == 0 ||
//                       strcmp(step13_reason, "991234") == 0 ||
//                       strcmp(step13_reason, "991235") == 0
//                    ) && 
//                    strcmp(step10_result, "1") == 0
//                ))
//                continue;
//        }
        
//        if(strlen(type) > 0 || index == 1)
//            printf("%-12s: ", type);
//        else
//            continue;
        for(i = 0; i < row -> pos; i ++){
            str = *(char **)k_ARRAY_ITEM(row, i);
            k_data_malloc(&field, str, KDATA_TYPE_STRING);
            if(link_row -> this_node == NULL){
                link_item = k_link_insert_this_tail_malloc(link_row, field, KDATA_TYPE_KDATA);
            }else{
                link_item = k_link_insert_after_malloc(&link_item, field, KDATA_TYPE_KDATA);
            }
            if(strlen(type) > 0 || index == 1 || 1){
                if(str[0] >= 0){
                    if(i == 32)
                        printf("%3d:%30.30s | ", i, str);
                    else
                        printf("%3d:%13.13s | ", i, str);
                }else{
                    printf("%3d:%13.13s | ", i, "UTF-8");
                }
            }
            
            if(i == 20)
                k_tree_insert_item_to_tree(&step_13_reason, platform, k_tree_comp_string);
            
            if(i == 32){
                if(strlen(str) >= 11 && strlen(str) < 18)
                    c_phone ++;
                if(strlen(str) < 6)
                    c_short ++;
                if(strncmp(str, "86", 2) == 0)
                    c_phone_86 ++;
                if(strcmp(str, "NULL") == 0)
                    c_null ++;
            }
            //free(str);
        }
        printf("\n");
    }
    
    k_tree_ergodic(step_13_reason, k_tree_print_single_item_string);
    
    c_phone -= c_phone_86;
    c_short -= c_null;
    printf("c_phone= %d, c_phone_86= %d, c_short = %d, c_null = %d\n", c_phone, c_phone_86, c_short, c_null);
    
    //k_link_traversal_print_handle(root, 4, 0, NULL, NULL);
    
    return 0;
}

#endif

//int k_tdw_read_query_file_header(FILE *fp, struct k_link *p_table)

int test_k_tdw_read_query_file()
{
    struct k_link *p_table;
    FILE *fp;

    freopen("log.txt", "w", stdout);
    
    fp = fopen("Calc_Step_Info_Receiver_0928.temp", "rb");
    k_tdw_read_query_file(fp, &p_table);
    fclose(fp);
    
    return 0;
}

int main()
{
    struct k_timer timer;
    
    system("pause");
    printf("size = %d + %d = %d\n", sizeof(struct k_link), sizeof(struct k_data), sizeof(struct k_link) + sizeof(struct k_data));
    k_timer_start(&timer);
    test_k_tdw_read_query_file();
    printf("time cost: %g\n", k_timer_stop(&timer)); 
    
    getchar();
}
