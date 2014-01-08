/*
 * download.c
 *
 *  Created on: 2013年12月4日
 *      Author: Administrator
 */
/*
 * main.c
 *
 *  Created on: 2013年11月22日
 *      Author: Administrator
 */
#include <time.h>
#include <sys/stat.h>
#include <ftw.h>
#include "ftk.h"
//#include "download.h"
#include "main.h"
#include "fbutils.h"
//#include "demo_tab.h"
#define RAND_MAX   40

typedef struct {
    char file[100];
    int dcc_started;
    int base_c_code;
    int dcc_c_page;
    int dcc_t_page;
    int dcc_cnt;
}user_data_t;
const char* pngs[] =
{
		"app0",
		"app1",
		"app2",
		"app3",
		"app4",
		"app5",
		"app6",
		"app7",
		"app8",
		"app9",
		"app10",
		"app11",
		"app12",
		"app13",
		"app14",
		"app15",
		"app16",
		"app17",
		"app18",
		"app19",
        "app20",
		"app21",
		"app22",
		"app23",
		"app24",
		"app25",
		"app26",
		"app27",
		"app28",
		"app29",
		"/Camera.png",
		"/Notes.png",
		"/RSS.png",
		"/Calculator.png",
		"/Twitter.png",
		"/Clock.png",
		"/Games.png",
		"/Youtube.png",
		"/AIM.png",
		"/LastFm.png",
		"/IP.png",
		"/Acrobat.png",
		"/Settings.png",
		"/Customize.png",
		"/Skype.png",
		"/Linkedin.png",
		"/Calender.png",
		"/Call.png",
		"/Install.png",
		"/Facebook.png",
		"/iPod.png",
		"/Chat.png",
		"/WLM.png",
		"/Flickr.png",
		"/Photos.png",
		"/Stock.png",
		"/Delicious.png",
		"/Maps.png",
		"/iTunes.png",
		"/Memory.png",
		"/Weather.png",
		"/Wordpress.png",
		"/iradio.png",
		"/Safari.png",
		"/Google.png",
		"/Yahoo.png",
		"/VLC.png",
		"/Sms.png",
		"/Mail.png",
		NULL
};
const char* wow1[] ={
		"INV_Jewelry_Ring_19.png",
		"INV_Jewelry_Ring_20.png",
		"INV_Jewelry_Ring_21.png",
		"INV_Jewelry_Ring_22.png",
		"INV_Jewelry_Ring_23.png",
		"INV_Jewelry_Ring_24.png",
		"INV_Jewelry_Ring_25.png",
		"INV_Jewelry_Ring_26.png",
		"INV_Jewelry_Ring_27.png",
		"INV_Jewelry_Ring_28.png",
		"INV_Jewelry_Ring_29.png",
		"INV_Jewelry_Ring_30.png",
		"INV_Jewelry_Ring_31.png",
		"INV_Jewelry_Ring_32.png",
		"INV_Jewelry_Ring_33.png",
		"INV_Jewelry_Ring_34.png",
		"INV_Jewelry_Ring_35.png",
		"INV_Jewelry_Ring_36.png",
		"INV_Jewelry_Ring_37.png",
		"INV_Jewelry_Ring_38.png",
		"INV_Jewelry_Ring_39.png",
		"INV_Jewelry_Ring_40.png",
};
const char* wow2[] ={
		"1.png","2.png","3.png","4.png","5.png","6.png","7.png","1.png","1.png","1.png","1.png","1.png",
		"1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png",
		"1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png",
		"1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png",
		"1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png",
		"1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png",
		"1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png",
		"1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png","1.png",
};
const char* crystal[] ={
		"1.png","2.png","3.png","4.png","5.png","6.png","7.png","8.png","9.png","10.png","11.png","12.png",
		"13.png","14.png","15.png","16.png","17.png","18.png",
};
//FtkWidget* app_wins[APP_WIN_NUM];
static const char* buttons[] = {"OK", NULL};
static const char* buttons_delete[] = {"OK","Cancel", NULL};
//FtkWidget *main_button[18];//[18];
FtkWidget *main_label[18];
static Ret button_clicked(void* ctx, void* obj)
{
	//ftk_widget_unref(ctx);
	button_config_clicked(ctx,obj);
	return RET_OK;
}
static Ret on_prepare_options_menu(void* ctx, FtkWidget* menu_panel)
{
	int i = 0;
	for(i = 0; i < 3; i++)
	{
		char text[32] = {0};
		FtkWidget* item = ftk_menu_item_create(menu_panel);
		snprintf(text, sizeof(text), "Menu%02d", i);
		ftk_widget_set_text(item, text);
		ftk_widget_show(item, 1);
	}

	return i > 0 ? RET_OK : RET_FAIL;
}
#define IDC_TEST_BUTTON 1000
static Ret button_cancel_clicked(void* ctx, void* obj)
{
	ftk_widget_unref(ctx);
	//printscreen_PNG();
	return RET_OK;
}
static Ret button_info_clicked(void* ctx, void* obj)
{
	//ftk_widget_unref(ctx);
    //printf("app\n");
	FtkFileInfo info;
	char text[100];
	char *time_access;
	char *time_modify;
	Ret ret;
	int abc;
	struct stat buf;
	stat(ctx, &buf);
	abc = buf.st_mode & S_IFDIR;
	if(abc == S_IFDIR)
		printf("It's a directory.\n");
    printf("mode is %d\n",buf.st_mode);
	ret = ftk_file_get_info(ctx,&info);
	if(ret == RET_OK){
		printf("size is %d,time is %d,mode is %d\n",info.size,info.last_modify,info.mode);
		time_access = ctime(&info.last_access);
		time_modify = ctime(&info.last_modify);
		printf("access is %s,modify is %s\n",time_access,time_modify);
		sprintf(text,"name of app is %s\n\nsize of file is %d Bytes\n\nlast access is %s\nlast modify is %s\nmode is %d\n",
				info.name,info.size,time_access,time_modify,info.mode);
	}
	ftk_infomation("Infomation",text, buttons);
	return RET_OK;
}

