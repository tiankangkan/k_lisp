/**
    此文件提供了一个错误处理函数 
    Version : 2013.04.05 
*/
#ifndef __K_ERROR_H__
    #define __K_ERROR_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define ERROR_log  "error_log.txt"
#define IF_print   1
#define IF_log     2
#define IF_pause   4
#define IF_return  8
#define IF_exit    16
#define MODE_WARING     IF_log
#define MODE_ERROR      (IF_print | IF_log | IF_pause | IF_return)

FILE *fp_error;    /* 全局变量 : 文件指针指向错误日志*/ 
/* 错误处理函数, TODO: 增加对错误码的处理 */ 
int my_error(FILE *fp, int mode, const char *fmt, ...);


int my_error(FILE *fp, int mode, const char *fmt, ...)
{
     char buf_1[2048];
     char buf_2[2048];
     buf_1[0] = buf_2[0] = '\0';
     va_list args;
     va_start(args, fmt);
     vsprintf(buf_2, fmt, args);
     strcat(buf_1, "\n---------------------------------------------- error_start ---\n");
     strcat(buf_1, buf_2);
     strcat(buf_1, "\n---------------------------------------------- error_end   ---\n");
     if((mode & IF_print) != 0)
         printf(buf_1);
     if((mode & IF_log) != 0){
         if(fp == NULL){
             fp_error = fopen(ERROR_log, "w");
             fprintf(fp_error, buf_1);
             fflush(fp_error);
         }else{
             fprintf(fp, buf_1);
             fflush(fp);
         }
     }
     if((mode & IF_pause) != 0){
         ;
     }
     if((mode & IF_return) != 0 && (mode & IF_exit) == 0)
         return -1;
     if((mode & IF_exit) != 0)
         exit(-1);
     return 0;
}

#endif
