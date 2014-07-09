#include "data_sql.h"

using namespace std;

/* sqlite db for usb and jixing statistic request */
int Data_Sql::sqlinit(){
	usb_db = NULL;
	mod_db = NULL;
    log_db = NULL;

	usb_table += "usb_table";
	mod_table += "mod_table";
    log_table += "log_table";

	int usb_rc=0, mod_rc=0;
	char sql[1024]="";

	usb_rc = sqlite3_open("usb_cnt.db", &usb_db);
	mod_rc = sqlite3_open("mod_cnt.db", &mod_db);
    int log_rc = sqlite3_open("log_table.db", &log_db);
	if( usb_rc != SQLITE_OK || mod_rc != SQLITE_OK){
		//printf("db init failed!!");
		return -1;
	}

	sprintf(sql, "create table %s ( id varchar(128), date varchar(32), cnt int, primary key(id, date))", usb_table.c_str());
	if( sqlite3_exec( usb_db, sql, NULL, NULL, &err_msg) != SQLITE_OK){
		if( sqlite3_exec( usb_db, sql, NULL, NULL, &err_msg) != SQLITE_OK){
		//	printf("create usb_table failed!\n");
		}
	}
	sprintf(sql, "create table %s ( id varchar(128), date varchar(32), cnt int, primary key(id, date))", mod_table.c_str());
	if( sqlite3_exec( mod_db, sql, NULL, NULL, &err_msg) != SQLITE_OK){
		if( sqlite3_exec( mod_db, sql, NULL, NULL, &err_msg) != SQLITE_OK){
		//	printf("create mod_table failed!\n");
		}
	}

    sprintf(sql, "create table %s ( id varchar(128),  log_total int, log_upload  int)", log_table.c_str());
    if( sqlite3_exec( log_db, sql, NULL, NULL, &err_msg) != SQLITE_OK){
        if( sqlite3_exec( log_db, sql, NULL, NULL, &err_msg) != SQLITE_OK){
        //	printf("create log_table failed!\n");
        }
    }
	return 0;
}

void Data_Sql::get_unix_time( char time_str[]){
	sprintf(time_str, "%ld", time(0));
}

void Data_Sql::get_date( char date[], int date_length, long int diff){
	time_t timep=time(0);
	//struct tm *p;
	timep -= diff;
	//p = localtime(&timep);
	//p = gmtime(&timep);
    strftime( date, date_length, "%Y%m%d", localtime(&timep));
}

void Data_Sql::db_get( const string date1, const string date2, const int mod, vector<Req_Data> *res){
	res->clear();
	
	char *errmsg;
	char **dbResult;
	int index=0;
	int nColumn;
	int nRow;
	int i,j,rc;
	char sql[128];
	if ( mod == 0){
		sprintf(sql, "select * from %s where date >= '%s' and date <= '%s';", mod_table.c_str(), date1.c_str(), date2.c_str());
		printf("%s\n", sql);
		rc = sqlite3_get_table( mod_db, sql, &dbResult, &nRow, &nColumn, &errmsg);
	}else{
		sprintf(sql, "select * from %s where date >= '%s' and date <= '%s';", usb_table.c_str(), date1.c_str(), date2.c_str());
		rc = sqlite3_get_table( usb_db, sql, &dbResult, &nRow, &nColumn, &errmsg);
	}
	if ( rc == SQLITE_OK){
		//printf("ok\n");
		Req_Data tmp_data;
        index = nColumn;
        for( i = 0; i < nRow ; i++ ){		
            Req_Data iter;
			iter.id = dbResult[index++];
			iter.date = dbResult[index++];
			iter.num = atoi(dbResult[index++]);
		//	printf("%d\n", iter.num);
			res->push_back( iter);
		}
	}
}

void Data_Sql::db_log_get( const string date1, const string date2, vector<Req_Log> *res){
    vector<Req_Log>().swap( *res);
    char *errmsg;
    char **dbResult;
    int index=0;
    int nColumn;
    int nRow;
    int i,j,rc;
    char sql[128];

     sprintf(sql, "select * from %s where id >= '%s' and id <= '%s';", log_table.c_str(), date1.c_str(), date2.c_str());
      rc = sqlite3_get_table( usb_db, sql, &dbResult, &nRow, &nColumn, &errmsg);

    if ( rc == SQLITE_OK){
        //printf("ok\n");
        Req_Data tmp_data;
        index = nColumn;
        Req_Data *iter;
        for( i = 0; i < nRow ; i++ ){
            Req_Log iter;
            iter.id = dbResult[index++];
            iter.log_total = atoi(dbResult[index++]);
            iter.log_upload = atoi(dbResult[index++]);
        //	printf("%d\n", iter.num);
            res->push_back( iter);
        }
    }
}


