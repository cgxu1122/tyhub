#include <unistd.h>  
#include <stdlib.h>  
#include <fcntl.h>  
#include <limits.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <stdio.h>  
#include <string.h> 
#include <QDir>
#include "record.h"
#include "data_sql.h"



int FILE_CTL::init(){
    flag = 1;

    data_sql = new Data_Sql;
    data_sql->sqlinit();

    char file_time[TIME_LENGTH];

    get_today(file_date);
    get_time(file_time);

    char time_file[128];
    char date_file[128];

    sprintf( date_file,  "%s%s.csv", DIR_PATH, file_date);
    sprintf( time_file,  "%s%s.csv", DIR_PATH, file_time);

    date_f.open( date_file, ios_base::out|ios_base::app);
    time_f.open( date_file, ios_base::out|ios_base::app);
    cnt = 0;
}

void FILE_CTL::get_unix_time(char time_str[]){
	sprintf(time_str, "%ld", time(0));
}

void FILE_CTL::get_today(char today[]){
    time_t timep;
    struct tm *p;
    time(&timep);
    p = localtime(&timep);
    sprintf( today, "%04d%02d%02d", (1900+ p->tm_year), (1 + p->tm_mon), (p->tm_mday) );
}

void FILE_CTL::get_time(char ntime[]){
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);
	sprintf( ntime, "%04d%02d%02d%02d%02d%02d", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
}

int  FILE_CTL::file_write( const string info){
	file_date_control();
	file_lines_control();
	date_f << info << endl;
	time_f << info << endl;
	cnt++;
}
void FILE_CTL::log_deal(){
	//int fd1,fd2,fd3,fd4,fd5,fd6,fd7,fd8,fd9,fd10,fd11,fd12;
	char buff[512];
	char fifo_name[128];
	int num;
	for(int i=0; i<12; i++){
		printf("%d\n",i);
		sprintf(fifo_name, "%s%d", FIFO, i);
		if  (access(fifo_name, F_OK)==  -1){
			fd[i] = mkfifo(fifo_name, 0777);
		}
//		printf("%s\n", fifo_name);
		fd[i]=open(fifo_name, O_RDONLY|O_NONBLOCK);
	}

	while(1){
		for(int i=0; i<12; i++){
			num = read(fd[i], buff, 512);
			file_write(buff);
		}
//		printf("while\n");
		sleep(1);
	}
}

/* one file write MAX_LINES data*/
int FILE_CTL::file_lines_control(){

	char file_name[128];
	char file_time[TIME_LENGTH];
    if ( cnt > MAX_LINES ){
		cnt = 0;
        time_f.close();
    }
    if( cnt = 0){
        sprintf( file_name,  "%s%s.csv", TIME_DIR_PATH, file_time);
		time_f.open( file_name, ios_base::out|ios_base::app);
        //sqlite3 add
        data_sql->log_write_incr();
        cnt = 1;
    }
    if( cnt >=1 && cnt <=MAX_LINES){
		return 0;
	}
}

void FILE_CTL::file_date_control(){
	char file_name[128];
	char tmp_date[DATE_LENGTH];

	get_today(tmp_date);
	sprintf( file_name,  "%s%s.csv", TIME_DIR_PATH, tmp_date);
	
	date_f.close();
	date_f.open( file_name, ios_base::out|ios_base::app);		

//	printf("%s\n", file_name);
}

int FILE_CTL::file_delete(char *file){
	char file_name[256];
	char cmd[256];
	sprintf(cmd , "rm %s%s", DIR_PATH, file);
	if(access(file_name, F_OK) == F_OK){
		sprintf( cmd, "rm %s", file);
		system(cmd);
	}
}

int  FILE_CTL::file_delete(){
	int today_max=0;
	int flag=0;
    char today[24];

    time_t timep;
    struct tm *p;
    time(&timep);
	timep = timep - 31*24*60*60;
    p = localtime(&timep);
	char month[6];
    sprintf( month, "%04d%02d", (1900+ p->tm_year), (1 + p->tm_mon));

	char cmd[144];
	char file_name[128];

	for(int i=1; i<=31; i++){
		sprintf(file_name, "%s%s%02d.csv", DIR_PATH, month, i);
		if(access(file_name, F_OK) == F_OK){
			sprintf(cmd, "rm %s", file_name);
			system(cmd);
//			printf("%s\n",cmd);
		}
	}
	
}

int  FILE_CTL::file_delete(char *date_b, char* date_a){
	char tmp_day[DATE_LENGTH];
	long int bef_day;
	long int aft_day;
	long int tmp=0;
	bef_day = atoi( date_b);
	aft_day = atoi( date_a);
	if( bef_day < aft_day){
		for( tmp = bef_day; tmp <= aft_day; tmp++){
			//printf("%ld\n", tmp);
			sprintf(tmp_day, "%ld", tmp);
			file_delete( tmp_day);
		}
	}else if( bef_day > aft_day){
		for( tmp = aft_day; tmp <= bef_day; tmp++){
			//printf("%ld\n", tmp);
			sprintf( tmp_day,"%ld", tmp);
			file_delete( tmp_day);
		}
	}else if( bef_day == aft_day){
		sprintf(tmp_day, "%ld", tmp);
		file_delete(tmp_day);
		//printf("%ld\n", tmp);
	}
	return 0;
}

int FILE_CTL::file_close(){
    flag = 0;
    int fd_tmp;
    char fifo_name[256];
    for(int i=0; i<12; i++){
        sprintf(fifo_name, "%s%d", FIFO, i);
        if  (access(fifo_name, F_OK)==  -1){
            fd_tmp = fd[i];
            close( fd_tmp);
        }
    }
    date_f.close();
    time_f.close();
    return 0;
}


void FILE_CTL::mkdir(QString  path){
    QDir *tmp = new QDir;
    QStringList  lpath = path.split("/");
    QString new_path;
  //      foreach( QVariant atom, apklist){
    foreach( QString  atom, lpath){
        new_path += "/";
        if ( atom != ""){
            new_path += atom;
            if( ! tmp->exists(new_path)){
                tmp->mkdir( new_path);
            }
        }
    }

}

void FILE_CTL::mk_filedir(QString  path){
    QDir *tmp = new QDir;
    QStringList  lpath = path.split("/");
    QString new_path;
  //      foreach( QVariant atom, apklist){
    foreach( QString  atom, lpath){
        new_path += "/";
        if ( atom != ""){
            new_path += atom;
            if( ! tmp->exists(new_path)){
                tmp->mkdir( new_path);
            }
        }
    }
    tmp->rmdir(new_path);

}

//int main(){
//	FILE_CTL file_ctl;
//	file_ctl.init();
//	printf("ok\n");

//	char time[TIME_LENGTH];
//	file_ctl.get_time(time);
//	printf("%s\n", time);
//	file_ctl.file_delete();
//	file_ctl.file_date_control();
//	file_ctl.log_deal();
//	file_ctl.file_close();

//	return 0;
//}
 
