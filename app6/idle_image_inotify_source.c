#include <linux/inotify.h>
#include <errno.h>
#include <sys/stat.h>

//#include "smartpad_protocol.h"
#include "event_source.h"
#include "main.h"

typedef struct _PrivInfo {
    int fd;
    int wd;
}PrivInfo;

static int g_f = 0;

static int ilde_image_inotify_source_get_fd(FtkSource *thiz) {
    DECL_PRIV(thiz, priv);
    //printf("udisk\n");
    return priv->fd;
}

static int ilde_image_inotify_source_check(FtkSource *thiz) {
    return -1;
}
static int FW_update1(const char *file) {
    int ret;
    char buf[64] = { 0, };
    /*
    printf("file: %s\n", file);
    snprintf(buf, sizeof(buf), "opkg --noaction install %s", file);
    printf("buf: %s\n", buf);
    ret = system(buf);
    if (ret) {
        return -1;
    }
    */
    snprintf(buf, sizeof(buf), "cp %s /opt/PBAPPS/app/", file);
    printf("buf: %s\n", buf);

    ret = system(buf);
    return ret;

    //return system("echo ./ipks/pb62_1.ipk > /etc/.need_update");

}
static Ret ftk_app_window_on_event1(const char *file) {
    int ret = 1;
    FtkWidget* win = NULL;
    FtkGc gc = {0};
    FtkWidget* button = NULL;
    int width,height,len;
    char filename[200] = {0};
    char info[300] = {0};
    char temp[20];
    const char *buttons[3] = {
        "YES", "NO", NULL
    };
    const char *_buttons[2] = {
        "OK", NULL
    };
    printf("event\n");
    win = ftk_app_window_create();
	printf("5\n");
//    ->on_event = ftk_app_dow_on_event;
	ftk_window_set_animation_hint(win, "app_download_window");
	printf("6\n");
	ftk_window_set_fullscreen(win, 1);
	width = ftk_widget_width(win);
	height = ftk_widget_height(win);
	gc.mask = FTK_GC_BITMAP;
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "/opt/PBAPPS/image/desktop1.png");
	ftk_widget_set_gc(win, FTK_WIDGET_NORMAL, &gc);
	ftk_bitmap_unref(gc.bitmap);
	/*
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "/opt/PBAPPS/image/qian1.png");
	ftk_widget_set_gc(win, FTK_WIDGET_FOCUSED, &gc);
	ftk_bitmap_unref(gc.bitmap);
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "/opt/PBAPPS/image/qian2.png");
	ftk_widget_set_gc(win, FTK_WIDGET_ACTIVE, &gc);
	ftk_bitmap_unref(gc.bitmap);
	*/
	sprintf(filename,"%s%s","/opt/PBAPPS/image/","25.png");
	printf("file is %s,len is %d\n",file,strlen(file));
	len = strlen(file);
	if(len == 21){
		memset(temp,0,sizeof(temp));
		memcpy(temp,&file[16],5);
        printf("number is %s\n",temp);
	}else if(len == 20){
		memset(temp,0,sizeof(temp));
		memcpy(temp,&file[16],4);
		printf("number is %s\n",temp);
	}
	printf("the last char is %c %c and %c\n",file[strlen(file)-1],file[strlen(file)-2],file[strlen(file)-3]);
	sprintf(info,"%s%s%s","a new app ",temp," found in udisk.Update it?");
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), filename);
	button = ftk_button_create(win,100,100, ftk_bitmap_width(gc.bitmap), ftk_bitmap_height(gc.bitmap));
	ftk_widget_set_gc(button, FTK_WIDGET_NORMAL, &gc);
	ftk_bitmap_unref(gc.bitmap);
	ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
	ftk_bitmap_unref(gc.bitmap);
	ftk_widget_show_all(win, 1);
	//ftk_widget_set_user_data(, idle_user_data_destroy, idle_data);
	//printscreen_PNG();
    if (1 == ftk_question("App Update", info, buttons)) {
        ret = FW_update1(file);
        if (ret) {
            ftk_warning("App Update", "Update Failed.", _buttons);
            ftk_widget_unref(win);
        } else {
            if (1 == ftk_infomation("App Update", "Update succeeded.\nReboot to install. Reboot Now?", buttons)) {
            	system("/opt/PBAPPS/bin/app_pb62 & -s");
            	exit(0);
            	//system("reboot");
                //sleep(10);
                while (1);
            }
            else ftk_widget_unref(win);
        }
    }
    else{
    	ftk_widget_unref(win);
    }

    return RET_REMOVE;
}

