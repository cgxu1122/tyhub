#ifndef GLOBAL_H
#define GLOBAL_H

//@lgs
#include <QString>
#include <string>
//@zb
#define SER_LENGTH 15
#define FIFO "/tmp/usb_fifo"
#define MAX_APK_NUM (64/8)
#define USB_COUNT 12

typedef struct USB_STATE{
    int8_t num;						 //usb ser
    int8_t install_state;                 //usb state 0?????? 1????װ??2????װ???? 3????װ?ж?4????װ????ȫ
                                     //0:free  1:installing  2:install_complete  3:break off 4:install uncomplete
    char   mod[64];                  //device mod
    char   ser[SER_LENGTH];          //device ser
    int8_t apk_num;                  //apk install now
    int8_t apk_total;                //apk total num
    int8_t apk_install[MAX_APK_NUM];  //apk install state
}USB_State;



class Global
{
public:
    Global();
public:
    static QString g_IP;                     //服务器IP
    static std::string g_DevID;         //Device ID
    static bool g_NetState;             //网络状态
    static int g_UnUpNum;           //未上传文件数量
    static USB_State usb_state[USB_COUNT];
};

#endif // GLOBAL_H
