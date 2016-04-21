/**
    name      : k_base.h
    date      ：2015.09.22
    describe  : 底层支持头文件 
    copyright : 你可以自由使用它, 但是不能在不变更版本号的情况下对其作出修改
    updata    : 
        1) 增加了数据类型的定义.
        2) 增加了结构体 struct k_pair {};
        3) 引入 stdint.h 
*/

#include <stdint.h>

#ifndef __K_BASE_H__
    #define __K_BASE_H__

/* JPEG_SUPPORT 用来控制是否编译 JPEG 部分 */
#ifndef JPEG_SUPPORT
    #define JPEG_SUPPORT_NO 
#endif

/* 对于数据类型的重定义原因主要如下:
   1). 物理文件或者网络对于数据长度(尤其是整数)要求严格.
   2). 对于一些特殊情况, 你可能希望 int 必须是 32 位的, 但是又不想使用 long . 
   3). 
   
   前缀 i 是为了减少 可能与已有的定义冲突.
 */

#define DATA_TYPE_REDEFINE_SUPPORT
#define LITTILE_ENDIAN_SUPPORT       /* 大小端编码可通过编译器设置 */

//typedef signed char int8_t;
//typedef unsigned char   uint8_t;
//typedef short  int16_t;
//typedef unsigned short  uint16_t;
//typedef int  int32_t;
//typedef unsigned   uint32_t;
//typedef long long  int64_t;
//typedef unsigned long long   uint64_t;

#ifdef DATA_TYPE_REDEFINE_SUPPORT

    typedef int16_t          iINT16;
    typedef uint16_t         iUINT16;
    
    typedef int32_t          iINT32;
    typedef uint32_t         iUINT32;
    
    typedef int64_t          iINT64;
    typedef uint64_t         iUINT64;

    typedef int8_t           iINT8;
    typedef uint8_t          iUINT8;
    
    /* 在目前的所有编译器中, float 为 32位, double 为 64 位 */
    typedef float            iFLOAT32;
    typedef double           iFLOAT64;

    /* Bool type start */
    #ifndef iFALSE
        #define              iFALSE   0
    #endif
    
    #ifndef iTRUE
        #define              iTRUE    1
    #endif
    
    #ifndef iUNKNOWN
        #define              iUNKNOWN 2
    #endif
    /* Bool type end */
    
#endif

#define K_CMP_BIGGER     1
#define K_CMP_EQUAL      0
#define K_CMP_SMALLER   -1
#define K_CMP_UNKNOWN   -2

struct point{
    int x;
    int y;
};

//struct Rect{
//    struct point p0;
//    struct point p1;
//};

struct k_pair{
    void *key;
    void *value;
};


#endif
