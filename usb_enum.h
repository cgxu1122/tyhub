#ifndef _USB_ENUM__
#define _USB_ENUM__
#include <stdio.h>  
#include <stdlib.h> 
#include <stdint.h>
#include <unistd.h>  
#include <stdbool.h>  
#include <string.h>  
#include <pthread.h>  
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef _C_H_
#define _C_H_
#ifdef __cplusplus
extern "C" {
#endif


/*处理USB设备插入的回调函数
 * num: 从0～11，分别表示第0～11个USB插口。
 * serial：插入设备的序列号。
 * */
typedef void (* add_usb_device_callback)(int num, const char *serial);

/*处理USB设备拔出的回调函数
 * num: 被移除设备的插口位置。
 * */
typedef void (* remove_usb_device_callback)(int num);

/*向USB Monitor注册回调函数*/
extern void register_usb_device_callback(add_usb_device_callback add_cb, 
        remove_usb_device_callback rm_cb);

/* 通知USB Monitor退出。*/
extern void notify_usb_device_monitor_exit(void);

/* 启动USB Monitor。
 * NOTE：必须先注册回调函数。
 * */
extern void start_usb_device_monitor(void);

/* 安装apk，返回0表示失败，1表示成功。*/
extern bool adb_install_cmd(const char *apk_name, const char *serial);

/*
*向目标设备push 一个文件
*返回0表示失败，1表示成功
*/
extern bool adb_push_cmd(char *src, const char *dest, const char *serial);

/*获取设备的属性*/
extern char * adb_getprop_cmd(const char *prop, const char *serial);

/*启动应用*/
extern bool adb_start_app_cmd(char *packge_name, const char *serial);

#ifdef __cplusplus
}
#endif
#endif /* _C_H_ */




void call();
#endif
