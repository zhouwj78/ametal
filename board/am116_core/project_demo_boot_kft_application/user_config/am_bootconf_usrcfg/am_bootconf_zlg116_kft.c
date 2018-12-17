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
 * \brief ZLG116 kboot KinetisFlashTool 用户配置文件
 *
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 18-12-13  yrh, first implementation
 * \endinternal
 */


/**
 * \addtogroup am_if_src_bootconf_zlg116_kft
 * \copydoc am_bootconf_zlg116_kft.c
 * @{
 */
#include <am_boot_kft.h>
#include "ametal.h"
#include "am_gpio.h"
#include "am_zlg116.h"
#include "am_zlg116_inst_init.h"
#include "am_zlg116_boot_serial_uart.h"
#include "am_zlg116_boot_flash.h"
#include "am_zlg116_boot.h"
#include "am_boot.h"
#include "am_boot_autobaud.h"
#include "am_boot_flash.h"
#include "am_zlg_tim_cap.h"
#include "am_zlg_flash.h"
#include "zlg116_regbase.h"
/**
 * \name 自动波特率检测宏定义
 * @{
 */

/** \brief 需要用到的定时器位数 */
#define     TIMER_WIDTH           16

/** @} */

/*******************************************************************************
 * 自动波特率检测配置
 ******************************************************************************/

/** \brief 自动波特率的平台初始化 */
void __zlg116_plfm_autobaud_init (void)
{
    amhw_zlg_tim_prescale_set((amhw_zlg_tim_t *)ZLG116_TIM1_BASE, (uint16_t)8);
}

/** \brief 自动波特率的平台解初始化  */
void __zlg116_plfm_autobaud_deinit (void)
{

}

/** \brief 平台捕获定时器溢出标志获取  */
int __fsl_plfm_stat_flag_get (void *parg)
{
    return AM_OK;
}

/** \brief 自动波特率的设备信息实例 */
static am_boot_autobaud_devinfo_t  __g_zlg116_boot_autobaud_devinfo = {

    2,                            /**< \brief CAP捕获通道号 */
    TIMER_WIDTH,                  /**< \brief TIMER定时器位数 */
    10,                           /**< \brief 接收一次数据的超时时间(ms)*/
    AM_CAP_TRIGGER_RISE,          /**< \brief CAP捕获触发方式 */
    __zlg116_plfm_autobaud_init,  /**< \brief 平台初始化函数 */
    __zlg116_plfm_autobaud_deinit,/**< \brief 平台解初始化函数 */

    __fsl_plfm_stat_flag_get,
};

/** \brief 自动波特率功能的设备实例 */
am_boot_autobaud_dev_t  __g_zlg116_boot_autobaud_dev;

/** \brief 实例初始化，获得自动波特率服务句柄 */
am_boot_autobaud_handle_t am_zlg116_boot_autobaud_inst_init (void)
{
    am_cap_handle_t   cap_handle   = am_zlg116_tim1_cap_inst_init();

    am_zlg_tim_cap_dev_t *p_cap_dev = (am_zlg_tim_cap_dev_t *)cap_handle;

    __g_zlg116_boot_autobaud_dev.cap_pin =
            p_cap_dev->p_devinfo->p_ioinfo[__g_zlg116_boot_autobaud_devinfo.cap_chanel].gpio;

    return am_boot_autobaud_init(&__g_zlg116_boot_autobaud_dev,
                                 &__g_zlg116_boot_autobaud_devinfo,
                                  cap_handle);
}

/** \brief 自动波特率实例解初始化 */
void am_zlg116_boot_autobaud_inst_deinit (am_boot_autobaud_handle_t handle)
{
    am_zlg116_tim1_cap_inst_deinit(handle->cap_handle);
    am_boot_autobaud_deinit(handle);
}


/*******************************************************************************
 * flash配置
 ******************************************************************************/
/**
 * < \brief bootloader flash 设备信息
 */
static am_zlg116_boot_flash_devinfo_t __g_flash_devinfo = {
    /** \brief flash的起始地址 */
    0x08000000,
    /** \brief flash的总的大小 */
    64 * 1024,
    /** \brief flash扇区大小 */
    1024,
    /** \brief flash扇区数 */
    64,
    /** \brief flash寄存器的基地址 */
    ZLG116_FLASH_BASE,
    /** \brief 平台初始化函数 */
    NULL,
    /** \brief 平台初解始化函数 */
    NULL,
};

/**
 * < \brief bootloader flash 设备实例
 */
static am_zlg116_boot_flash_dev_t __g_flash_dev;

/**
 * \brief bootloader flash实例初始化，获得其标准服务句柄
 *
 * \param 无
 *
 * \return bootloader flash标准服务句柄，若为 NULL，表明初始化失败
 */
am_boot_flash_handle_t am_zlg116_boot_flash_inst_init()
{
    return am_zlg116_boot_flash_init(&__g_flash_dev, &__g_flash_devinfo);
}



/*******************************************************************************
 * 标准bootloader接口配置
 ******************************************************************************/
/**
 * < \brief bootloader标准设备信息
 */
static am_zlg116_boot_devinfo_t __g_zlg116_boot_devinfo = {
    /** \brief flash起始地址*/
    0x08000000,
    /** \brief flash大小 */
    64 * 1024,
    /** \brief ram起始地址 */
    0x20000000,
    /** \brief ram结束地址 */
    8 * 1024,
    /** \brief 平台初始化函数 */
    NULL,
    /** \brief 平台解初始化函数 */
    NULL,
};

/**
 * < \brief bootloader标准设备实例
 */
static am_zlg116_boot_dev_t __g_zlg116_boot_dev;

int am_zlg116_std_boot_inst_init()
{
    return am_zlg116_boot_init(&__g_zlg116_boot_dev, &__g_zlg116_boot_devinfo);
}

/*******************************************************************************
 * kboot KinetisFlashTool 配置
 ******************************************************************************/

/**
 * < \brief kboot KinetisFlashTool 设备信息
 */
static am_boot_kft_dev_info_t  __g_zlg116_boot_kft_dev_info = {
    0X08007000,
    NULL,
    NULL,
};

/**
 * < \brief kboot KinetisFlashTool 设备实例
 */
static am_boot_kft_dev_t __g_zlg116_boot_kft_dev;
/**
 * \brief bootloader kboot KinetisFlashTool 实例初始化
 *
 * \return 若为 AM_OK，表明初始化成功
 */
int am_zlg116_boot_kft_inst_init()
{

    am_boot_flash_handle_t flash_handle = am_zlg116_boot_flash_inst_init();

    am_boot_autobaud_handle_t autobaud_handle = am_zlg116_boot_autobaud_inst_init();

    am_boot_serial_handle_t serial_handle  = am_zlg116_boot_serial_uart_init(autobaud_handle);

    return am_boot_kft_init(&__g_zlg116_boot_kft_dev,
                            &__g_zlg116_boot_kft_dev_info,
                             flash_handle,
                             serial_handle);
}

/** @} */

/* end of file */