static Ret udisk_check(void *data) {
    int ret = 0;
    FtkFileInfo info;
    char file[64] = { 0, };
    char infomation[100];
    char *time_access;
    char *time_modify;
    int i,k;
	char file1[64] = { 0, };
	char file2[64] = { 0, };
	char file3[100];
	char file4[100];
	char path[100];
	char question[100];
	int ipk_num = 0;
	const char *buttons[3] = {
	        "YES", "rename", "CANCEL"
	    };
    const char *_buttons[2] = {
		"OK", NULL
	};
    struct stat _stat;
    struct inotify_event *event = (struct inotify_event *)data;
	if(memcmp(event->name,"sda",strlen("sda"))!=0){
		printf("a error udisk\n");
		return RET_FAIL;
	}
    //printf("event is %s\n",event->name);
    //sprintf(file2,"%s%s","/media/ram/",event->name);
    memcpy(file2,event->name,strlen(event->name));
    sprintf(path,"%s%s","/media/ram/",file2);
    //memcpy(path,file2,strlen(file2));
    printf("path is %s\n",path);
	if(check_dir(path,&ipk_num)==1){
		printf("a ipk is found\n");

	}
    for(i = 0;i < 99;i++){
		sprintf(file1,"%s%d","/app",i);
		//snprintf(file2, sizeof(file2), "/media/ram/%s/", event->name);
		//printf("file2 is %s\n",file2);
		memset(file3,0,sizeof(file3));
		sprintf(file3,"%s%s",path,file1);
		printf("file3 is %s\n",file3);
		ret = stat(file3, &_stat);
		if(ret == 0){
			printf("a new app %s is found\n",file3);
			break;
		}
	}
    sprintf(file4,"%s%s","/opt/PBAPPS/app/",file1);
    printf("file4 is %s\n",file4);
    ret = stat(file4, &_stat);
	if (ret == 0) {
		printf("the file %s has existed in PB6\n", file1);
		sprintf(question, "%s%s%s", "the file ", file1,
				"has existed in PB6,do you want to overwrite it?");
		k = ftk_question("App Update", question, buttons);
		printf("ret is %d\n",k);
		switch (k) {
		case 1:
			printf("override the orignal file\n");
			remove(file4);
			ftk_app_window_on_event1(file3);
			break;
		case 2:
			printf("rename a new app\n");
			for (i = 0; i < 99; i++) {
				memset(file1, 0, sizeof(file1));
				sprintf(file1, "%s%d", "/opt/PBAPPS/app/app", i);
				ret = stat(file1, &_stat);
				if (ret != 0) {
					printf("file1 %s is empty\n", file1);
					memset(file1, 0, sizeof(file1));
					sprintf(file1,"%s%d","/media/ram/sda1/app",i);
					rename(file3,file1);
					memset(file,0,sizeof(file));
					sprintf(file,"%s%d","the new file will be saved as app",i);
					ftk_tips(file);
					break;
				}
			}
			//rename(file4,file1);
			ftk_app_window_on_event1(file1);
			break;
		default:
			printf("cancel\n");
			break;
		}
	}
	else {
	    ret = ftk_file_get_info(file3,&info);
	    time_access = ctime(&info.last_access);
	    time_modify = ctime(&info.last_modify);
	    printf("time is %s and %s\n",time_access,time_modify);
	    printf("file is %s\n",file3);
	    /*
	    if (ret == 0) {
	    	printf("the file is exsiting\n");
	    	ftk_app_window_on_event1(file3);
	    } */
	    ftk_app_window_on_event1(file3);
	}
    g_f = 0;
    return RET_REMOVE;
}
static void handle_inotify_event(struct inotify_event *event) {
    FtkSource *timer = NULL;
    if (event->mask == (IN_ISDIR | IN_CREATE)) {
        if (!memcmp(event->name, "sd", 2) && g_f == 0) {
            g_f = 1;
            timer = ftk_source_timer_create(1000, udisk_check, event);
            ftk_main_loop_add_source(ftk_default_main_loop(), timer);
        }
    }
    return;
}

static Ret ilde_image_inotify_source_dispatch(FtkSource *thiz) {
    char buf[2048] = { 0 };
    int index = 0;
    int len = 0;
    struct inotify_event *event = NULL;

    DECL_PRIV(thiz, priv);
    printf("a udisk is find\n");
    while (((len += read(priv->fd, &buf[len], sizeof(buf) - len)) < 0) && (errno == EINTR));
    while (index < len) {
        event = (struct inotify_event *)(buf + index);
        handle_inotify_event(event);
        index += sizeof(struct inotify_event) + event->len;
    }
    return RET_OK;
}


static void ilde_image_inotify_source_destroy(FtkSource *thiz) {
    if (thiz != NULL) {
        DECL_PRIV(thiz, priv);
        if (priv) {
            inotify_rm_watch(priv->fd, priv->wd);
            close(priv->fd);
        }
        FTK_ZFREE(thiz, sizeof(thiz) + sizeof(PrivInfo));
    }
    return;
}

FtkSource* ilde_image_inotify_source_create(const char *dir) {
    FtkSource *thiz = NULL;
    thiz = (FtkSource *)FTK_ZALLOC(sizeof(FtkSource) + sizeof(PrivInfo));

    if (thiz == NULL) {
        return NULL;
    }

    DECL_PRIV(thiz, priv);
    thiz->ref = 1;
    thiz->get_fd   = ilde_image_inotify_source_get_fd;
    thiz->dispatch = ilde_image_inotify_source_dispatch;
    thiz->check    = ilde_image_inotify_source_check;
    thiz->destroy  = ilde_image_inotify_source_destroy;

    priv->fd = inotify_init();
    if (priv->fd < 0) {
        FTK_ZFREE(thiz, sizeof(FtkSource) + sizeof(PrivInfo));
        return NULL;
    }

    priv->wd = inotify_add_watch(priv->fd, dir, IN_CREATE);
    if (priv->wd < 0) {
        close(priv->fd);
        FTK_ZFREE(thiz, sizeof(FtkSource) + sizeof(PrivInfo));
        return NULL;
    }
    return thiz;
}



