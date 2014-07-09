#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include "sqlopt.h"

int SqlOpt::sqlinit(){
    dev_db = NULL;
    apk_db = NULL;
    pkg_db = NULL;
    mob_db = NULL;

    dev_table += "dev_table";
    apk_table += "apk_table";
    pkg_table += "pkg_table";
    mob_table += "mob_table";

    int dev_rc=0, apk_rc=0, pkg_rc=0, mob_rc=0;
    char sql[1024]="";

    mob_rc = sqlite3_open("sqldb_mob.db", &mob_db);
    sprintf(sql, "CREATE TABLE %s ( id varchar(128) PRIMARY KEY, info varchar(128));", mob_table.c_str());
    if(sqlite3_exec(mob_db, sql, NULL, NULL, &err_msg)!= SQLITE_OK){
    }else{
        sqlite3_exec(mob_db, sql, NULL, NULL, &err_msg);
    }
    apk_rc = sqlite3_open("sqldb_apk.db", &apk_db);
    sprintf(sql, "CREATE TABLE %s ( id varchar(128) PRIMARY KEY, pkgname varchar(128), counter int, icon int, run int, md5 varchar(32), packagePath  varchar(256));", apk_table.c_str());
    if(sqlite3_exec(apk_db, sql, NULL, NULL, &err_msg)!= SQLITE_OK){
    }else{
        sqlite3_exec(apk_db, sql, NULL, NULL, &err_msg);
    }

    pkg_rc = sqlite3_open("sqldb_pkg.db", &pkg_db);

    sprintf(sql, "CREATE TABLE %s ( id varchar(128) PRIMARY KEY, pkg_name  varchar(128), batchCode varchar(32), apk_list TEXT,  apk_sum int,  date varchar(16));", pkg_table.c_str());
    if(sqlite3_exec(pkg_db, sql, NULL, NULL, &err_msg)!= SQLITE_OK){
        ;
    }else{
        sqlite3_exec(pkg_db, sql, NULL, NULL, &err_msg)!= SQLITE_OK;
    }

    dev_rc = sqlite3_open("sqldb_dev.db", &dev_db);
    sprintf(sql, "CREATE TABLE %s ( id varchar(128) PRIMARY KEY,  info varchar(128));", dev_table.c_str());
    if(sqlite3_exec(dev_db, sql, NULL, NULL, &err_msg)!= SQLITE_OK){
    }else{
        sqlite3_exec(dev_db, sql, NULL, NULL, &err_msg)!= SQLITE_OK;
    }
    string strver="version";
    string strapkver = "apkversion";
    string strpkgver = "pkgversion";
    if( ! dev_exist(strver)){
        sprintf(sql, "insert into dev_table (id, info)  values( \"version\", \"0\");");
        sqlite3_exec(dev_db, sql, NULL, NULL, &err_msg);
    }
    if( ! dev_exist(strapkver)){
        sprintf(sql, "insert into dev_table (id, info)  values( \"apkversion\", \"0\");");
        sqlite3_exec(dev_db, sql, NULL, NULL, &err_msg);
    }
    if( ! dev_exist(strpkgver)){
        sprintf(sql, "insert into dev_table (id, info)  values( \"pkgversion\", \"0\");");
        sqlite3_exec(dev_db, sql, NULL, NULL, &err_msg);
    }


    if( dev_rc == SQLITE_OK && apk_rc == SQLITE_OK && pkg_rc == SQLITE_OK && dev_rc == SQLITE_OK){
        return 1;
    }else{
        return 0;
    }
}

