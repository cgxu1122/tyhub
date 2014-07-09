#ifndef  RECORD__H___
#define RECORD__H___
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<string.h>
#include<unistd.h>
#include <QFile>
#include<time.h>
#include <qstring.h>
#include<iostream>
#include<fstream>
#include"data_sql.h"


#define MAX_LINES 100
#define DIR_PATH "/home/zb/DATA/"
#define TIME_DIR_PATH "/home/zb/DATA/log/"
#define DATE_LENGTH 12
#define TIME_LENGTH 24
#define MAX_FILE_NUM 300
#define FIFO "/tmp/usb_fifo"

using namespace std;

class FILE_CTL{
	public: 
		int file_write( const string info);
		int file_lines_control();
		int file_control();
		int file_delete();
		int file_delete(char *date);
		int get_file_lines( const char* path);
		int file_delete(char *date1, char* date2);
        void get_unix_time( char time_str[]);
		int  init();
		void log_deal();
		void get_time(char ntime[]);
		void get_today(char today[]);
		void file_date_control();
        void mkdir(QString path);
        void mk_filedir(QString path);
		int file_close();
	public:
		ofstream date_f;
		ofstream time_f;
        Data_Sql  *data_sql;
		int  fd[12];
		int  cnt;
		int  flag;
		int  FILE_NUM;
		char file_date[DATE_LENGTH];
};
#endif