static Ret button_delete_clicked(void* ctx, void* obj)
{
	/*
	if(!ftk_window_is_fullscreen(ctx))
	{
		ftk_infomation("Infomation", "the app has been deteled.", buttons);
	}
	else
	{
		ftk_window_set_fullscreen(ctx, 0);
	}
	ftk_logd("%s: width=%d height=%d\n", __func__, ftk_widget_width(ctx), ftk_widget_height(ctx));
    */
	int i,j;
	char info[100];
	char path[100];
	FtkWidget* label = NULL;
	FtkWidget* button = NULL;
	const char *text;
	sprintf(info,"%s%s","Do you really want to delete ",ctx);
	j = ftk_question("app delete",info,buttons_delete);
	if(j==1){
		sprintf(path,"%s%s","rm /opt/PBAPPS/app/",ctx);
		system(path);
		ftk_tips("app has been deleted");
		system("/opt/PBAPPS/bin/app_pb62 & -s");
	    exit(0);
		for(i = 0;i < 18;i++){
			label = ftk_widget_lookup(app_wins[APP_WIN_0],i+20);
			button = ftk_widget_lookup(app_wins[APP_WIN_0],i);
			/*
			text = ftk_widget_get_text(label);
			printf("text is %s\n",text);
			*/
			if(memcmp(ctx,text,strlen(ctx))==0){
               printf("%s is deleted\n",pngs[i]);

               //i--;
               //ftk_widget_unref_self(main_label[i]);
               //ftk_widget_set_active(button,0);
               //ftk_widget_destroy(button);
               ftk_widget_remove_child(app_wins[APP_WIN_0],button);
               ftk_widget_remove_child(app_wins[APP_WIN_0],label);
               /*
               ftk_widget_set_text(label,"");
               ftk_widget_set_active(label,0);
               ftk_widget_set_active(label,1);
               */
               ftk_widget_unref(app_wins[APP_WIN_1]);
               return RET_OK;
			}
		}
	}
	return RET_OK;
}
static Ret button_set_clicked(void* ctx, void* obj)
{
	char script[100] = "echo \"#!/bin/sh -x\n.";
	char temp[100];
	char path[100];
	/*
	if(!ftk_window_is_fullscreen(ctx))
	{
		ftk_infomation("Infomation", "the app has been deteled.", buttons);
	}
	else
	{
		ftk_window_set_fullscreen(ctx, 0);
	}
	ftk_logd("%s: width=%d height=%d\n", __func__, ftk_widget_width(ctx), ftk_widget_height(ctx));
    */
	int i;
	i = ftk_question("app setting","NOTE:if you set this item,the application will run as the default program!\nDo you want to set this app as a init option?",buttons_delete);
	printf("i = %d\n",i);
	if(i == 1){
       chdir("/opt/PBAPPS/bin/");
       system("mv app_pb62 app_pb62_old");
       //sprintf(path,"%s%s%s","cp /opt/PBAPPS/app/",ctx," .");
       sprintf(path,"%s%s","/opt/PBAPPS/app/",ctx);
       //system(path);
       //system("mkdir test.sh");
       //system("echo ""#!/bin/sh -x\necho ""hello"""">test.sh");
       sprintf(temp,"%s%s%s",script,path,"\">app_pb62");
       printf("cmd is %s\n",temp);
       system(temp);
       system("mv /etc/network/interfaces /etc/network/old_interfaces");
       system("mv /etc/network/backup_interfaces /etc/network/interfaces");
       system("chmod 777 app_pb62");
	}
	return RET_OK;
}

