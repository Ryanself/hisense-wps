#include <stdio.h>

/*
 * Function: CallBackFunc4NetWorkConnectState
 * Pram:
 * State:0,网络连通,1,网络断开
 * Return:无
 * Description:设备联网状态变化时,sdk 通过此接口进行通知
 * */
typedef void (* CallBackFunc4NetWorkConnectState)(int state);

/*
 * Function: CallBackFunc4Wps
 * Pram:
 * State:0,wps 启动,1,wps 结束
 * Return:无
 * Description:Wps 状态变化通知,wps 过程启动或者结束
 * */
typedef void (* CallBackFunc4Wps)(int state);

/*Function: registerCallBackFunc
 * Pram:
 * deviceOnofflineFunc:设备上下线回调函数
 * wpsFunc:wps 回调函数
 * Return:无
 * Description:回调函数注册接口
 * */
int registerCallBackFunc(CallBackFunc4NetWorkConnectState
			networkConnectState, CallBackFunc4Wps wpsFunc);

/*
 * Function:startWps
 * Pram:
 * seconds,wps 持续时间
 * Return:0,成功,1,失败
 * Description:启动 wps
 * */
int startWps(int seconds); //fatww (but it doesnot support seconds control for now.)

/*
 * Function:stopWps
 * Pram:无Return:0,成功,1,失败
 * Description:结束 wps
 * */
int stopWps();// call of wps_stop

/*
 * Function: updateWiFiInfo
 * Pram:
 * ssid:WiFi SSID
 * password:WiFi 密码
 * key_id:  enc
 * Return:0,成功,1,失败
 * Description:更新 WiFi 信息
 * */
int updateWiFiInfo(char *ssid, char *password, int key_id);// wifi_conf.c//restore_wifi_info_to_flash