bool SqlOpt::exist_in_db(sqlite3 *db, const string table, const string id ){
    char sql_query[128]={0};
    sprintf(sql_query,"select count(*) from %s where id='%s'", table.c_str(), id.c_str());

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


void SqlOpt::get_unix_time( char time_str[]){
    sprintf(time_str,"%ld", time(0));
}

void SqlOpt::get_date( char date[], int date_length, long int diff){
    time_t timep=time(0);
    timep -= diff;
    strftime( date, date_length, "%Y%m%d", localtime(&timep));
}

int get_record(	void *params, int n_column, char **column_value, char **column_name){
    int i=0;
    strcpy( (char *)params, column_value[0]);
    for(i=1; i<n_column; i++){
        strcat( (char *)params, "|");
        strcat( (char *)params, column_value[i]);
    }
    if ( n_column == 0){
        return 0;
    }else{
        return 1;
    }

}

string SqlOpt::db_get( sqlite3 *db, string  table, const string id){
    string sql;
    int ret=0;
    char tmp[1024];
    sql.clear();
    sql = "select * from " + table + " where id ='" + id + "';";
    ret = sqlite3_exec( db, sql.c_str(), get_record, (void *)tmp, &err_msg);
    if( ret != SQLITE_OK){
         sqlite3_free(err_msg);
    }
    sql.clear();
    sql += (char *)tmp;
    return sql;
}

int SqlOpt::dev_update( const string id, const string info){
    string sql;
    sql.clear();
    int ret = 0;
    sql += "insert or replace into " ;
    sql += dev_table;
    sql += "( id, info )  values ( \"";
    sql += id;
    sql += "\", \"";
    sql += info;
    sql += "\");";
    ret = sqlite3_exec( dev_db, sql.c_str(), 0, 0, &err_msg );
    if ( ret != SQLITE_OK){
        ret = sqlite3_exec( dev_db, sql.c_str(), 0, 0, &err_msg );
    }
    return 0;
}

bool SqlOpt::dev_exist(const string id){
    return exist_in_db( dev_db, dev_table, id);
}

string SqlOpt::dev_get( const string dev_id){
    string res;
    res = db_get( dev_db, dev_table, dev_id);
    return res;
}

bool SqlOpt::mob_exist(const string id){
    return exist_in_db( mob_db, mob_table, id);
}

int SqlOpt::mob_update( const string id, const string info){
    string sql;
    sql.clear();
    int ret = 0;
    sql = sql + "insert or replace into " + mob_table + "( id, info )  values ( \"" + id.c_str() +"\", \"" + info.c_str() + "\");";
    ret = sqlite3_exec( mob_db, sql.c_str(), 0, 0, &err_msg );
    if ( ret != SQLITE_OK){
        ret = sqlite3_exec( mob_db, sql.c_str(), 0, 0, &err_msg );
    }
    return 0;
}

string SqlOpt::mob_get( const string mob_id){
    string res;
    res = db_get( mob_db, mob_table, mob_id);
    return res;
}

string SqlOpt::mob_get_pkgid(const string mob_id ){
    string sql;
    sql.clear();
    char tmp[1024];
    sql = "select info from dev_table where id = \"" + mob_id +"\";";
    sqlite3_exec( dev_db, sql.c_str(), get_record, (void *)tmp, &err_msg);
    sql.clear();
    sql += (char *)tmp;
    return sql;
}



string SqlOpt::apk_get( const string apk_id){
    string res;
    res = db_get( apk_db, apk_table, apk_id);
    return res;
}

int SqlOpt::apk_get_all( const string apk_id, ApkData *res){

    char *errmsg;
    char **dbResult;
    int index=0;
    int nRow;
    int nColumn;
    int rc;
    char sql[1024];

    sprintf(sql, "select * from %s where id = \"%s\"", apk_table.c_str(), apk_id.c_str());

    rc = sqlite3_get_table( apk_db, sql, &dbResult, &nRow, &nColumn, &errmsg);
    if (rc != SQLITE_OK){
        sqlite3_get_table( apk_db, sql, &dbResult, &nRow, &nColumn, &errmsg);
        return 0;
    }else{
        res->apkid = dbResult[index++];
        res->pkgname = dbResult[index++];
        res->counter = atoi(dbResult[index++]);
        res->icon = atoi(dbResult[index++]);
        res->run = atoi(dbResult[index++]);
        res->pkgname = dbResult[index++];
        //res->md5 = dbResult[index++];
        sprintf(res->md5, "%s", dbResult[index++]);
        return 1;
    }

}

string SqlOpt::dev_get_storeid(){
    string sql;
    sql.clear();
    char tmp[1024];
    sql = "select info from dev_table where id = \"storeid\"";
    sqlite3_exec( dev_db, sql.c_str(), get_record, (void *)tmp, &err_msg);
    sql.clear();
    sql += (char *)tmp;
    return sql;
}

string SqlOpt::dev_get_cid(){
    string sql;
    sql.clear();
    char tmp[1024];
    sql = "select info from dev_table where id = \"cid\"";
    sqlite3_exec( dev_db, sql.c_str(), get_record, (void *)tmp, &err_msg);
    sql.clear();
    sql += (char *)tmp;
    return sql;
}


string SqlOpt::dev_get_dev(){
    string sql;
    sql.clear();
    char tmp[1024];
    sql = "select info from dev_table where id = \"version\"";
    sqlite3_exec( dev_db, sql.c_str(), get_record, (void *)tmp, &err_msg);
    sql.clear();
    sql += (char *)tmp;
    return sql;
}

string SqlOpt::dev_get_apk(){
    string sql;
    sql.clear();
    char tmp[1024];
    sql = "select info from dev_table where id = \"apkversion\"";
    sqlite3_exec( dev_db, sql.c_str(), get_record, (void *)tmp, &err_msg);
    sql.clear();
    sql += (char *)tmp;
    return sql;
}

string SqlOpt::dev_get_pkg(){
    string sql;
    sql.clear();
    char tmp[1024];
    sql = "select info from dev_table where id = \"pkgversion\"";
    sqlite3_exec( dev_db, sql.c_str(), get_record, (void *)tmp, &err_msg);
    sql.clear();
    sql += (char *)tmp;
    return sql;
}

string SqlOpt::apk_get_path( const string apk_id, int *counter, int *run){
    string sql;
    int ret=0;
    sql.clear();
    char tmp[1024];
    sql = "select pkgname, counter, run from "+ apk_table+ " where id = '" + apk_id + "';";
//	cout <<sql<<endl;
    ret = sqlite3_exec( apk_db, sql.c_str(), get_record, ( void *)tmp, &err_msg);

    if (ret != SQLITE_OK){
        sqlite3_exec( apk_db, sql.c_str(), get_record, ( void *)tmp, &err_msg);
    }
    sql.clear();
    sql += (char *)tmp;
    cout << sql<< endl;
    int pos = sql.find("|");
    if(pos >= 0){
        string tmp_str = sql.substr(pos, sql.size()-pos);
        sql = sql.substr(0, pos);
        pos = tmp_str.find("|");
        if( pos >=0 ){
            int str_length = (int)tmp_str.size();
            *counter = atoi(tmp_str.substr(0, pos).c_str());
            *run = atoi(tmp_str.substr(pos, str_length-pos).c_str());
            return sql;
        }
    }
    sql.clear();
    return  sql;
}

string SqlOpt::apk_get_packagePath( const string apk_id){
    string sql;
    int ret=0;
    sql.clear();
    char tmp[1024];
    sql = "select packagePath "+ apk_table+ " where id = '" + apk_id + "';";
    ret = sqlite3_exec( apk_db, sql.c_str(), get_record, ( void *)tmp, &err_msg);

    if (ret != SQLITE_OK){
        sqlite3_exec( apk_db, sql.c_str(), get_record, ( void *)tmp, &err_msg);
    }
    sql.clear();
    sql += (char *)tmp;
    return  sql;
}

int SqlOpt::apk_update( const string apk_id, const string pkgname, const int counter, const int icon, const int run, const string md5){
    int ret;
    char sql[1024];
    sprintf(sql, "insert or replace into %s ( id, pkgname, counter, icon, run, md5 )  values ( \"%s\", \"%s\", %d, %d, %d, \"%s\");", apk_table.c_str(), apk_id.c_str(), pkgname.c_str(), counter, icon, run, md5.c_str());
    ret = sqlite3_exec( apk_db, sql, 0, 0, &err_msg);
    //cout << sql << endl;
    if( ret != SQLITE_OK){
        ret = sqlite3_exec( apk_db, sql, 0, 0, &err_msg);
    }
    return ret;
}

int SqlOpt::apk_update( const string apk_id, const string pkgname, const int counter, const int icon, const int run){
    int ret;
    char sql[1024];
    if( ! exist_in_db( apk_db, apk_table, apk_id)){
        sprintf(sql, "insert or replace into %s ( id, pkgname, counter, icon, run, md5 )  values ( \"%s\", \"%s\", %d, %d, %d, \"%s\");", apk_table.c_str(), apk_id.c_str(), pkgname.c_str(), counter, icon, run, "0");
    }else{
        sprintf(sql, " update %s  set  id=\"%s\",  pkgname=\"%s\",  counter=\"%d\",  icon=%d,  run=%d  where id = \"%s\";", apk_table.c_str(), pkgname.c_str(), counter, icon, run, apk_id.c_str());
    }
    ret = sqlite3_exec( apk_db, sql, 0, 0, &err_msg);
    //cout << sql << endl;
    if( ret != SQLITE_OK){
        ret = sqlite3_exec( apk_db, sql, 0, 0, &err_msg);
    }
    return ret;
}

int SqlOpt::apk_update( const string apk_id, const string md5){
    int ret;
    char sql[1024];
    if( ! exist_in_db( apk_db, apk_table, apk_id)){
        sprintf(sql, "insert or replace into %s ( id, pkgname, counter, icon, run, md5 )  values ( \"%s\", \"%s\", %d, %d, %d, \"%s\");", apk_table.c_str(), apk_id.c_str(), "0", 0, 0, 0, md5.c_str());
    }else{
        sprintf(sql, " update %s  set  md5=\"%s\"  where id = \"%s\";", apk_table.c_str(), md5.c_str(), apk_id.c_str());
    }
    ret = sqlite3_exec( apk_db, sql, 0, 0, &err_msg);
    //cout << sql << endl;
    if( ret != SQLITE_OK){
        ret = sqlite3_exec( apk_db, sql, 0, 0, &err_msg);
    }
    return ret;
}

int SqlOpt::apk_update_packagePath(const string apk_id, const string packagePath){
    int ret;
    char sql[1024];
    if( ! exist_in_db( apk_db, apk_table, apk_id)){
        sprintf(sql, "insert or replace into %s ( id, pkgname, counter, icon, run, md5, packagePath )  values ( \"%s\", \"%s\", %d, %d, %d, \"%s\");", apk_table.c_str(), apk_id.c_str(), "0", 0, 0, 0, "00", packagePath.c_str());
    }else{
        sprintf(sql, " update %s  set  packagePath=\"%s\"  where id = \"%s\";", apk_table.c_str(), packagePath.c_str(), apk_id.c_str());
    }
    ret = sqlite3_exec( apk_db, sql, 0, 0, &err_msg);
    //cout << sql << endl;
    if( ret != SQLITE_OK){
        ret = sqlite3_exec( apk_db, sql, 0, 0, &err_msg);
    }
    return ret;
}

string SqlOpt::pkg_get( const string pkg_id){
    string res;
    res = db_get( pkg_db, pkg_table, pkg_id);
    return res;
}



int SqlOpt::pkg_update( const string pkg_id, const string pkg_name, const string apk_list){
    int ret;
    string sql;
    sql += "insert or replace into " + pkg_table + " ( id, pkg_name, apk_list)  values (\"" + pkg_id + "\", \"" + pkg_name + "\", \"" + apk_list + "\");";
//	cout << sql << endl;
    ret = sqlite3_exec( pkg_db, sql.c_str(), 0, 0, &err_msg);
    if( ret != SQLITE_OK){
        sqlite3_exec( pkg_db, sql.c_str(), 0, 0, &err_msg);
    }
    return ret;
}

int SqlOpt::pkg_update( const string pkg_id, const string pkg_name, const string apk_list, const int apk_sum, const string date){
    int ret;
    string sql;
    char apksum[2];
    sprintf(apksum, "%d", apk_sum);
    sql += "insert or replace into " + pkg_table + " ( id, pkg_name, apk_list, apk_sum,  date)  values ( \"" + pkg_id + "\",  \"" + pkg_name + "\",  \"" + apk_list + "\","  +  apksum +  ", \"" + date  + "\");";
//	cout << sql << endl;
    ret = sqlite3_exec( pkg_db, sql.c_str(), 0, 0, &err_msg);
    if( ret != SQLITE_OK){
        sqlite3_exec( pkg_db, sql.c_str(), 0, 0, &err_msg);
    }
    return ret;
}
//    sprintf(sql, "CREATE TABLE %s ( id varchar(128) PRIMARY KEY, pkg_name  varchar(128), batchCode varchar(32), apk_list TEXT,  apk_sum int,  date varchar(16));", pkg_table.c_str());

string SqlOpt::pkg_get_apk_list( const string pkg_id){
    string sql;
    int ret=0;
    sql.clear();
    char tmp[1024];
    sql = "select apk_list "+ pkg_table+ " where id = '" + pkg_id + "';";
    ret = sqlite3_exec( pkg_db, sql.c_str(), get_record, ( void *)tmp, &err_msg);
    if (ret != SQLITE_OK){
        sqlite3_exec( pkg_db, sql.c_str(), get_record, ( void *)tmp, &err_msg);
    }
    sql.clear();
    sql += (char *)tmp;
    return  sql;
}

string SqlOpt::pkg_get_batchCode( const string pkg_id){
    string sql;
    int ret=0;
    sql.clear();
    char tmp[1024];
    sql = "select batchCode "+ pkg_table+ " where id = '" + pkg_id + "';";
    ret = sqlite3_exec( pkg_db, sql.c_str(), get_record, ( void *)tmp, &err_msg);
    if (ret != SQLITE_OK){
        sqlite3_exec( pkg_db, sql.c_str(), get_record, ( void *)tmp, &err_msg);
    }
    sql.clear();
    sql += (char *)tmp;
    return  sql;
}


int SqlOpt::pkg_update( const string pkg_id, const string pkg_name, const string apk_list, const string batchCode, const int apk_sum, const string date){
    int ret;
    string sql;
    char apksum[2];
    sprintf(apksum, "%d", apk_sum);
    sql += "insert or replace into " + pkg_table + " ( id, pkg_name, apk_list, batchCode, apk_sum,  date)  values ( \"" + pkg_id + "\",  \"" + pkg_name + "\",  \"" + apk_list + "\", \""  + batchCode.c_str()  + "\","  +  apksum +  ", \"" + date  + "\");";
//	cout << sql << endl;
    ret = sqlite3_exec( pkg_db, sql.c_str(), 0, 0, &err_msg);
    if( ret != SQLITE_OK){
        sqlite3_exec( pkg_db, sql.c_str(), 0, 0, &err_msg);
    }
    return ret;
}

void SqlOpt::sqlclose(){
    sqlite3_close( dev_db);
    sqlite3_close( apk_db);
    sqlite3_close( pkg_db);
    sqlite3_close( mob_db);

}
/*
int main(){
    ApkData ad;
    ad.apkid = "123";
    ad.pkgname = "abc";
    ad.counter = 1;
    ad.run = 0;
    sprintf(ad.md5, "asdfasdfasdf");

    SqlOpt sql;
    sql.sqlinit();
    cout <<ad.apkid << endl;
    char time[32];
    sql.get_unix_time(time);
    cout << time<< endl;

    string res;
    string id="moto";
    string value="hd330";

    sql.dev_update( id, value);
    cout  <<3<< "ok"<<endl;
    res = sql.dev_get(id);
    cout <<4<< res << endl;

    value = "adb";
    sql.mob_update( id, value);
    res = sql.mob_get( id);
    printf("%s\n", res.c_str());


    value = "asdfasdf";
    sql.pkg_update(id, value, value);
    res = sql.pkg_get(id);
    cout <<6<< res << endl;

    int counter=12;
    int run = 10;
    string amd5="00000";

    string apkid = id;
    string pkgname = value;
    sql.apk_update( apkid, pkgname, 1, 0, 1, amd5);
    cout << 7 <<endl;
    amd5="111111";
    res = sql.apk_get_path(id, &counter, &run);
    ApkData a;
    sql.apk_get_all(id, &a);
    cout << a.apkid << "\t" << a.pkgname <<"\t"<< a.md5<<endl;


    return 0;
}
*/
