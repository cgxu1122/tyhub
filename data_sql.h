#ifndef DATA_SQL_H__
#define DATA_SQL_H__
#include<stdio.h>
#include<stdlib.h>
#include<sqlite3.h>
#include<string>
#include<string.h>
#include<iostream>
#include<time.h>
#include<vector>

using namespace std;

typedef struct  REQDATA{
	string id;
	string date;
	int    num;
}Req_Data;

typedef struct  REQLOG{
    string id;
    int   log_total;
    int   log_upload;
}Req_Log;

using namespace std;

class Data_Sql{
	public:
		int sqlinit();
		void get_unix_time( char time_str[]);
		void get_date( char date[], int date_length, long int diff);
		void db_get ( const string date1, const string data2, const int mod, vector<Req_Data> *res);
		int  db_incr( const int mod, const string id);
		int  db_opt_incr( sqlite3* db, const string table, const string id);
		int  incr(const int mod, const string id);
		bool exist_in_db( sqlite3 *db, const string table,  const string id, const string date);
        bool exist_in_log_db(sqlite3 *db, const string table,  const string id );
        void db_log_get( const string id, const string id2, vector<Req_Log> *res);
        int  log_write_incr();
        int  log_read_incr(const string id);
		void sqlclose();
	private:
		sqlite3 *usb_db;
		sqlite3 *mod_db;
        sqlite3 *log_db;
		string usb_table;
		string mod_table;
        string log_table;
		char *err_msg;
};

int print_record( void *params, int n_column, char **conlumn_value, char **column_name);
#endif
