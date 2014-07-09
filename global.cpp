#include "global.h"

Global::Global()
{
}
QString Global::g_IP = 0;
std::string Global::g_DevID = "";
bool  Global::g_NetState = false;
int Global::g_UnUpNum = 0;
USB_State Global::usb_state[USB_COUNT] = {0};
