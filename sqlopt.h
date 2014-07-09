#ifndef _SQLOPT_H__
#define _SQLOPT_H__
#include<sqlite3.h>
#include<string>
#include<string.h>
#include<vector>

#define APKS_PATH  "/tmp/apk_lib"

using std::string;
using namespace std;

typedef struct APkDATA{
    string apkid;
    string pkgname;
    int counter;
    int icon;
    int run;
    char md5[32];
}ApkData;


class SqlOpt
{
    public:
        int sqlinit();
        void get_unix_time(char time_str[]);
        void get_date( char date[], int date_length, long int diff);
        string db_get( sqlite3 *db,string table, const string id);
        int db_update( sqlite3 *db, string table, const string id, int opt, const string value);

        string dev_get( const string device_id);
        int dev_update( const string device_id,  const string value);
        bool dev_exist( const string id);
        string dev_get_storeid();
        string dev_get_dev();
        string dev_get_apk();
        string dev_get_pkg();
        string dev_get_cid();

        string mob_get( const string mob_id);
        int mob_update( const string mob_id, const string value);
        bool mob_exist( const string id);
        string mob_get_pkgid(const string mob_id );

        string pkg_get( const string pkg_id);
        string pkg_get_apk_list( const string pkg_id);
        string pkg_get_batchCode( const string pkg_id);
        int pkg_update( const string pkg, const string pkg_name, const string value);
        int pkg_update( const string pkg_id, const string pkg_name, const string apk_list, const int apk_sum, const string date);
        int pkg_update( const string pkg_id, const string pkg_name, const string apk_list, const string batchCode, const int apk_sum, const string date);


        string apk_get( const string apk_id);
        int apk_get_all( const string apk_id, ApkData *res);
        int apk_update(const string apkid, const string pkgname, const int counter, const int icon, const int run ,const string md5);
        int apk_update( const string apk_id, const string pkgname, const int counter, const int icon, const int run);
        int apk_update(const string apk_id, const string md5);
        int apk_update_packagePath(const string apk_id, const string packagePath);
        string apk_get_path( const string apk_id, int *conuter , int *icon);
        string apk_get_packagePath(const string apk_id);

        bool exist_in_db(sqlite3 *db, const string table, const string id );
        void sqlclose();
    private:
        sqlite3 *dev_db;
        sqlite3 *mob_db;
        sqlite3 *pkg_db;
        sqlite3 *apk_db;
        string  dev_table;
        string  mob_table;
        string  pkg_table;
        string  apk_table;
        char  *err_msg;
};

int print_record(void *params, int n_column, char **column_value, char **column_name);
#endif
