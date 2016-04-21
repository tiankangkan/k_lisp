/**
@File      : k_xyz.h
@Author    : kangtian
@Date      : 15-09-28
@Describle : ��ϵ��������ӿ�.
             xyz �����ֲ�����Ϲȡ��, ������ĳ�ֳ�����.
*/
#ifndef __K_XYZ_H__
    #define __K_XYZ_H__

#include "k_error.h"
#include "k_link.h"

#define K_XYZ_TYPE_LINK    1

/* �ͷŽڵ� */
void *k_xyz_creat_node(void **node, void *parameter, int type); 
/* �ͷŽڵ� */
void *k_xyz_free_node(void **node, void *parameter, int type); 
/* �õ��ӽڵ� */
void *k_xyz_get_sub_node(void *node, void *parameter, int type);
/* ����Ϊ�ӽڵ� */
void *k_xyz_set_sub_node(void *node, void *node_sub, void *parameter, int type);
/* �õ����ڵ� */
void *k_xyz_get_parent_node(void *node, void *parameter, int type);
/* ����Ϊ���ڵ� */
void *k_xyz_set_parent_node(void *node, void *node_parent, void *parameter, int type);

/*****************************************************/

#endif