static Ret button_run_clicked(void* ctx, void* obj)
{
	int status;
	printf("%s will run\n",ctx);
	/*
	if(ftk_window_is_fullscreen(ctx))
	{
		ftk_infomation("Infomation", "Windows is fullscreen.", buttons);
	}
	else
	{
		ftk_window_set_fullscreen(ctx, 1);
	}

	ftk_logd("%s: width=%d height=%d\n", __func__, ftk_widget_width(ctx), ftk_widget_height(ctx));
    */
	//system(appname);
	//exit(0);
	int i,ret;
	char appname[100];
	char cmd[100];
	sprintf(appname,"%s%s","/opt/PBAPPS/app/",ctx);
	sprintf(cmd,"%s%s",appname,"&");
	i = ftk_question("app run","Do you want to run this app?",buttons_delete);
	printf("i = %d\n",i);
	if(i==1){
		ret = ftk_tips("app manager is quit,new app is running");
		printf("the ret is %d\n",ret);
		//fork();
		ret = system(appname);
		wait(&status);
		printf("process is %d\n",ret);
		ftk_widget_show_all(app_wins[APP_WIN_1], 0);
		ftk_widget_show_all(app_wins[APP_WIN_1], 1);
		if(ret==-1){
			ret = ftk_tips("app can't run,please check if it's valid");
			printf("the ret is %d\n",ret);
			return RET_OK;
		}
		else {
			//exit(0);
		}
	}
	return RET_OK;
}
static Ret app_clicked(void* ctx, void* obj)
{
	//ftk_widget_unref(ctx);
	user_data_t *usrdata;
	FtkFileInfo info;
	int width = 0;
	int height = 0;
	FtkWidget* win = NULL;
	FtkWidget* button = NULL;
	FtkBitmap* image = NULL;
	FtkGc gc = {0};
	Ret ret;
	char appname[100];
	char *name = (char *)ctx;
	char *type;
	struct stat *buf;
	printf("file is %s\n",name);
	//usrdata =(user_data_t *)ftk_widget_user_data((FtkWidget*)ctx);
    //printf("app is %s,name is %s\n",usrdata->file,name);
    //chdir("/opt/PBAPPS/app/");
    ret = ftk_file_get_info(name,&info);
	if(ret == RET_OK){
		printf("size is %d,time is %d,mode is %d\n",info.size,info.last_modify,info.mode);
	}
	else return RET_FAIL;
    sprintf(appname,"%s%s","/opt/PBAPPS/app/",name);
    //sprintf(appname,"%s%s",appname,"&");
    printf("app is %s\n",appname);
    /*
    if(stat(name,buf)==0){
        	printf("time is %d,size is %d,mode is %d",buf->st_mtim,buf->st_blksize,buf->st_mode);
    }
    type = ftk_file_get_mime_type(appname);

    printf("type is %s\n",type);
    */
    printf("new window\n");
    app_wins[APP_WIN_1] = ftk_app_window_create();
	ftk_window_set_animation_hint(app_wins[APP_WIN_1], "app_main_window");
	gc.mask = FTK_GC_BITMAP;
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "/opt/PBAPPS/image/desktop3.png");
	ftk_widget_set_gc(app_wins[APP_WIN_1], FTK_WIDGET_NORMAL, &gc);
	ftk_bitmap_unref(gc.bitmap);
	width = ftk_widget_width(app_wins[APP_WIN_1]);
	height = ftk_widget_height(app_wins[APP_WIN_1]);

	width = width/2 - 10;
	button = ftk_button_create(app_wins[APP_WIN_1], 0, height/4, width, 50);
	ftk_widget_set_text(button, "Run");
	ftk_button_set_clicked_listener(button, button_run_clicked, name);

	gc.mask = FTK_GC_FG;
	gc.fg.a = 00;
	gc.fg.r = 0x45;
	gc.fg.g = 0x98;
	gc.fg.b = 0xb3;
	ftk_widget_set_gc(button, FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(button, FTK_WIDGET_INSENSITIVE, &gc);
	//ftk_widget_set_gc(app_wins[APP_WIN_1], FTK_WIDGET_FOCUSED, &gc);
	ftk_widget_set_gc(button, FTK_WIDGET_ACTIVE, &gc);
	//ftk_gc_reset(&gc);

	button = ftk_button_create(app_wins[APP_WIN_1], width + 10, height/4, width, 50);
	ftk_widget_set_text(button, "Delete");
	ftk_button_set_clicked_listener(button, button_delete_clicked, name);

	button = ftk_button_create(app_wins[APP_WIN_1], width + 10, height/2, width, 60);
	ftk_widget_set_text(button, "Setting");
	ftk_button_set_clicked_listener(button, button_set_clicked, name);

	button = ftk_button_create(app_wins[APP_WIN_1], 0, height/2, width, 60);
	ftk_widget_set_text(button, "Information");
	ftk_button_set_clicked_listener(button, button_info_clicked, name);
	/*
	image = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "/opt/PBAPPS/app/icon1.png");
	ret = ftk_image_set_image(button,image);
	ftk_bitmap_unref(image);
	printf("ret is %d\n",ret);
    */
	button = ftk_button_create(app_wins[APP_WIN_1], width/2 + 10, height/2 + 100, width, 60);
	ftk_widget_set_text(button, "Cancel");
	ftk_button_set_clicked_listener(button, button_cancel_clicked, app_wins[APP_WIN_1]);
	ftk_window_set_focus(app_wins[APP_WIN_1], button);

	ftk_widget_set_text(app_wins[APP_WIN_1], "fullscreen");
	ftk_widget_show_all(app_wins[APP_WIN_1], 1);
	//FTK_QUIT_WHEN_WIDGET_CLOSE(app_wins[APP_WIN_1]);
	ftk_app_window_set_on_prepare_options_menu(app_wins[APP_WIN_1], on_prepare_options_menu, app_wins[APP_WIN_1]);
	//system("cd /home/");
    //system(appname);
    //exit(0);
	return RET_OK;
}
Ret set_listener(FtkWidget* widget,void *ptr)
{
	ftk_button_set_clicked_listener(widget, app_clicked,ptr);
	return RET_OK;
}
#ifdef FTK_AS_PLUGIN
#include "ftk_app_demo.h"
FTK_HIDE int FTK_MAIN(int argc, char* argv[]);
FtkApp* ftk_app_demo_image_button_create()
{
	return ftk_app_demo_create(_("image_button"), ftk_main);
}
#else
#define FTK_HIDE extern
#endif /*FTK_AS_PLUGIN*/
typedef struct _TimerInfo
{
	int times;
	FtkWidget* label;
}TimerInfo;

