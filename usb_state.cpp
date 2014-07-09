#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <stdint.h>
#include <QtDebug>
#include "global.h"
#include "usb_enum.h"
#include "sqlopt.h"
#include "data_sql.h"

USB_State usb_state[12];

void add_callback(int num,  const char *serial){
    Data_Sql data_sql;
	usb_state[num].num = num;
	char *device_mod = adb_getprop_cmd("ro.build.product", serial); //to do ????????????
	sprintf(usb_state[num].mod, "%s", device_mod);
	sprintf(usb_state[num].ser, "%s", serial);
	string apk_info[MAX_APK_NUM];
	
	usb_state[num].install_state=1;

	
	string apk_path;
	bool per_flag=true;
	bool Flag = true;
	SqlOpt sql_lib;
	sql_lib.sqlinit();
    string pkg_id;
	string mod = usb_state[num].mod;
	if( sql_lib.mob_exist( mod)){
        pkg_id += sql_lib.mob_get_pkgid( mod);
	}else{
		mod.clear();
		mod += "common";
        pkg_id += sql_lib.mob_get_pkgid( mod);
	}

	if (Flag){
        string apk_list = sql_lib.pkg_get_apk_list(pkg_id);
		int size = apk_list.size();
        int i,j, pos;
		for( i=0, j=0; i < apk_list.size(); i++, j++){
			pos = apk_list.find(";", i);
			if ( pos == -1){
				string apk_end = apk_list.substr(0, size -1);
				apk_info[j]=apk_end;
			}
			if ( pos > 0){
				string sub = apk_list.substr(i, pos-1);
				apk_info[j] = sub;
				i = pos;
			}
	
		}
		string apk_path;
		int counter;
		int run;
        string active;
        string packageid;
		for ( i = 0; apk_info[i] != ""; i++){
			apk_path.clear();
			apk_path += APKS_PATH ;

			apk_path += apk_info[i];
			per_flag = adb_install_cmd( apk_path.c_str(), serial); //*******************//
			for( int k = 0; k<2 && per_flag == false; k++){
                packageid = sql_lib.apk_get_path(apk_info[i], &counter, &run);
                apk_path += packageid + "/" + apk_info[i] + ".apk";
				per_flag = adb_install_cmd( apk_path.c_str(), serial);
				if( counter == 1){
                     active = sql_lib.apk_get_packagePath( apk_info[i]);
                    char act[256];
                    sprintf(act, "%s", active.c_str());
                    adb_start_app_cmd( act, serial); //**************************//
				}
			}
			if( per_flag == true){
				int p_num = i / 8;
				int p_pos = i % 8;
				usb_state[num].apk_install[p_num] + (int)pow(2.0, (7.0 - p_pos));
			}
			Flag = Flag && per_flag;
		}
	}
	if ( Flag){
		usb_state[num].install_state=3;
		/*send message*/

        string usb_num;
        char usb_num_tmp[8];
        sprintf( usb_num_tmp,"%d", num);
        usb_num +=  usb_num_tmp;

        data_sql.incr( 1, usb_num);
        data_sql.incr(0, device_mod);

		int fd;
		char msg[256];
		char fdname[32];
		sprintf(fdname, "%s%d", FIFO, num);
        string batchCode = sql_lib.pkg_get_batchCode(pkg_id);
        /*
        imei：手机imei
        ua：手机ua信息
        渠道id：加工设备传递给手机
        加工设备编码：加工设备传递给手机
        批次号：安装包的批次号，加工设备传递给手机
        手机加工时间：设备加工手机的时间戳
        */
        string cid = sql_lib.dev_get_cid();
        char unixtime[32];
        sql_lib.get_unix_time( unixtime);
        sprintf(msg,"%s,%s,%s,%s,%s,%s", serial, device_mod, cid.c_str(), Global::g_DevID.c_str(), batchCode.c_str(), unixtime);/***************/
		string send_msg = msg;
		fd=open(fdname, O_WRONLY|O_NONBLOCK,0);
		if(fd=open(fdname, O_WRONLY|O_NONBLOCK,0) == -1 ){
			fd = open( fdname, O_WRONLY|O_NONBLOCK,0);
		}
		if(write(fd, send_msg.c_str(), send_msg.size()) == -1){
			write(fd, send_msg.c_str(), send_msg.size());
		}
	}else{
		usb_state[num].install_state=4;
	}
    sql_lib.sqlclose();
    data_sql.sqlclose();
}

void remove_callback( int num){
	if( usb_state[num].install_state == 1)
		usb_state[num].install_state = 3;
	else
		usb_state[num].install_state = 0;
}

/*
void add_callback(int num, const char *serial)
{
    printf("Add USB num: %d, Serial: %s\n", num, serial);

    // Place Your CODE here. 
}

void remove_callback(int num)
{
    printf("Remove USB num: %d\n", num);
    // Place Your CODE here. 
}
*/

void call()
{
    register_usb_device_callback(add_callback, remove_callback);
    start_usb_device_monitor();
//	char adb_pus_1[] = "/home/xianfeng/usb_enum/net_link.txt";
//	char adb_pus_2[] = "/sd/net_link.txt";
//	char adb_pus_3[] = "0123456789ABCDEF";
   // printf(" adb_push_cmd  = %d\n", adb_push_cmd( adb_pus_1, adb_pus_2, adb_pus_3));
   // printf(" getprop = %s\n", adb_getprop_cmd("ro.build.product","0123456789ABCDEF"));
//	char adb_install_1[] = "com.android.browser/com.android.browser.BrowserActivitya";
//	char adb_install_2[] = "0123456789ABCDEF";
  //  printf("%d\n", adb_install_cmd("/home/xianfeng/Downloads/a.apk","0123456789ABCDEF"));

    //start com.android.browser/com.android.browser.BrowserActivity
//	char activ_1[] = "com.android.browser/com.android.browser.BrowserActivitya";
//	char activ_2[] = "0123456789ABCDEF";
//    qDebug()<<"test for void call!\n";
    //printf("start app %d \n", adb_start_app_cmd( activ_1, activ_2));
    //return 0;
}

