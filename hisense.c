/*#include "FreeRTOS.h"
#include <lwip_netconf.h>
#include <lwip/netif.h>
#include <string.h>
#if CONFIG_WLAN
#include <wifi/wifi_conf.h>
#include <wifi/wifi_util.h>
#include <wifi/wifi_ind.h>
#endif
#include "wps/wps_defs.h"
#include "hisense.h"
*/

#include "hisense.h"
extern void wps_stop();
extern void cmd_wps(int argc, char **argv);

static CallBackFunc4NetWorkConnectState network_connect_state = NULL;
static CallBackFunc4Wps wps_func = NULL;

static rtw_network_info_t wifi = {
	{0},    // ssid
	{0},    // bssid
	0,      // security
	NULL,   // password
	0,      // password len
	-1      // key id
};

static void init_wifi_struct(void)
{
	memset(wifi.ssid.val, 0, sizeof(wifi.ssid.val));
	memset(wifi.bssid.octet, 0, ETH_ALEN);
	memset(password, 0, sizeof(password));
	wifi.ssid.len = 0;
	wifi.password = NULL;
	wifi.password_len = 0;
	wifi.key_id = -1;
}

int registerCallBackFunc(CallBackFunc4NetWorkConnectState
			networkConnectState, CallBackFunc4Wps wpsFunc)
{
	if (networkConnectState)
	      network_connect_state = networkConnectState;
	if (wpsFunc)
	      wps_func = wpsFunc;
	return 0;
}

int startWps(int seconds)
{
	int argc = 0;
	char *argv[4];
	argv[argc++] = "wifi_wps";
	argv[argc++] = "pbc";
	sprintf(argv[argc], "%d", seconds);
	argc++;
	cmd_wps(argc, argv);
	return 0;
}

int stopWps()
{
	wps_stop();
}

// use same struct will be benifit for extension.
int updateWiFiInfo(char *ssid, char *password, int key_id)
{
	int ret = RTW_SUCCESS;
	init_wifi_struct();
	if(!ssid){
		printf("SSID(Maximum length is 32)can not be null\n\r");
		ret = RTW_BADARG;
		goto exit;
	}

	if(strlen(ssid) > 32){
		printf("Error: SSID length can't exceed 32\n\r");
		ret = RTW_BADARG;
		goto exit;
	}

	if(key_id < 0 || key_id > 3){
		printf("Error: Wrong WEP key id. Must be one of 0,1,2, or 3.\n\r");
		ret = RTW_BADARG;
		goto exit;
	}
	if(password){
		wifi.password = password;
		wifi.password_len = strlen(password);
		if((wifi.key_id >= 0)&&(wifi.key_id <= 3)) {
			wifi.security_type = RTW_SECURITY_WEP_PSK;
		}
		else{
			wifi.security_type = RTW_SECURITY_WPA2_AES_PSK;
		}
	}
	else{
		wifi.security_type = RTW_SECURITY_OPEN;
	}

	wifi.ssid.val = ssid;
	wifi.ssid.len = strlen(ssid);
	wifi.key_id = key_id;

	//restore_wifi_info_to_flash will be called in wifi_connect to save info.
	ret = wifi_connect((char*)wifi.ssid.val, wifi.security_type, (char*)wifi.password,
				wifi.ssid.len, wifi.password_len, wifi.key_id, NULL);
	if(ret!= RTW_SUCCESS){
		if(ret == RTW_INVALID_KEY)
			printf("\n\rERROR:Invalid Key ");
		goto exit;
	}
	//tick2 = xTaskGetTickCount();
	//printf("\r\nConnected after %dms.\n", (tick2-tick1));
#if CONFIG_LWIP_LAYER
		LwIP_DHCP(0, DHCP_START);
	//tick3 = xTaskGetTickCount();
	//printf("\r\n\nGot IP after %dms.\n", (tick3-tick1));
#endif
	//printf("\n\r");
exit:
	init_wifi_struct();
	return ret;
}