static Ret timeout(void* ctx)
{
	TimerInfo* info = ctx;
	char buffer[32] = {0};
	if(info->times > 0)
	{
		//snprintf(buffer, sizeof(buffer), "Quit after %d seconds", info->times);
		sprintf(buffer, "Quit after %d seconds", info->times);
		ftk_widget_set_text(info->label, buffer);
		info->times--;
		//ftk_widget_show(label1,1);
		//printf("%ds\n",info->times);
		printf("%s\n",buffer);
		if(info->times == 1){
			info->times = 200;
		}
		return RET_OK;
	}
	else
	{
		ftk_widget_unref(ftk_widget_toplevel(info->label));
		ftk_logd("%s: timeout and quit.\n", __func__);
		FTK_FREE(info);
		FTK_QUIT();
		return RET_REMOVE;
	}
}
static Ret update_time(void* ctx)
{
	char text[10] = {0};
	FtkWidget* item = NULL;
	FtkWidget* panel = NULL;
	FtkWidget* win = ctx;
	FtkWidget* button = NULL;
	FtkGc gc = {0};
	struct timespec nano = {0};
	clock_gettime(CLOCK_REALTIME,&nano);
	srand((int)(nano.tv_nsec));
	//sprintf(temp_filename,"%d%s",rand()%96,".png");
	button = ftk_widget_lookup(win,rand()%17);
	//ptr = ftk_widget_get_text(button);
	//printf("p2 is %s\n",ptr);
	/*
	if(ftk_widget_is_active(button)){
		ftk_widget_set_active(button,0);
	}
	else ftk_widget_set_active(button,1);
	*/
}
//FTK_HIDE int FTK_MAIN(int argc, char* argv[])
static void dcc_user_data_destroy(void *data) {
	user_data_t *userdata = (user_data_t *)data;
    free(userdata);
}
int win_download_creat(void)
{
	int i = 0;
	int j = 0;
	static int k = 0;
	static int app_number = 0;
	int app_total = 0;
	int m = 0;
	static int last = 0;
	int w = 133;
	int h = 155;
	int width = 0;
	int height = 0;
	int size;
	Ret ret;
	char filename[FTK_MAX_PATH] = {0};
	char cmd[100];
	char temp_filename[FTK_MAX_PATH] = {0};
	char appname[FTK_MAX_PATH] = {0};
	struct timespec nano = {0};
	struct tm temptm;
	FtkGc gc = {0};
	TimerInfo* info = NULL;
	FtkSource* timer = NULL;
	//FtkWidget* win = NULL;
	FtkWidget* button = NULL;
	FtkWidget* wait_box = NULL;
	FtkWidget* label = NULL;
	FtkFileInfo file_info;
	user_data_t userdata;
	//FTK_INIT(argc, argv);
	info = (TimerInfo*)FTK_ZALLOC(sizeof(TimerInfo));
	info->times = 200;
	//timer = ftk_source_timer_create(1000, timeout, info);
	gc.mask = FTK_GC_BITMAP;
	app_wins[APP_WIN_0] = ftk_app_window_create();
	//win = app_wins[APP_WIN_0];
	ftk_window_set_animation_hint(app_wins[APP_WIN_0], "app_main_window");
	width = ftk_widget_width(app_wins[APP_WIN_0]);
	height = ftk_widget_height(app_wins[APP_WIN_0]);
	wait_box = ftk_wait_box_create(app_wins[APP_WIN_0], 400, 240);
	ftk_widget_set_id(wait_box,WAIT_BOX_ID);
	gc.mask = FTK_GC_BITMAP;

	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "/opt/PBAPPS/image/desktop1.png");
	ftk_widget_set_gc(app_wins[APP_WIN_0],FTK_WIDGET_NORMAL, &gc);
	ftk_bitmap_unref(gc.bitmap);
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "/opt/PBAPPS/image/desktop1.png");
	ftk_widget_set_gc(app_wins[APP_WIN_0], FTK_WIDGET_FOCUSED, &gc);
	ftk_bitmap_unref(gc.bitmap);
	/*
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "/opt/PBAPPS/image/desktop2.png");
    ftk_widget_set_gc(app_wins[APP_WIN_0], FTK_WIDGET_ACTIVE, &gc);
    ftk_bitmap_unref(gc.bitmap);
    gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "/opt/PBAPPS/image/desktop2.png");
	ftk_widget_set_gc(app_wins[APP_WIN_0], FTK_WIDGET_INSENSITIVE, &gc);
	ftk_bitmap_unref(gc.bitmap);
	*/
	printf("path is %s\n",ftk_config_get_test_data_dir(ftk_default_config()));
	ret = ftk_fs_change_dir("/opt/PBAPPS/app/");
	if(ret == RET_OK){
		printf("fs_change_success\n");
	}
	check_dir("/opt/PBAPPS/app/",&app_total);
	printf("total is %d\n",app_total);
	if(1)
	{
		//ftk_widget_set_text(label,"empty file");
		gc.mask = FTK_GC_BITMAP;
		gc.bitmap = ftk_bitmap_factory_load(
				ftk_default_bitmap_factory(),
				"/opt/PBAPPS/image/config1.png");
		button = ftk_button_create(app_wins[APP_WIN_0], 5 * w, 2 * h + 5, ftk_bitmap_width(gc.bitmap), ftk_bitmap_height(gc.bitmap));
		ftk_widget_set_gc(button, FTK_WIDGET_NORMAL, &gc);
		ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
		ftk_bitmap_unref(gc.bitmap);
		gc.mask = FTK_GC_BITMAP;
		gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(),
				"/opt/PBAPPS/image/config2.png");
		ftk_widget_set_gc(button, FTK_WIDGET_ACTIVE, &gc);
		ftk_widget_set_gc(button, FTK_WIDGET_INSENSITIVE, &gc);
		ftk_bitmap_unref(gc.bitmap);
		ftk_button_set_clicked_listener(button, button_clicked, app_wins[APP_WIN_0]);
		label = ftk_label_create(app_wins[APP_WIN_0], 5 * w, 2 * h + 128, 128, 40);
		ftk_label_set_alignment(label, FTK_ALIGN_CENTER);
		__ftk_widget_set_font(label, PB62_SMALL_FONT);
		gc.mask = FTK_GC_FG;
		gc.fg.a = 0x00;
		gc.fg.r = 0x30;
		gc.fg.g = 0x34;
		gc.fg.b = 0x23;
		ftk_widget_set_gc(label, FTK_WIDGET_NORMAL, &gc);
		ftk_widget_set_gc(label, FTK_WIDGET_FOCUSED, &gc);
		ftk_widget_set_gc(label, FTK_WIDGET_ACTIVE, &gc);
		ftk_widget_set_gc(label, FTK_WIDGET_INSENSITIVE, &gc);
		/*
		gc.mask = FTK_GC_BG;
		gc.bg.a = 0x00;
		gc.bg.r = BG_COLOR_R;
		gc.bg.g = BG_COLOR_G;
		gc.bg.b = BG_COLOR_B;
		ftk_widget_set_gc(app_wins[APP_WIN_0], FTK_WIDGET_NORMAL, &gc);
		ftk_widget_set_gc(app_wins[APP_WIN_0], FTK_WIDGET_FOCUSED, &gc);
		ftk_widget_set_gc(app_wins[APP_WIN_0], FTK_WIDGET_ACTIVE, &gc);
		ftk_widget_set_gc(app_wins[APP_WIN_0], FTK_WIDGET_INSENSITIVE, &gc);
		*/
		ftk_widget_set_text(label, "Config");
		//continue;
		//break;
	}
	for(i = 0; i < height/h; i++)
	{
		for(j = 0; j < width/w; j++)
		{
			if((k >= (app_total - 1))||(k == 17))break;
			//snprintf(filename, sizeof(filename), "%s%s",
			//	ftk_config_get_test_data_dir(ftk_default_config()), pngs[rand()%39]);
			//srand(t->tm_sec);
			//if(i == 2 && j == 5)
			clock_gettime(CLOCK_REALTIME,&nano);
			srand((int)(nano.tv_nsec));
			sprintf(temp_filename,"%d%s",rand()%48,".png");
			//sprintf(filename,"%s%s","/opt/PBAPPS/app/WOW2/",wow[rand()%21]);
			sprintf(filename,"%s%s","/opt/PBAPPS/image/",temp_filename);
			//printf("i+2 = %d,j = %d,rand is %d\n",i+2,j,rand()%40);
			//rand();
			//memcpy(filename,pngs[i],sizeof(pngs[i]));
			printf("filename is %s\n",filename);
			gc.mask = FTK_GC_BITMAP;
			gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), filename);
			ftk_widget_set_gc(button, FTK_WIDGET_ACTIVE, &gc);
			ftk_bitmap_unref(gc.bitmap);
			gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), filename);
			button = ftk_button_create(app_wins[APP_WIN_0], j * w, i * h + 5, ftk_bitmap_width(gc.bitmap), ftk_bitmap_height(gc.bitmap));
			ftk_widget_set_gc(button, FTK_WIDGET_NORMAL, &gc);
			ftk_bitmap_unref(gc.bitmap);
			//snprintf(filename, sizeof(filename), "%s%s",
			//	ftk_config_get_test_data_dir(ftk_default_config()), pngs[rand()%39]);
            //sprintf(filename,"%s%s","/opt/PBAPPS/app/WOW1/",wow[rand()%21]);
            sprintf(temp_filename,"%d%s",rand()%48,".png");
            sprintf(filename,"%s%s","/opt/PBAPPS/image/",temp_filename);
			gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), filename);
			ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
			ftk_bitmap_unref(gc.bitmap);
			//snprintf(filename, sizeof(filename), "%s%s",
			//	ftk_config_get_test_data_dir(ftk_default_config()), pngs[rand()%39]);
            //sprintf(filename,"%s%s","/opt/PBAPPS/app/WOW1/",wow[rand()%21]);
            sprintf(temp_filename,"%d%s",rand()%48,".png");
            sprintf(filename,"%s%s","/opt/PBAPPS/image/",temp_filename);
            label = ftk_label_create(app_wins[APP_WIN_0],j * w,i * h + 128,128,40);
			ftk_label_set_alignment(label, FTK_ALIGN_CENTER);
			__ftk_widget_set_font(label, PB62_SMALL_FONT);
			gc.mask = FTK_GC_FG;
			gc.fg.a = 0x00;
			gc.fg.r = 0x30;
			gc.fg.g = 0x34;
			gc.fg.b = 0x23;
			ftk_widget_set_gc(label, FTK_WIDGET_NORMAL, &gc);
			ftk_widget_set_gc(label, FTK_WIDGET_FOCUSED, &gc);
			ftk_widget_set_gc(label, FTK_WIDGET_ACTIVE, &gc);
			ftk_widget_set_gc(label, FTK_WIDGET_INSENSITIVE, &gc);
			gc.mask = FTK_GC_BG;
			gc.bg.a = 0x00;
			gc.bg.r = BG_COLOR_R;
			gc.bg.g = BG_COLOR_G;
			gc.bg.b = BG_COLOR_B;
			/*
			ftk_widget_set_gc(win, FTK_WIDGET_NORMAL, &gc);
			ftk_widget_set_gc(win, FTK_WIDGET_FOCUSED, &gc);
			ftk_widget_set_gc(win, FTK_WIDGET_ACTIVE, &gc);
			ftk_widget_set_gc(win, FTK_WIDGET_INSENSITIVE, &gc);
			*/
			for(m = last;m < 99;m++){
				sprintf(appname,"%s%d","app",m);
			    if(ftk_file_exist(appname)){
			    	app_number++;
			    	printf("app is exist!\n");
			    	sprintf(cmd,"%s%s","chmod 777 ",appname);
			    	printf("cmd is %s\n",cmd);
			    	system(cmd);
			    	size =  ftk_file_length(appname);
			    	printf("size is %d\n",size);
			    	ret = ftk_file_get_info(appname,&file_info);
					if(ret == RET_OK){
						printf("size is %d,time is %d,mode is %d,last is %d\n",file_info.size,
								file_info.last_modify,file_info.mode,file_info.last_access);
					}
					temptm = *localtime(&file_info.last_modify);
					printf("time is %d,%d,%d,%d,%d,%d",temptm.tm_year+1900,temptm.tm_mon,temptm.tm_yday,temptm.tm_wday,temptm.tm_hour,temptm.tm_sec);
			    	//ftk_widget_set_text(button,appname);
			    	memset(userdata.file,0,sizeof(userdata.file));
			    	memcpy(userdata.file,appname,strlen(appname));
			    	printf("file is %s,name is %s,png is %s\n",userdata.file,appname,pngs[m]);
			    	ftk_widget_set_user_data(button, dcc_user_data_destroy,&userdata);
			    	ftk_button_set_clicked_listener(button, app_clicked,pngs[m]);
			    	ftk_widget_set_text(label,appname);
			    	last = m+1;
			    	break;
			    }
			    else {
			    	//ftk_widget_set_text(label,"empty file");
			    	//ftk_widget_set_id(label,k+20);
			    }
			}
			ftk_gc_reset(&gc);
			//ftk_widget_set_text(button,appname);
			ftk_widget_set_id(button,k);
			ftk_widget_set_id(label,k+20);
			//ftk_widget_set_text(button,"who");
			k++;
		}
		//k++;
			//break;
	}
	printf("k = %d\n",k);
	ftk_widget_set_text(app_wins[APP_WIN_0], "image button demo");
	//ftk_gc_reset(&gc);
	ftk_widget_show_all(app_wins[APP_WIN_0], 1);
	ftk_widget_show(wait_box, 0);
	check_dir("/opt/PBAPPS/app/",&app_number);
	printf("there are %d app\n",app_number);
	if((app_number - 1) > 17){
		ftk_tips("notic: \nthere are too many app,please delete some one\n");
		//check_dir("/opt/PBAPPS/app/");
	}
	//ftk_widget_set_attr(win, FTK_ATTR_IGNORE_CLOSE);
	timer = ftk_source_timer_create(1000, update_time,app_wins[APP_WIN_0]);
    ftk_main_loop_add_source(ftk_default_main_loop(), timer);
	FTK_QUIT_WHEN_WIDGET_CLOSE(app_wins[APP_WIN_0]);
	return 0;
}
/*
int win_download_creat(void)
{
	int i = 0;
	int j = 0;
	int k = 1;
	int m = 0;
	static int last = 0;
	int w = 133;
	int h = 155;
	int width = 0;
	int height = 0;
	int size;
	Ret ret;
	char filename[FTK_MAX_PATH] = {0};
	char temp_filename[FTK_MAX_PATH] = {0};
	char appname[FTK_MAX_PATH] = {0};
	struct timespec nano = {0};
	struct tm temptm;
	FtkGc gc = {0};
	TimerInfo* info = NULL;
	FtkSource* timer = NULL;
	FtkWidget* win = NULL;
	FtkWidget* button = NULL;
	//FtkWidget* label = NULL;
	FtkFileInfo file_info;
	user_data_t userdata;
	//FTK_INIT(argc, argv);
	info = (TimerInfo*)FTK_ZALLOC(sizeof(TimerInfo));
	info->times = 200;
	//timer = ftk_source_timer_create(1000, timeout, info);
	gc.mask = FTK_GC_BITMAP;
	win = ftk_app_window_create();
	ftk_window_set_animation_hint(win, "app_main_window");
	width = ftk_widget_width(win);
	height = ftk_widget_height(win);
	gc.mask = FTK_GC_BITMAP;

	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "/opt/PBAPPS/image/qian1.png");
	ftk_widget_set_gc(win, FTK_WIDGET_NORMAL, &gc);
	ftk_bitmap_unref(gc.bitmap);
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "/opt/PBAPPS/image/qian1.png");
	ftk_widget_set_gc(win, FTK_WIDGET_FOCUSED, &gc);
	ftk_bitmap_unref(gc.bitmap);
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "/opt/PBAPPS/image/qian2.png");
    ftk_widget_set_gc(win, FTK_WIDGET_ACTIVE, &gc);
    ftk_bitmap_unref(gc.bitmap);
    gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "/opt/PBAPPS/image/qian2.png");
	ftk_widget_set_gc(win, FTK_WIDGET_INSENSITIVE, &gc);
	ftk_bitmap_unref(gc.bitmap);
	printf("path is %s\n",ftk_config_get_test_data_dir(ftk_default_config()));
	ret = ftk_fs_change_dir("/opt/PBAPPS/app/");
	if(ret == RET_OK){
		printf("fs_change_success\n");
	}
	for(i = 0; i < height/h; i++)
	{
		for(j = 0; j < width/w; j++)
		{
			//snprintf(filename, sizeof(filename), "%s%s",
			//	ftk_config_get_test_data_dir(ftk_default_config()), pngs[rand()%39]);
			//srand(t->tm_sec);
			button = NULL;
			if(i == 2 && j == 5)
			{
				//ftk_widget_set_text(label,"empty file");
				gc.mask = FTK_GC_BITMAP;
				gc.bitmap = ftk_bitmap_factory_load(
						ftk_default_bitmap_factory(),
						"/opt/PBAPPS/image/config1.png");
				button = ftk_button_create(win, j * w, i * h + 5, ftk_bitmap_width(gc.bitmap), ftk_bitmap_height(gc.bitmap));
				ftk_widget_set_gc(button, FTK_WIDGET_NORMAL, &gc);
				ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
				ftk_bitmap_unref(gc.bitmap);
				gc.mask = FTK_GC_BITMAP;
				gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(),
						"/opt/PBAPPS/image/config2.png");
				ftk_widget_set_gc(button, FTK_WIDGET_ACTIVE, &gc);
				ftk_widget_set_gc(button, FTK_WIDGET_INSENSITIVE, &gc);
				ftk_bitmap_unref(gc.bitmap);
				ftk_button_set_clicked_listener(button, button_clicked, win);
				main_label[i * 6 + j] = ftk_label_create(win, j * w, i * h + 128, 128, 40);
				ftk_label_set_alignment(main_label[i * 6 + j], FTK_ALIGN_CENTER);
				__ftk_widget_set_font(main_label[i * 6 + j], PB62_SMALL_FONT);
				gc.mask = FTK_GC_FG;
				gc.fg.a = 0x00;
				gc.fg.r = 0x30;
				gc.fg.g = 0x34;
				gc.fg.b = 0x23;
				ftk_widget_set_gc(main_label[i * 6 + j], FTK_WIDGET_NORMAL, &gc);
				ftk_widget_set_gc(main_label[i * 6 + j], FTK_WIDGET_FOCUSED, &gc);
				ftk_widget_set_gc(main_label[i * 6 + j], FTK_WIDGET_ACTIVE, &gc);
				ftk_widget_set_gc(main_label[i * 6 + j], FTK_WIDGET_INSENSITIVE, &gc);
				gc.mask = FTK_GC_BG;
				gc.bg.a = 0x00;
				gc.bg.r = BG_COLOR_R;
				gc.bg.g = BG_COLOR_G;
				gc.bg.b = BG_COLOR_B;
				ftk_widget_set_gc(win, FTK_WIDGET_NORMAL, &gc);
				ftk_widget_set_gc(win, FTK_WIDGET_FOCUSED, &gc);
				ftk_widget_set_gc(win, FTK_WIDGET_ACTIVE, &gc);
				ftk_widget_set_gc(win, FTK_WIDGET_INSENSITIVE, &gc);
				ftk_widget_set_text(main_label[i * 6 + j], "Config");
				continue;
			}
			clock_gettime(CLOCK_REALTIME,&nano);
			srand((int)(nano.tv_nsec));
			sprintf(temp_filename,"%d%s",rand()%48,".png");
			//sprintf(filename,"%s%s","/opt/PBAPPS/app/WOW2/",wow[rand()%21]);
			sprintf(filename,"%s%s","/opt/PBAPPS/image/",temp_filename);
			//printf("i+2 = %d,j = %d,rand is %d\n",i+2,j,rand()%40);
			//rand();
			//memcpy(filename,pngs[i],sizeof(pngs[i]));
			printf("filename is %s\n",filename);
			gc.mask = FTK_GC_BITMAP;
			gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), filename);
			ftk_widget_set_gc(button, FTK_WIDGET_ACTIVE, &gc);
			ftk_bitmap_unref(gc.bitmap);
			gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), filename);
			button = ftk_button_create(win, j * w, i * h + 5, ftk_bitmap_width(gc.bitmap), ftk_bitmap_height(gc.bitmap));
			ftk_widget_set_gc(button, FTK_WIDGET_NORMAL, &gc);
			ftk_bitmap_unref(gc.bitmap);
			//snprintf(filename, sizeof(filename), "%s%s",
			//	ftk_config_get_test_data_dir(ftk_default_config()), pngs[rand()%39]);
            //sprintf(filename,"%s%s","/opt/PBAPPS/app/WOW1/",wow[rand()%21]);
            sprintf(temp_filename,"%d%s",rand()%48,".png");
            sprintf(filename,"%s%s","/opt/PBAPPS/image/",temp_filename);
			gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), filename);
			ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
			ftk_bitmap_unref(gc.bitmap);
			//snprintf(filename, sizeof(filename), "%s%s",
			//	ftk_config_get_test_data_dir(ftk_default_config()), pngs[rand()%39]);
            //sprintf(filename,"%s%s","/opt/PBAPPS/app/WOW1/",wow[rand()%21]);
            sprintf(temp_filename,"%d%s",rand()%48,".png");
            sprintf(filename,"%s%s","/opt/PBAPPS/image/",temp_filename);
            main_label[i * 6 + j] = ftk_label_create(win,j * w,i * h + 128,128,40);
			ftk_label_set_alignment(main_label[i * 6 + j], FTK_ALIGN_CENTER);
			__ftk_widget_set_font(main_label[i * 6 + j], PB62_SMALL_FONT);
			gc.mask = FTK_GC_FG;
			gc.fg.a = 0x00;
			gc.fg.r = 0x30;
			gc.fg.g = 0x34;
			gc.fg.b = 0x23;
			ftk_widget_set_gc(main_label[i * 6 + j], FTK_WIDGET_NORMAL, &gc);
			ftk_widget_set_gc(main_label[i * 6 + j], FTK_WIDGET_FOCUSED, &gc);
			ftk_widget_set_gc(main_label[i * 6 + j], FTK_WIDGET_ACTIVE, &gc);
			ftk_widget_set_gc(main_label[i * 6 + j], FTK_WIDGET_INSENSITIVE, &gc);
			gc.mask = FTK_GC_BG;
			gc.bg.a = 0x00;
			gc.bg.r = BG_COLOR_R;
			gc.bg.g = BG_COLOR_G;
			gc.bg.b = BG_COLOR_B;
			ftk_widget_set_gc(win, FTK_WIDGET_NORMAL, &gc);
			ftk_widget_set_gc(win, FTK_WIDGET_FOCUSED, &gc);
			ftk_widget_set_gc(win, FTK_WIDGET_ACTIVE, &gc);
			ftk_widget_set_gc(win, FTK_WIDGET_INSENSITIVE, &gc);
			for(m = last;m < 99;m++){
				sprintf(appname,"%s%d","app",m);
			    if(ftk_file_exist(appname)){
			    	printf("app is exist!\n");
			    	size =  ftk_file_length(appname);
			    	printf("size is %d\n",size);
			    	ret = ftk_file_get_info(appname,&file_info);
					if(ret == RET_OK){
						printf("size is %d,time is %d,mode is %d,last is %d\n",file_info.size,
								file_info.last_modify,file_info.mode,file_info.last_access);
					}
					temptm = *localtime(&file_info.last_modify);
					printf("time is %d,%d,%d,%d,%d,%d",temptm.tm_year+1900,temptm.tm_mon,temptm.tm_yday,temptm.tm_wday,temptm.tm_hour,temptm.tm_sec);
			    	//ftk_widget_set_text(button,appname);
			    	memset(userdata.file,0,sizeof(userdata.file));
			    	memcpy(userdata.file,appname,strlen(appname));
			    	printf("file is %s,name is %s,png is %s\n",userdata.file,appname,pngs[m]);
			    	ftk_widget_set_user_data(button, dcc_user_data_destroy,&userdata);
			    	ftk_button_set_clicked_listener(button, app_clicked,pngs[m]);
			    	ftk_widget_set_text(main_label[i * 6 + j],appname);
			    	last = m+1;
			    	break;
			    }
			    else ftk_widget_set_text(main_label[i * 6 + j],"empty file");
			}
			ftk_gc_reset(&gc);
			//ftk_widget_set_text(button,appname);

			ftk_widget_set_id(button,k);
			//ftk_widget_set_text(button,"who");
			k++;
		}
	}
	printf("k = %d\n",k);
	ftk_widget_set_text(win, "image button demo");
	//ftk_gc_reset(&gc);
	ftk_widget_show_all(win, 1);
	//ftk_widget_set_attr(win, FTK_ATTR_IGNORE_CLOSE);
	timer = ftk_source_timer_create(1000, update_time,win);
    ftk_main_loop_add_source(ftk_default_main_loop(), timer);
	FTK_QUIT_WHEN_WIDGET_CLOSE(win);
	return 0;
}
*/
