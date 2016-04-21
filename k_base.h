/**
    name      : k_base.h
    date      ��2015.09.22
    describe  : �ײ�֧��ͷ�ļ� 
    copyright : ���������ʹ����, ���ǲ����ڲ�����汾�ŵ�����¶��������޸�
    updata    : 
        1) �������������͵Ķ���.
        2) �����˽ṹ�� struct k_pair {};
        3) ���� stdint.h 
*/

#include <stdint.h>

#ifndef __K_BASE_H__
    #define __K_BASE_H__

/* JPEG_SUPPORT ���������Ƿ���� JPEG ���� */
#ifndef JPEG_SUPPORT
    #define JPEG_SUPPORT_NO 
#endif

/* �����������͵��ض���ԭ����Ҫ����:
   1). �����ļ���������������ݳ���(����������)Ҫ���ϸ�.
   2). ����һЩ�������, �����ϣ�� int ������ 32 λ��, �����ֲ���ʹ�� long . 
   3). 
   
   ǰ׺ i ��Ϊ�˼��� ���������еĶ����ͻ.
 */

#define DATA_TYPE_REDEFINE_SUPPORT
#define LITTILE_ENDIAN_SUPPORT       /* ��С�˱����ͨ������������ */

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
    
    /* ��Ŀǰ�����б�������, float Ϊ 32λ, double Ϊ 64 λ */
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
