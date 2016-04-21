#include "k_string.h"


int test_k_string_split_malloc()
{
    int i;
    struct k_array *result;
    
    k_array_malloc(&result, sizeof(char *), 64);        /* 初始化为 64 个元素 */
    char *line = " end";
    k_string_split_malloc(line, " ", ' ', iTRUE, result);
    printf("src : <%s>\n", line);
    printf("res : \n");
    for(i = 0; i < result -> pos; i ++){
        printf("<%s> | ", *(char **)k_ARRAY_ITEM(result, i));
    }
    printf("\n");
    return 0; 
}

int test_k_string_split_get_index_malloc()
{
    int i, start, end;
    struct k_array *result;
    char buf[4096];
    
    k_array_malloc(&result, sizeof(int), 64);        /* 初始化为 64 个元素 */
    char *line = "123  |||||  67  |  7890 |  2346 |";
    k_string_split_get_index_malloc(line, "|", iTRUE, result);
    
    for(i = 0; i < result -> pos; i += 2){
        start = *(int *)k_ARRAY_ITEM(result, i);
        end = *(int *)k_ARRAY_ITEM(result, i + 1);
        strncpy(buf, line + start, end - start);
        buf[end - start] = '\0';
        printf("%s ", buf);
    }
    printf("\n");
    return 0; 
}

int test_k_string_token_replace()
{
    char str[1024];
    strcpy(str, "(strcat 'hello' ', world')");
    printf("src : %s\n", str);
    k_string_token_replace(str, '\'', '\'', ' ', '_', iTRUE);
    printf("dest: %s\n", str);
    return 0;
}

int test_k_string_cut_pattern()
{
    char str[1024];
    strcpy(str, "  \r   hello, world! | \t \n");
    printf("src : |%s|\n", str);
    k_string_cut_pattern(str, K_STRING_CUT_ALL, isspace);
    printf("dest: |%s|\n", str);
    return 0;
}

int main()
{
    test_k_string_split_malloc();
    test_k_string_token_replace();
    test_k_string_split_get_index_malloc();
    test_k_string_cut_pattern();
    getchar();
}