int  Data_Sql::log_write_incr(){
    char  sql[512];
    string date;
    char today[12];
    get_date( today, 12, 0);
    if( exist_in_log_db(  log_db,  log_table, date) == 0){
        sprintf(sql, "insert into %s (id, log_total, log_upload) values ( '%s', 1, 0);", log_table.c_str(), date.c_str());
    }else{
        sprintf(sql, "update %s set log_total= log_total +1 where id='%s' and date='%s';", log_table.c_str(), date.c_str());
    }
    if( sqlite3_exec( log_db, sql, NULL, NULL, &err_msg) != SQLITE_OK){
        if( sqlite3_exec( log_db, sql, NULL, NULL, &err_msg) != SQLITE_OK){
            return -1;
        }
    }
}

int  Data_Sql::log_read_incr( const string id){
    char sql[512];
    sprintf(sql, "update %s set log_total= log_total +1 where id='%s' and date='%s';", log_table.c_str(), id.c_str());
     if( sqlite3_exec( log_db, sql, NULL, NULL, &err_msg) != SQLITE_OK){
            if( sqlite3_exec( log_db, sql, NULL, NULL, &err_msg) != SQLITE_OK){
                  return -1;
             }
      }
}



int Data_Sql::incr( const int mod, const string id){
	char date[12];
	char sql[512];
	get_date( date, 12, 0);
	if ( mod == 0){ // 0--mod jixing
		if( exist_in_db( mod_db, mod_table, date, id) == 0){
			sprintf(sql, "insert into %s (id, date, cnt) values ( '%s', '%s', 1);", mod_table.c_str(), id.c_str(), date);
		}else{
			sprintf(sql, "update %s set cnt= cnt+1 where id='%s' and date='%s';", mod_table.c_str(), id.c_str(), date);
		}
		if( sqlite3_exec( mod_db, sql, NULL, NULL, &err_msg) != SQLITE_OK){
			if( sqlite3_exec( mod_db, sql, NULL, NULL, &err_msg) != SQLITE_OK){
				return -1;
			}	
		}	
	}else if( mod == 1){
		if( exist_in_db(usb_db, usb_table, date, id) == 0){
			sprintf(sql, "insert into %s (id, date, cnt) values ( '%s', '%s', 1);", usb_table.c_str(), id.c_str(), date);
		}else{
			sprintf(sql, "update %s set cnt= cnt+1 where id='%s' and date='%s';", usb_table.c_str(), id.c_str(), date);
		}
		if( sqlite3_exec( usb_db, sql, NULL, NULL, &err_msg) != SQLITE_OK){
			if( sqlite3_exec( usb_db, sql, NULL, NULL, &err_msg) != SQLITE_OK){
				return -1;
			}	
		}	
	}
}

bool Data_Sql::exist_in_db(sqlite3 *db, const string table, const string date,  const string id )
{
	char sql_query[128]={0};
	sprintf(sql_query,"select count(*) from %s where id='%s' and date='%s'",table.c_str(), id.c_str(), date.c_str());
	
	sqlite3_stmt *pstmt;
	sqlite3_prepare(db, sql_query, strlen(sql_query), &pstmt, NULL);
	sqlite3_step(pstmt);

	int count=sqlite3_column_int(pstmt,0);
	sqlite3_finalize(pstmt);
	if(count > 0)
		return true;
	else
		return false;
}

bool Data_Sql::exist_in_log_db(sqlite3 *db, const string table,  const string id )
{
    char sql_query[128]={0};
    sprintf(sql_query,"select count(*) from %s where id='%s' '",table.c_str(), id.c_str());

    sqlite3_stmt *pstmt;
    sqlite3_prepare(db, sql_query, strlen(sql_query), &pstmt, NULL);
    sqlite3_step(pstmt);

    int count=sqlite3_column_int(pstmt,0);
    sqlite3_finalize(pstmt);
    if(count > 0)
        return true;
    else
        return false;
}

void Data_Sql::sqlclose(){
	sqlite3_close( usb_db);
	sqlite3_close( mod_db);
}

//int main(){
//	Data_Sql date_sql;
//	string id;
	
//	date_sql.sqlinit();
//	vector<Req_Data> res;
//	string date1="2014-00-11";
//	string date2="2014-12-12";
//	date_sql.db_get(date1, date2, 0, &res);
//	vector<Req_Data>::iterator iter;
//	for(iter = res.begin(); iter != res.end(); iter ++)
//		printf("%s\n", iter->id.c_str());
//	/*date_sql.incr(1, id);
//	date_sql.incr(1, id);
//	id="huawei";
//	date_sql.incr(0, id);
//	date_sql.incr(0, id);
//	date_sql.incr(0, id);
//	date_sql.sqlclose();*/
//	return 1;
//}
