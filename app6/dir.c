#define _XOPEN_SOURCE 500    
#include<ftw.h>   
#include<sys/stat.h>   
#include<unistd.h>   
#include<stdio.h>   
#include<string.h>
#include "ftk_typedef.h"
#include "ftk.h"
#include "main.h"
#define FILEOPEN 1024    
int gb_filecount;
int getMetadata(const char *dirpath, const struct stat *sb,int typeflag, struct FTW *ftwbuf);
static int FW_update(const char *file);
static Ret ftk_app_window_on_event(const char *file);
static int FW_update(const char *file) {
    int ret;
    FtkWidget* wait_box = NULL;
    char buf[64] = { 0, };
    char cmd[100] = {0,};
    printf("file: %s\n", file);
    /*
    snprintf(buf, sizeof(buf), "opkg --noaction install %s", file);
    printf("buf: %s\n", buf);
    ret = system(buf);
    if (ret) {
        return -1;
    }
    */
    wait_box = ftk_widget_lookup(app_wins[APP_WIN_0],WAIT_BOX_ID);
    ftk_widget_show(wait_box, 1);
    ftk_wait_box_start_waiting(wait_box);
	if (access("/opt/PBAPPS/ipks/pb62_1.ipk", F_OK) == 0) {
		printf("pb62 is exist\n");
		sprintf(cmd, "%s", "rm /opt/PBAPPS/ipks/pb62_1.ipk");
		//ftk_wait_box_start_waiting(wait_box);
		ret = system(cmd);
		if (ret) {
			return -1;
		}
	}

	/*
    snprintf(buf, sizeof(buf), "cp %s /opt/PBAPPS/ipks/", file);
    printf("buf: %s\n", buf);
    ret = system(buf);
    if (ret) {
		  return -1;
	  }
    */
    memset(cmd,0,sizeof(cmd));
    sprintf(cmd,"%s %s %s","cp",file,"/opt/PBAPPS/ipks/pb62_1.ipk");
    printf("cmd is %s\n",cmd);

    ret = system(cmd);
    if (ret) {
	   return -1;
    }
    ftk_tips("updating,please wait for some minutes");
    memset(buf,0,sizeof(buf));
    snprintf(buf, sizeof(buf), "opkg install /opt/PBAPPS/ipks/pb62_1.ipk");
    printf("buf: %s\n", buf);
    ret = system(buf);
        if (ret) {
            return -1;
        }
    ftk_wait_box_stop_waiting(wait_box);
    //ftk_widget_remove_child(app_wins[APP_WIN_0], wait_box);
    ftk_widget_show(wait_box, 0);
    //ftk_widget_destroy(wait_box);
    //return 0;
    //system("echo ./ipks/pb62_1.ipk > /etc/.need_update");
    return system("echo ./ipks/pb62_1.ipk > /etc/.need_update");
}
static Ret ftk_app_window_on_event(const char *file) {
    int ret;
    const char *buttons[3] = {
        "YES", "NO", NULL
    };
    const char *_buttons[2] = {
        "OK", NULL
    };

    if (1 == ftk_question("Firmware Update", "Firmware found in udisk. Update it?",  buttons)) {
        ret = FW_update(file);
        if (ret) {
            ftk_warning("Firmware Update", "Update Failed.", _buttons);
        } else {
            if (1 == ftk_infomation("Firmware Update", "Update succeeded.\nReboot to install. Reboot Now?", buttons)) {
                system("reboot");
                while (1);
            }
        }
    }
    return RET_REMOVE;
}
int check_dir(char *path,int *number){
     
  int ret = 0;   
  struct stat pathbuf;   
  if(stat(path,&pathbuf)){
    printf("-nfwt_t:invalid dirpath:%s/n",path);
    return -1;   
  }else{   
    if(0 == S_ISDIR(pathbuf.st_mode)){   
      printf("-nfwt_t:/""%s/" "is not dirpath/n",path);
      return -1;   
    }   
  }   
  gb_filecount=0;   
  ret = nftw(path,getMetadata,FILEOPEN,FTW_PHYS);
    if(ret<0){   
    printf("-nftw:[wrong:%d]ntfw search %d files/n",ret,gb_filecount);   
    return -1;   
  }else if(ret == 0){
    printf("-nftw:[done:%d]trasvers in %s search %d files/n",ret,path,gb_filecount);
    *number = gb_filecount;
    return 0;   
  }
  else if(ret == 99){
	  return 1;
  }
}   
int    
getMetadata(const char *dirpath, const struct stat *sb,int typeflag, struct FTW *ftwbuf){
  int len = 0;
  char name[20];
  char path[100];
  printf("num:%d path:%s\n",++gb_filecount,dirpath);
  len = strlen(dirpath);
  snprintf(name,5,"%s",&dirpath[len - 4]);
  if(memcmp(name,".ipk",5)==0){
	  snprintf(path,14,"%s",dirpath);
	  //printf("path is %s and len is %d\n",path,strlen(path));
	  if(memcmp(path,"/media/ram/sda",13)==0){
		  printf("a install package %s is found\n",dirpath);
		  ftk_app_window_on_event(dirpath);
		  return 99;
	  }
	  else{
		  return 0;
	  }
  }
  printf("ipk is %s\n",name);
  //memcmp(dirpath[len - 4],".ipk",sizeof(".ipk"));
  /*
  printf("st_dev:%d ",(*sb).st_dev);   
  printf("st_ino:%d ",(*sb).st_ino);   
  printf("st_mode:%d S_ISDIR:%d ",(*sb).st_mode,S_ISDIR((*sb).st_mode));   
  printf("st_nlink:%d ",(*sb).st_nlink);   
  printf("st_uid:%d ",(*sb).st_uid);   
  printf("st_gid:%d ",(*sb).st_gid);   
  printf("st_rdev:%d ",(*sb).st_rdev);   
  printf("st_size:%d ",(*sb).st_size);   
  printf("st_blksize:%lu ",(*sb).st_blksize);   
  printf("st_blocks:%lu ",(*sb).st_blocks);   
  printf("st_atime:%ld ",(*sb).st_atime);   
  printf("st_mtime:%ld ",(*sb).st_mtime);   
  printf("st_ctime:%ld ",(*sb).st_ctime);   
  printf("typeflag:%d ",typeflag);   
  printf("FTW_base:%d FTW_level:%d /n",(*ftwbuf).base,(*ftwbuf).level);
  */
  return 0;   
} 
