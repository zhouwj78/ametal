/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief bootloader 通用基本标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 18-11-13  yrh, first implementation.
 * \endinternal
 */

#ifndef __AM_BOOT_FLASH_H
#define __AM_BOOT_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_boot.h"
typedef struct am_boot_flash_info {
    uint32_t  flash_start_addr;
    uint32_t  flash_size;
    uint32_t  flash_sector_size;
    uint32_t  flash_sector_count;
}am_boot_flash_info_t;

struct am_boot_flash_drv_funcs {
    int  (*pfn_flash_erase_region) (void     *p_drv,
                                    uint32_t  start_addr,
                                    uint32_t  length);
    int  (*pfn_flash_program) (void      *p_drv,
                               uint32_t   dst_addr,
                               uint8_t  *p_src,
                               uint32_t   size);
    void (*pfn_flash_info_get) (void     *p_drv,am_boot_flash_info_t  **p_info );
    int  (*pfn_flash_erase_all)(void     *p_drv);
};

typedef struct am_boot_flash_serv {
    struct am_boot_flash_drv_funcs *p_funcs;
    void                           *p_drv;
}am_boot_flash_serv_t;

typedef am_boot_flash_serv_t *am_boot_flash_handle_t;

/**
 * \brief flash区域擦除
 *
 * \param[in] start_addr : 起始地址
 * \param[in] length     : 擦除长度
 *
 * \retval AM_OK    擦除成功
 * \retval AM_ERROR 传入的参数有误
 * \retval >0       出错扇区号
 */
am_static_inline
int am_boot_flash_erase_region(am_boot_flash_handle_t handle,
                               uint32_t               start_addr,
                               uint32_t               length)
{
    return handle->p_funcs->pfn_flash_erase_region(handle->p_drv,
                                                   start_addr,
                                                   length);
}
/**
 * \brief flash编程
 *
 * \param[in] dst_addr : 目标地址
 * \param[in] p_src    : 数据源地址
 * \param[in] size     : 写入的数据的大小(数据的大小要为4的整数倍)
 *
 * \retval AM_OK    写入成功
 * \retval AM_ERROR 传入的参数有误
 * \retval >0       出错扇区号
 */

am_static_inline
int am_boot_flash_program(am_boot_flash_handle_t handle,
                          uint32_t               dst_addr,
                          uint8_t               *p_src,
                          uint32_t               size)
{
    return handle->p_funcs->pfn_flash_program(handle->p_drv,
                                              dst_addr,
                                              p_src,
                                              size);
}

/**
 * \brief 获取flash的信息
 * \param[in] p_info : 存放获取后的信息

 * \retval 无
 */
am_static_inline
void am_boot_flash_info_get(am_boot_flash_handle_t  handle,
                            am_boot_flash_info_t  **p_info)
{
    if(handle != NULL &&
       handle->p_funcs != NULL &&
       handle->p_funcs->pfn_flash_info_get != NULL) {
        return handle->p_funcs->pfn_flash_info_get(handle->p_drv, p_info);
    }
    return;
}

am_static_inline
int am_boot_flash_erase_all(am_boot_flash_handle_t handle)
{
    if(handle != NULL &&
       handle->p_funcs != NULL &&
       handle->p_funcs->pfn_flash_erase_all != NULL) {
        handle->p_funcs->pfn_flash_erase_all(handle->p_drv);
    }
    return -AM_EINVAL;
}

#ifdef __cplusplus
}
#endif

#endif /* __AM_BOOT_FLASH_H */

/* end of file */
