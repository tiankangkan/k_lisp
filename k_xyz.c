/**
@File      : k_xyz.h
@Author    : kangtian
@Date      : 15-09-28
@Describle : 关系描述抽象接口.
             xyz 的名字并不是瞎取的, 它代表某种抽象性.
*/
#ifndef __K_XYZ_H__
    #define __K_XYZ_H__

#include "k_error.h"
#include "k_link.h"

#define K_XYZ_TYPE_LINK    1

/* 释放节点 */
void *k_xyz_creat_node(void **node, void *parameter, int type); 
/* 释放节点 */
void *k_xyz_free_node(void **node, void *parameter, int type); 
/* 得到子节点 */
void *k_xyz_get_sub_node(void *node, void *parameter, int type);
/* 设置为子节点 */
void *k_xyz_set_sub_node(void *node, void *node_sub, void *parameter, int type);
/* 得到父节点 */
void *k_xyz_get_parent_node(void *node, void *parameter, int type);
/* 设置为父节点 */
void *k_xyz_set_parent_node(void *node, void *node_parent, void *parameter, int type);

/*****************************************************/

#endif
