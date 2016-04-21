#include "k_data.h"

int main()
{
    char *s1 = "hehe", *s2 = "heihei", *s3 = "GG";
    iINT32 int_4 = 123;
	struct k_data *d1, *d2, *d3, *d4;
	char *str;
	iINT32 *p_int_4;
    
    k_data_malloc(&d1, s1, KDATA_TYPE_STRING);
    k_data_malloc(&d2, s2, KDATA_TYPE_STRING);
    k_data_malloc(&d3, s3, KDATA_TYPE_STRING);
    k_data_malloc(&d4, &int_4, KDATA_TYPE_INT32);
    
    str = k_data_convert_to_string_malloc(d1);
    printf("data = %s\n", str);
    str = k_data_convert_to_string_malloc(d2);
    printf("data = %s\n", str);
    str = k_data_convert_to_string_malloc(d3);
    printf("data = %s\n", str);
    printf("type = %d\n", (d4)->type);
    
//    p_int_4 = switch(d4->type)
//    {
//        case KDATA_TYPE_STRING:
//            *(iINT8 **)d4->p_data;
//            break;
//        case KDATA_TYPE_INT32:
//            *(iINT32 *)d4->p_data;
//            break;
//    }
    /* ( ? : ) 结合性从右至左 */ 
//    int_4 = d4->type == 1 ? *(iINT8 **)d4->p_data : d4->type == 4 ? *(iINT32 *)d4->p_data : (my_error(fp_error, MODE_ERROR, "<k_data:macro> :support type!"), 0);
//    p_int_4 = (d4 -> type == 1) ? (iINT8 **)d4->p_data : d4->type == 4 ? (iINT32 *)d4->p_data : (my_error(fp_error, MODE_ERROR, "<k_data:macro> :support type!"), NULL);
//    printf("data = %d\n", int_4);
//    printf("pdata = %d\n", p_int_4);
    p_int_4 = KDATA_P_KDATA(d4);
    //int_4 = (int)KDATA_GET_KDATA(d4);
    printf("pdata = %d\n", *p_int_4);
    //*(KDATA_P_KDATA(d4));
    //printf("data = %d\n", KDATA_GET_KDATA(d4));
    
    //( (d4)->type == 1 ? ( (iINT8 **)(((d4)->p_data)) ) : ( (d4)->type == 2 ? ( (iINT8 *)((d4)->p_data) ) : ( (d4)->type == 4 ? ( (iINT32 *)((d4)->p_data) ) : ( (d4)->type == 6 ? ( (iINT64 *)((d4)->p_data) ) : ( (d4)->type == 8 ? ( (iFLOAT32 *)((d4)->p_data) ) : ( (d4)->type == 9 ? ( (iFLOAT64 *)((d4)->p_data) ) : ( ((void *)0) )))))) );

    //KDATA_P_KDATA(d4);
    //printf("data = %d\n", *(KDATA_P_KDATA(d4)));
    system("pause");
}
