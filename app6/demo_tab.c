#include <unistd.h>
#include "ftk.h"
#include "ftk_tab.h"
#include "main.h"
#include "network.h"
#define PLAY_ID     100
int num[15] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13};
char current_path[100];
FtkWidget* label_ip = NULL;
struct net_addr network_addr;
FtkWidget* ip_button[12];
FtkWidget* net_button[6];
FtkWidget* type_button;
static const char* buttons_ok[] = {"Confirm","Cancel", NULL};
static int count= 0;
//static Net_type netwrok_type = IP;
Net_type network_type = IP;
//static char net_addres[15];
int str(char *ptr)
{
   int in=0;
   int i,j;
   //char buffer[INFO_MAX_SZ]="Fred male 25,John male 62,Anna female 16";
   char *p[20];
   char *buf=ptr;

   while((p[in]=strtok(buf,"/"))!=NULL) {
             buf=p[in];
             while((p[in]=strtok(buf," "))!=NULL) {
                       in++;
                       buf=NULL;
                    }
                 p[in++]="***";
                 buf=NULL; }

   printf("Here we have %d strings/n",in);
   for (j=0; j<in; j++)
         printf(">%s</n",p[j]);
   return 0;
}
static Ret button_default_clicked(void* ctx, void* obj)
{
	printf("%s: button %s is clicked.\n", __func__, ftk_widget_get_text(obj));
	return RET_OK;
}
static Ret button_movie1_clicked(void* ctx, void* obj)
{
	//printf("%s: button %s is clicked.\n", __func__, ftk_widget_get_text(obj));
	//player();
	int ret = 0;
	FtkWidget* button = NULL;
	const char* text;
	button = ftk_widget_lookup(ctx,PLAY_ID);
	text = ftk_widget_get_text(button);
	printf("text is %s\n",text);
	player(50,100,"/opt/PBAPPS/movie/movie1.dat");
	printf("finished\n");

	return RET_OK;
}
static Ret button_movie2_clicked(void* ctx, void* obj)
{
	//printf("%s: button %s is clicked.\n", __func__, ftk_widget_get_text(obj));
	FtkWidget* button = NULL;
	const char* text;
	button = ftk_widget_lookup(ctx,PLAY_ID);
	text = ftk_widget_get_text(button);
	printf("text is %s\n",text);
	player(450,100,"/opt/PBAPPS/movie/movie2.dat");
	printf("finished\n");
	return RET_OK;
}
static int address_check(char *res,char *tar){
	int i,j,k;
	for(i = 0;i < 4;i++)
	{
		/*
		for (j = 0; j < 3; j++) {
			k = addr[i * 3 + j] * 100 + addr[i * 3 + j + 1] * 10
					+ addr[i * 3 + j + 2];
		}
		*/
		k = res[i * 3] * 100 + res[i * 3 + 1] * 10
							+ res[i * 3 + 2];
		printf("%d.", k);
		if(k > 255){
			printf("invalid address\n");
			return -1;
		}
		tar[i] = k;
	}
	printf("vailid address\n");
	return 0;
}
static Ret button_ip_setting_clicked(void* ctx, void* obj)
{
	FtkGc gc = {0};
	int i;
	printf("clicked\n");
	network_type = IP;
	for(i = 0;i < 12;i++){
		gc.mask = FTK_GC_BITMAP;
		gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(),"/opt/PBAPPS/image/focus/empty.png");
		ftk_widget_set_gc(ip_button[i], FTK_WIDGET_NORMAL, &gc);
		//ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
		//ftk_widget_set_gc(ip_button[count], FTK_WIDGET_ACTIVE, &gc);
		//ftk_widget_set_gc(ip_button[count], FTK_WIDGET_INSENSITIVE, &gc);
		ftk_bitmap_unref(gc.bitmap);
		//ftk_widget_update(ip_button[count]);
		//ftk_widget_paint_self(ip_button[i]);
		ftk_widget_show(ip_button[i],0);
		ftk_widget_show(ip_button[i],1);
	}
	/*
	__ftk_widget_set_font(type_button,  "size:24 bold:1 italic:0");
	gc.mask = FTK_GC_FG;
	gc.fg.a = 0x00;
	gc.fg.r = 0x30;
	gc.fg.g = 0x34;
	gc.fg.b = 0x23;
	ftk_widget_set_gc(type_button, FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(type_button, FTK_WIDGET_FOCUSED, &gc);
	ftk_widget_set_gc(type_button, FTK_WIDGET_ACTIVE, &gc);
	ftk_widget_set_gc(type_button, FTK_WIDGET_INSENSITIVE, &gc);
	ftk_gc_reset(&gc);
	*/
	ftk_widget_set_text(type_button, "IP");
	count= 0;
	return RET_OK;
}
static Ret button_broadcasting_setting_clicked(void* ctx, void* obj)
{
	FtkGc gc = {0};
	int i;
	printf("clicked\n");
	network_type = BROAD;
	for(i = 0;i < 12;i++){
		gc.mask = FTK_GC_BITMAP;
		gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(),"/opt/PBAPPS/image/focus/empty.png");
		ftk_widget_set_gc(ip_button[i], FTK_WIDGET_NORMAL, &gc);
		//ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
		//ftk_widget_set_gc(ip_button[count], FTK_WIDGET_ACTIVE, &gc);
		//ftk_widget_set_gc(ip_button[count], FTK_WIDGET_INSENSITIVE, &gc);
		ftk_bitmap_unref(gc.bitmap);
		//ftk_widget_update(ip_button[count]);
		//ftk_widget_paint_self(ip_button[i]);
		ftk_widget_show(ip_button[i],0);
		ftk_widget_show(ip_button[i],1);
	}
	ftk_widget_set_text(type_button, "broad");
	count= 0;
	return RET_OK;
}
static Ret button_gateway_setting_clicked(void* ctx, void* obj)
{
	FtkGc gc = {0};
	int i;
	printf("clicked\n");
	network_type = GATE;
	for(i = 0;i < 12;i++){
		gc.mask = FTK_GC_BITMAP;
		gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(),"/opt/PBAPPS/image/focus/empty.png");
		ftk_widget_set_gc(ip_button[i], FTK_WIDGET_NORMAL, &gc);
		//ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
		//ftk_widget_set_gc(ip_button[count], FTK_WIDGET_ACTIVE, &gc);
		//ftk_widget_set_gc(ip_button[count], FTK_WIDGET_INSENSITIVE, &gc);
		ftk_bitmap_unref(gc.bitmap);
		//ftk_widget_update(ip_button[count]);
		//ftk_widget_paint_self(ip_button[i]);
		ftk_widget_show(ip_button[i],0);
		ftk_widget_show(ip_button[i],1);
	}
	ftk_widget_set_text(type_button, "gateway");
	count= 0;
	return RET_OK;
}static Ret button_mask_setting_clicked(void* ctx, void* obj)
{
	FtkGc gc = {0};
	int i;
	printf("clicked\n");
	network_type = MASK;
	for(i = 0;i < 12;i++){
		gc.mask = FTK_GC_BITMAP;
		gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(),"/opt/PBAPPS/image/focus/empty.png");
		ftk_widget_set_gc(ip_button[i], FTK_WIDGET_NORMAL, &gc);
		//ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
		//ftk_widget_set_gc(ip_button[count], FTK_WIDGET_ACTIVE, &gc);
		//ftk_widget_set_gc(ip_button[count], FTK_WIDGET_INSENSITIVE, &gc);
		ftk_bitmap_unref(gc.bitmap);
		//ftk_widget_update(ip_button[count]);
		//ftk_widget_paint_self(ip_button[i]);
		ftk_widget_show(ip_button[i],0);
		ftk_widget_show(ip_button[i],1);
	}
	ftk_widget_set_text(type_button, "mask");
	count= 0;
	return RET_OK;
}
static Ret on_file_selected(void* ctx, int index, const char* path)
{
	//ftk_logd("%s: [%d] %s\n", __func__, index, path);
	//char ptr[20][100];
	int num;
	char *ptr = NULL;
	char des[100];
	char *result;
	char temp[100];
	//str(path);
	memset(current_path,0,sizeof(current_path));
	getcwd(current_path,sizeof(current_path));
	printf("path is %s\n",current_path);
    printf("%s: [%d] %s\n", __func__, index, path);
    memset(temp,0,sizeof(temp));
    memcpy(temp,path,strlen(path));
    result = strtok(path,"/");
    while( result != NULL ) {
	   printf( "result is \"%s\"\n",result);
	   result = strtok(NULL, "/" );
	   num = strlen(result);
    }
    memset(des,0,sizeof(des));
    strncpy(des,temp,strlen(temp));
    printf("des is %s\n",des);
    //printf("path is %s,result is \n",temp,result);
    //printf("path is %s,result is \n",strlen(temp),strlen(result));
    //printf("path is %s\n",ptr);
	//strtok_r(path,"/",&ptr);
	//printf("str is %s,%s,%s,%s\n",ptr,ptr+1,ptr+2,ptr+3);
	return RET_OK;
}
static Ret button_single_choose_clicked(void* ctx, void* obj)
{
	FtkWidget* win = ftk_file_browser_create(FTK_FILE_BROWER_SINGLE_CHOOSER);
	ftk_file_browser_set_choosed_handler(win, on_file_selected, NULL);
	ftk_file_browser_set_path(win, "./");
	ftk_file_browser_load(win);
	return RET_OK;
}
static Ret button_ip_clicked(void* ctx, void* obj)
{
	//printf("%s: button %s is clicked.\n", __func__, ftk_widget_get_text(obj));
	int *n;
	int i,ret;
	char m[100];
	char addr[100];
	char gate[100];
	char ip_text[100] = "               ";
	char info[100];
	char net[100];
	//char defalt_net[100] = "000.000.000.000";
	char net_type[100];
	char net_info[100];
	static char net_addres[15];
	static char target[4];
	 const char *_buttons[2] = {
	        "OK", NULL
	    };
	FtkGc gc = {0};
	n = (int *)ctx;
	i = *n;
	//memcpy(&n,ctx,sizeof(n));
    printf("%d\n",i);
    if((i<10)&&(count<12)){
    sprintf(m,"%s%d%s","/opt/PBAPPS/image/focus/",i,".png");
    //net_test(&network_addr);
    //GetGateWay(gate);
    gc.mask = FTK_GC_BITMAP;
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(),m);
	//ip_button[i] = ftk_button_create(ip_button[*n], 20 + i * 50, 240,
	//		ftk_bitmap_width(gc.bitmap), ftk_bitmap_height(gc.bitmap));
	ftk_widget_set_gc(ip_button[count], FTK_WIDGET_NORMAL, &gc);
	//ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
	ftk_widget_set_gc(ip_button[count], FTK_WIDGET_ACTIVE, &gc);
	ftk_widget_set_gc(ip_button[count], FTK_WIDGET_INSENSITIVE, &gc);
	ftk_bitmap_unref(gc.bitmap);
	ftk_widget_update(ip_button[count]);
	//ftk_widget_paint_self(ip_button[i]);
	ftk_widget_show(ip_button[count],0);
	ftk_widget_show(ip_button[count],1);
	net_addres[count] = i;
	count++;
    }
	//if(!memcmp(n,"del",sizeof("del"))){
	if(i==10){
		//ftk_bitmap_unref(gc.bitmap);
		//count--;
		if(count == 12){
			count--;
			gc.mask = FTK_GC_BITMAP;
			gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(),"/opt/PBAPPS/image/focus/empty.png");
			ftk_widget_set_gc(ip_button[count], FTK_WIDGET_NORMAL, &gc);
			//ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
			ftk_widget_set_gc(ip_button[count], FTK_WIDGET_ACTIVE, &gc);
			ftk_widget_set_gc(ip_button[count], FTK_WIDGET_INSENSITIVE, &gc);
			ftk_bitmap_unref(gc.bitmap);
			ftk_widget_update(ip_button[count]);
			//ftk_widget_paint_self(ip_button[i]);
			ftk_widget_show(ip_button[count],0);
			ftk_widget_show(ip_button[count],1);
			net_addres[count] = 0;
		}
		else{
			count--;
			gc.mask = FTK_GC_BITMAP;
			gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(),"/opt/PBAPPS/image/focus/empty.png");
			ftk_widget_set_gc(ip_button[count], FTK_WIDGET_NORMAL, &gc);
			//ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
			ftk_widget_set_gc(ip_button[count], FTK_WIDGET_ACTIVE, &gc);
			ftk_widget_set_gc(ip_button[count], FTK_WIDGET_INSENSITIVE, &gc);
			ftk_bitmap_unref(gc.bitmap);
			ftk_widget_update(ip_button[count]);
			//ftk_widget_paint_self(ip_button[i]);
			ftk_widget_show(ip_button[count],0);
			ftk_widget_show(ip_button[count],1);
			net_addres[count] = 0;
		}
	}
	if(i==11){
		net_addres[count] = 0;
		count++;
	}
	if(i==12){
		printf("ok\n");
		printf("count = %d\n",count);
		memset(ip_text,0,sizeof(ip_text));
		GetLocalIp(ip_text);
		if(count < 9){
			ftk_warning("warning", "please input a complete address!", _buttons);
			return RET_OK;
		}
		ret = address_check(net_addres,target);
		if(ret < 0){
			ftk_warning("warning", "Invalid address!", _buttons);
			return RET_OK;
		}
		sprintf(ip_text,"%d.%d.%d.%d",target[0],target[1],target[2],target[3]);
		printf("ip is %s\n",ip_text);
		switch(network_type){
		case IP:memcpy(net_type,"IP address",sizeof("IP address"));break;
		case GATE:memcpy(net_type,"gateway",sizeof("gateway"));break;
		case BROAD:memcpy(net_type,"broadcasting",sizeof("broadcasting"));break;
		case MASK:memcpy(net_type,"net mask",sizeof("net mask"));break;
		}
		sprintf(info,"%s%s%s%s%s","Do you want to set ",ip_text," as ",net_type,"?");
        sprintf(net_info,"%s%s",net_type," setting");
		ret = ftk_question(net_info,info,buttons_ok);
		printf("i = %d\n",i);
		if(ret==1){
		   printf("confirm\n");
		   //SetLocalIp(ip_text);
		   net_setting(network_type,ip_text);
		   ftk_tips("please waiting for a second");
		   //sleep(1);
		   net_test(&network_addr);
		   GetGateWay(gate);
		   switch(network_type){
			case IP:
				if(memcmp(ip_text,network_addr.ip,strlen(ip_text))==0){
					//ftk_tips("configure success!");
					//GetLocalIp(ip_text);
					memset(net,0,sizeof(net));
					//sprintf(net,"%s:%s","netmask",network_addr.mask);
					//ftk_button_set_clicked_listener(net_button[3], button_mask_setting_clicked, net);
					sprintf(net,"%s:%s","IP",ip_text);
					ftk_widget_set_text(net_button[0],net);
					ftk_widget_show(net_button[0],0);
					ftk_widget_show(net_button[0],1);
				}
				else{
					/*
					memset(net,0,sizeof(net));
					sprintf(net,"%s:%s","IP","       ");
					ftk_widget_set_text(net_button[3],net);
					ftk_widget_show(net_button[0],0);
					ftk_widget_show(net_button[0],1);
					*/
				}
				break;
			case GATE:
				if(memcmp(ip_text,gate,strlen(ip_text))==0){
					//ftk_tips("configure success!");
					memset(net,0,sizeof(net));
					sprintf(net,"%s:%s","gateway",ip_text);
					ftk_widget_set_text(net_button[1],net);
					ftk_widget_show(net_button[1],0);
					ftk_widget_show(net_button[1],1);
				}
				else{
					/*
					memset(net,0,sizeof(net));
					sprintf(net,"%s:%s","gateway","       ");
					ftk_widget_set_text(net_button[3],net);
					ftk_widget_show(net_button[1],0);
					ftk_widget_show(net_button[1],1);
					*/
				}
				break;
			case BROAD:
				if(memcmp(ip_text,network_addr.gate,strlen(ip_text))==0){
					//ftk_tips("configure success!");
					memset(net,0,sizeof(net));
					sprintf(net,"%s:%s","broad",ip_text);
					ftk_widget_set_text(net_button[2],net);
					ftk_widget_show(net_button[2],0);
					ftk_widget_show(net_button[2],1);
				}
				else{
					/*
					memset(net,0,sizeof(net));
					sprintf(net,"%s:%s","broad","       ");
					ftk_widget_set_text(net_button[3],net);
					ftk_widget_show(net_button[2],0);
					ftk_widget_show(net_button[2],1);
					*/
				}
				break;
			case MASK:
				if(memcmp(ip_text,network_addr.mask,strlen(ip_text))==0){
					//ftk_tips("configure success!");
					memset(net,0,sizeof(net));
					sprintf(net,"%s:%s","netmask",ip_text);
					ftk_widget_set_text(net_button[3],net);
					ftk_widget_show(net_button[3],0);
					ftk_widget_show(net_button[3],1);
				}
				else{
					/*
					memset(net,0,sizeof(net));
					sprintf(net,"%s:%s","netmask","         ");
					ftk_widget_set_text(net_button[3],net);
					ftk_widget_show(net_button[3],0);
					ftk_widget_show(net_button[3],1);
					*/
				}
				break;
			}
		}
		else printf("cancel\n");
	}
	if(count>12)count = 12;
	if(count<0)count = 0;
	/*
    printf("ip is %s,gate is %s,mask is %s,mac is %02x-%02x-%02x-%02x-%02x-%02x\n",network_addr.ip,
    		network_addr.gate,network_addr.mask,network_addr.mac[0],network_addr.mac[1],network_addr.mac[2],
    		network_addr.mac[3],network_addr.mac[4],network_addr.mac[5]);
    */
    //ftk_widget_set_text(label_ip,m);
    //ftk_widget_show(label_ip,1);
    /*
    memset(addr,0,sizeof(addr));
    GetLocalIp(addr);
    printf("ip is %s\n",addr);
    printf("ascll is ");
    for(i = 0;i < strlen(addr);i++)
       printf("%d",addr[i]);
    */
	return RET_OK;
}
static Ret button_quit_clicked(void* ctx, void* obj)
{
	ftk_widget_unref(ctx);
	//printscreen_PNG();
	return RET_OK;
}
static Ret button_dialog_clicked(void* ctx, void* obj)
{
	int id = 0;
	int width = 0;
	int height = 0;
//	FtkWidget* label = NULL;
	FtkWidget* button = NULL;
	FtkWidget* dialog = NULL;
	FtkBitmap* icon = NULL;
	FtkWidget* combo_box = NULL;
	int modal = (int)ctx;

	ftk_logd("%s:%d begin\n", __func__, __LINE__);
	dialog = ftk_dialog_create(0, 40, 200, 240);

	icon = ftk_theme_load_image(ftk_default_theme(), "/opt/"FTK_STOCK_IMG_SUFFIX);
	ftk_dialog_set_icon(dialog, icon);

	width = ftk_widget_width(dialog);
	height = ftk_widget_height(dialog);
//	label = ftk_label_create(dialog, width/6, height/4, 5*width/6, 20);
//	ftk_widget_set_text(label, "Are you sure to quit?");

	combo_box = ftk_combo_box_create(dialog, width/6, height/4,150, 30);
	ftk_combo_box_set_text(combo_box, "1 second");
	ftk_combo_box_append(combo_box, NULL, "1 second");
	ftk_combo_box_append(combo_box, NULL, "2 seconds");
	ftk_combo_box_append(combo_box, NULL, "3 seconds");
	//ftk_entry_set_readonly(ftk_combo_box_get_entry(combo_box), modal);

	button = ftk_button_create(dialog, width/6, height/2, width/3, 50);
	ftk_widget_set_text(button, "yes");
	ftk_button_set_clicked_listener(button, button_quit_clicked, dialog);

	button = ftk_button_create(dialog, width/2, height/2, width/3, 50);
	ftk_widget_set_text(button, "no");
	//ftk_button_set_clicked_listener(button, button_quit_clicked, modal ? &id : NULL);
	ftk_button_set_clicked_listener(button, button_quit_clicked, dialog);
	ftk_window_set_focus(dialog, button);

	ftk_widget_set_text(dialog, modal ? "demo dialog" : "normal dialog");

	if(modal)
	{
		assert(ftk_dialog_run(dialog) == id);
		ftk_widget_unref(dialog);
	}
	else
	{
		ftk_widget_show_all(dialog, 1);
	}
	ftk_logd("%s:%d end\n", __func__, __LINE__);

	return RET_OK;
}

#ifdef FTK_AS_PLUGIN
#include "ftk_app_demo.h"
FTK_HIDE int FTK_MAIN(int argc, char* argv[]);
FtkApp* ftk_app_demo_tab_create()
{
	return ftk_app_demo_create(_("tab"), ftk_main);
}
#else
#define FTK_HIDE extern
#endif /*FTK_AS_PLUGIN*/

static void add_gen_page(FtkWidget* tab, const char* text, FtkBitmap* bitmap)
{
	int width = 0;
	int height = 0;
	FtkWidget* page = NULL;
	FtkWidget* button = NULL;
	char movie1[10] = "movie1";
	char movie2[10] = "movie2";

	page = ftk_tab_add_page(tab, text, bitmap);
	width = ftk_widget_width(page);
	height = ftk_widget_height(page);

	button = ftk_button_create(page, 170, 250, 80, 60);
	ftk_widget_set_text(button,movie1);
	ftk_widget_set_id(button,PLAY_ID);

	ftk_button_set_clicked_listener(button, button_movie1_clicked, tab);
	ftk_widget_show(button, 1);

	button = ftk_button_create(page, 580, 250, 80, 60);
	ftk_widget_set_text(button, movie2);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_movie2_clicked, tab);

	return;
}
static void add_file_page(FtkWidget* tab, const char* text, FtkBitmap* bitmap)
{
	int width = 0;
	int height = 0;
	FtkWidget* page = NULL;
	FtkWidget* button = NULL;
	FtkWidget* browser = NULL;
	char file[100];
	char path[100];
	getcwd(path,sizeof(path));
	sprintf(file,"%s%s","default image path is ",path);
	page = ftk_tab_add_page(tab, text, bitmap);
	width = ftk_widget_width(page);
	height = ftk_widget_height(page);
	/*
	browser = ftk_file_browser_create(FTK_FILE_BROWER_MULTI_CHOOSER);
	ftk_widget_set_parent(browser,page);
	//ftk_file_browser_set_choosed_handler(win, on_file_selected, NULL);
	ftk_file_browser_set_path(browser, "./");
	ftk_file_browser_load(browser);
    */
	button = ftk_button_create(page, 100, height/2-30-100, width/2+100, 60);
	memset(file,0,sizeof(file));
	sprintf(file,"%s%s","default image path is ",path);
	ftk_widget_set_text(button, file);
	ftk_widget_show(button, 1);
	//ftk_button_set_clicked_listener(button, button_default_clicked, tab);
	button = ftk_button_create(page, 610, height/2-30-100, 80, 60);
	ftk_widget_set_text(button, "....");
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_single_choose_clicked, page);
	
	button = ftk_button_create(page, 100, height/2-30, width/2+100, 60);
	//sprintf(file,"%s%s","default app path is ",path);
	memset(file,0,sizeof(file));
	sprintf(file,"%s%s","default app path is ",path);
	ftk_widget_set_text(button, file);
	ftk_widget_show(button, 1);
	button = ftk_button_create(page, 610, height/2-30, 80, 60);
	ftk_widget_set_text(button, "....");
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_single_choose_clicked, page);

	button = ftk_button_create(page, 100, height/2-30+100, width/2+100, 60);
	//sprintf(file,"%s%s","default app path is ",path);
	memset(file,0,sizeof(file));
	sprintf(file,"%s%s","default other path is ",path);
	ftk_widget_set_text(button, file);
	ftk_widget_show(button, 1);
	button = ftk_button_create(page, 610, height/2-30+100, 80, 60);
	ftk_widget_set_text(button, "....");
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_single_choose_clicked, page);

	return;
}
static void add_other_page(FtkWidget* tab, const char* text, FtkBitmap* bitmap)
{
	int width = 0;
	int height = 0;
	FtkWidget* page = NULL;
	FtkWidget* button = NULL;

	page = ftk_tab_add_page(tab, text, bitmap);
	width = ftk_widget_width(page);
	height = ftk_widget_height(page);

	button = ftk_button_create(page, 0, height/2-30, width/2, 60);
	ftk_widget_set_text(button, text);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_default_clicked, tab);

	button = ftk_button_create(page, width/2, height/2-30, width/2, 60);
	ftk_widget_set_text(button, text);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_dialog_clicked, tab);

	return;
}
static void add_IP_page(FtkWidget* tab, const char* text, FtkBitmap* bitmap)
{
	int width = 0;
	int height = 0;
	FtkWidget* page = NULL;
	FtkWidget* button = NULL;
	FtkWidget* label = NULL;
	//FtkBitmap* bitmap = NULL;
    FtkGc gc = {0};
    char number[100];
    char addr[100];
    char m[10];
    char gate[100];
    char net[100];
	int i,j;
	int w,h;
	int ret;

	page = ftk_tab_add_page(tab, text, bitmap);
	width = ftk_widget_width(page);
	height = ftk_widget_height(page);
    /*
	button = ftk_button_create(page, 0, height/2-30, width/2, 60);
	ftk_widget_set_text(button, text);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_default_clicked, tab);
	*/
	/*
	label_ip = ftk_label_create(tab,50,height / 2 -110, 128, 40);
	ftk_label_set_alignment(label_ip, FTK_ALIGN_CENTER);
	__ftk_widget_set_font(label_ip, PB62_DEFAULT_FONT);
	gc.mask = FTK_GC_FG;
	gc.fg.a = 0x00;
	gc.fg.r = 0x30;
	gc.fg.g = 0x34;
	gc.fg.b = 0x23;
	ftk_widget_set_gc(label_ip, FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(label_ip, FTK_WIDGET_FOCUSED, &gc);
	ftk_widget_set_gc(label_ip, FTK_WIDGET_ACTIVE, &gc);
	ftk_widget_set_gc(label_ip, FTK_WIDGET_INSENSITIVE, &gc);
	*/
	GetLocalIp(addr);
	net_test(&network_addr);
	GetGateWay(gate);
	//ip
	/*
	net_button[0] = ftk_label_create(tab,50,height / 2 -110, 300, 60);
	ftk_label_set_alignment(net_button[0], FTK_ALIGN_LEFT);
	gc.mask = FTK_GC_BITMAP;
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(),
			"/opt/PBAPPS/image/focus/back.png");
	*/
	net_button[0] = ftk_button_create(page,20,height / 2 -110 - 60, 370, 60);
	/*
	ftk_widget_set_gc(net_button[0], FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(net_button[0], FTK_WIDGET_FOCUSED, &gc);
	ftk_widget_set_gc(net_button[0], FTK_WIDGET_ACTIVE, &gc);
	ftk_widget_set_gc(net_button[0], FTK_WIDGET_INSENSITIVE,&gc);
	*/
	ftk_bitmap_unref(gc.bitmap);
	__ftk_widget_set_font(net_button[0], PB62_DEFAULT_FONT);
	gc.mask = FTK_GC_FG;
	gc.fg.a = 0x00;
	gc.fg.r = 0x30;
	gc.fg.g = 0x34;
	gc.fg.b = 0x23;
	ftk_widget_set_gc(net_button[0], FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(net_button[0], FTK_WIDGET_FOCUSED, &gc);
	ftk_gc_reset(&gc);
	gc.mask = FTK_GC_FG;
	gc.fg.a = 0x00;
	gc.fg.r = 0xf0;
	gc.fg.g = 0x34;
	gc.fg.b = 0xf3;
	ftk_widget_set_gc(net_button[0], FTK_WIDGET_ACTIVE, &gc);
	ftk_widget_set_gc(net_button[0], FTK_WIDGET_INSENSITIVE, &gc);
	ftk_gc_reset(&gc);
	gc.mask = FTK_GC_BG;
	gc.fg.a = 0x00;
	gc.fg.r = 0x12;
	gc.fg.g = 0x34;
	gc.fg.b = 0xff;
	ftk_widget_set_gc(net_button[0], FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(net_button[0], FTK_WIDGET_FOCUSED, &gc);
	ftk_gc_reset(&gc);
	memset(net,0,sizeof(net));
	sprintf(net,"%s:%s","IP",network_addr.ip);
	ftk_button_set_clicked_listener(net_button[0], button_ip_setting_clicked, net);
	ftk_widget_set_text(net_button[0],net);
	//ftk_widget_set_focused(net_button[0],1);
	//gateway
	net_button[1] = ftk_button_create(page,20,height / 2 -30 - 60, 370, 60);
	//ftk_label_set_alignment(net_button[1], FTK_ALIGN_LEFT);
	__ftk_widget_set_font(net_button[1], PB62_DEFAULT_FONT);
	gc.mask = FTK_GC_FG;
	gc.fg.a = 0x00;
	gc.fg.r = 0x30;
	gc.fg.g = 0x34;
	gc.fg.b = 0x23;
	ftk_widget_set_gc(net_button[1], FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(net_button[1], FTK_WIDGET_FOCUSED, &gc);
	ftk_gc_reset(&gc);
	gc.mask = FTK_GC_FG;
	gc.fg.a = 0x00;
	gc.fg.r = 0xf0;
	gc.fg.g = 0x34;
	gc.fg.b = 0xf3;
	ftk_widget_set_gc(net_button[1], FTK_WIDGET_ACTIVE, &gc);
	ftk_widget_set_gc(net_button[1], FTK_WIDGET_INSENSITIVE, &gc);
	ftk_gc_reset(&gc);
	gc.mask = FTK_GC_BG;
	gc.fg.a = 0x00;
	gc.fg.r = 0x12;
	gc.fg.g = 0x34;
	gc.fg.b = 0xff;
	ftk_widget_set_gc(net_button[1], FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(net_button[1], FTK_WIDGET_FOCUSED, &gc);
	ftk_gc_reset(&gc);
	memset(net,0,sizeof(net));
	sprintf(net,"%s:%s","gateway",gate);
	ftk_button_set_clicked_listener(net_button[1], button_gateway_setting_clicked, gate);
	ftk_widget_set_text(net_button[1],net);
	//broadcast
    net_button[2] = ftk_button_create(page,400,height / 2 -110 - 60, 380, 60);
	//ftk_label_set_alignment(net_button[1], FTK_ALIGN_LEFT);
	__ftk_widget_set_font(net_button[2], PB62_DEFAULT_FONT);
	gc.mask = FTK_GC_FG;
	gc.fg.a = 0x00;
	gc.fg.r = 0x30;
	gc.fg.g = 0x34;
	gc.fg.b = 0x23;
	ftk_widget_set_gc(net_button[2], FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(net_button[2], FTK_WIDGET_FOCUSED, &gc);
	gc.mask = FTK_GC_FG;
	gc.fg.a = 0x00;
	gc.fg.r = 0xf0;
	gc.fg.g = 0x34;
	gc.fg.b = 0xf3;
	ftk_widget_set_gc(net_button[2], FTK_WIDGET_ACTIVE, &gc);
	ftk_widget_set_gc(net_button[2], FTK_WIDGET_INSENSITIVE, &gc);
	gc.mask = FTK_GC_BG;
	gc.fg.a = 0x00;
	gc.fg.r = 0x12;
	gc.fg.g = 0x34;
	gc.fg.b = 0xff;
	ftk_widget_set_gc(net_button[2], FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(net_button[2], FTK_WIDGET_FOCUSED, &gc);
	ftk_gc_reset(&gc);
	memset(net,0,sizeof(net));
	sprintf(net,"%s:%s","broad",network_addr.gate);
	ftk_button_set_clicked_listener(net_button[2], button_broadcasting_setting_clicked, net);
	ftk_widget_set_text(net_button[2],net);
	//netmask
	net_button[3] = ftk_button_create(page,400,height / 2 -30 - 60, 380, 60);
	//ftk_label_set_alignment(net_button[1], FTK_ALIGN_LEFT);
	__ftk_widget_set_font(net_button[3], PB62_DEFAULT_FONT);
	gc.mask = FTK_GC_FG;
	gc.fg.a = 0x00;
	gc.fg.r = 0x30;
	gc.fg.g = 0x34;
	gc.fg.b = 0x23;
	ftk_widget_set_gc(net_button[3], FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(net_button[3], FTK_WIDGET_FOCUSED, &gc);
	gc.mask = FTK_GC_FG;
	gc.fg.a = 0x00;
	gc.fg.r = 0xf0;
	gc.fg.g = 0x34;
	gc.fg.b = 0xf3;
	ftk_widget_set_gc(net_button[3], FTK_WIDGET_ACTIVE, &gc);
	ftk_widget_set_gc(net_button[3], FTK_WIDGET_INSENSITIVE, &gc);
	gc.mask = FTK_GC_BG;
	gc.fg.a = 0x00;
	gc.fg.r = 0x12;
	gc.fg.g = 0x34;
	gc.fg.b = 0xff;
	ftk_widget_set_gc(net_button[3], FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(net_button[3], FTK_WIDGET_FOCUSED, &gc);
	ftk_gc_reset(&gc);
	memset(net,0,sizeof(net));
	sprintf(net,"%s:%s","netmask",network_addr.mask);
	ftk_button_set_clicked_listener(net_button[3], button_mask_setting_clicked, net);
	ftk_widget_set_text(net_button[3],net);
	memset(addr,0,sizeof(addr));
	memcpy(addr,network_addr.ip,strlen(network_addr.ip));
	//ftk_widget_set_text(label_ip,addr);
	w = ftk_bitmap_width(gc.bitmap);
	h = ftk_bitmap_height(gc.bitmap);
	//for (i = 0; i < 12; i++)
	for(i = 0;i < 4;i++)
	{
		for (j = 0; j < 3; j++) {
			gc.mask = FTK_GC_BITMAP;
			gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(),
					"/opt/PBAPPS/image/focus/empty.png");
			w = ftk_bitmap_width(gc.bitmap);
			h = ftk_bitmap_height(gc.bitmap);
			ip_button[i * 3 + j] = ftk_button_create(page,
					20 + i * (3 * w + 20) + j * w, height / 2, w, h);
			ftk_widget_set_gc(ip_button[i * 3 + j], FTK_WIDGET_NORMAL, &gc);
			ftk_widget_set_gc(ip_button[i * 3 + j], FTK_WIDGET_FOCUSED, &gc);
			ftk_widget_set_gc(ip_button[i * 3 + j], FTK_WIDGET_ACTIVE, &gc);
			ftk_widget_set_gc(ip_button[i * 3 + j], FTK_WIDGET_INSENSITIVE,
					&gc);
			ftk_bitmap_unref(gc.bitmap);
		}
		if((i * 3 + j) == 12)continue;
		gc.mask = FTK_GC_BITMAP;
		gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(),
				"/opt/PBAPPS/image/focus/10.png");
		button = ftk_button_create(page, 20 + i * (3 * w + 20) + j * w, height / 2,
				20, h);
		ftk_widget_set_gc(button, FTK_WIDGET_NORMAL, &gc);
		ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
		ftk_widget_set_gc(button, FTK_WIDGET_ACTIVE, &gc);
		ftk_widget_set_gc(button, FTK_WIDGET_INSENSITIVE, &gc);
		ftk_bitmap_unref(gc.bitmap);
	}
	type_button = ftk_button_create(page, 690, height / 2,100, 60);
	__ftk_widget_set_font(type_button,  "size:36 bold:1 italic:0");
	gc.mask = FTK_GC_FG;
	gc.fg.a = 0x00;
	gc.fg.r = 0x30;
	gc.fg.g = 0x34;
	gc.fg.b = 0x23;
	ftk_widget_set_gc(type_button, FTK_WIDGET_NORMAL, &gc);
	ftk_widget_set_gc(type_button, FTK_WIDGET_FOCUSED, &gc);
	ftk_widget_set_gc(type_button, FTK_WIDGET_ACTIVE, &gc);
	ftk_widget_set_gc(type_button, FTK_WIDGET_INSENSITIVE, &gc);
	ftk_gc_reset(&gc);
	ftk_widget_set_text(type_button, "IP");
	/*
	for (i = 0; i < strlen(addr); i++) {
		j = addr[i] - 48;
		if(j < 0)j = 10;
		sprintf(number, "%s%d%s", "/opt/PBAPPS/app/number/",j, ".png");
		gc.mask = FTK_GC_BITMAP;
		gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(),
				number);
		//ftk_widget_set_gc(tab, FTK_WIDGET_NORMAL, &gc);
		//ftk_bitmap_unref(gc.bitmap);
		button = ftk_button_create(page, 50 + i * 50, height / 2 - 140,
				ftk_bitmap_width(gc.bitmap), ftk_bitmap_height(gc.bitmap));
		ftk_widget_set_gc(button, FTK_WIDGET_NORMAL, &gc);
		//ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
		ftk_widget_set_gc(button, FTK_WIDGET_ACTIVE, &gc);
		ftk_widget_set_gc(button, FTK_WIDGET_INSENSITIVE, &gc);
		//ftk_button_set_clicked_listener(button, button_ip_clicked, &num[i]);
		ftk_bitmap_unref(gc.bitmap);
		gc.mask = FTK_GC_BITMAP;
		sprintf(number, "%s%d%s", "/opt/PBAPPS/image/focus/",j, ".png");
		gc.mask = FTK_GC_BITMAP;
		gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(),number);
		ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
		ftk_bitmap_unref(gc.bitmap);
		if(i == 0){
			ftk_widget_set_focused(button,1);
		}
	}
    */
	for (i = 0; i < 13; i++) {
		sprintf(number, "%s%d%s", "/opt/PBAPPS/image/number/", i, ".png");
		//ftk_label_set_alignment(label_ip, FTK_ALIGN_CENTER);
		button = ftk_button_create(page,i * 60 + 5, height / 2 + 100,60,70);
		__ftk_widget_set_font(button, PB62_DEFAULT_FONT);
		gc.mask = FTK_GC_FG;
		gc.fg.a = 0x00;
		gc.fg.r = 0x30;
		gc.fg.g = 0x34;
		gc.fg.b = 0x23;
		ftk_widget_set_gc(button, FTK_WIDGET_NORMAL, &gc);
		ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
		ftk_gc_reset(&gc);
		gc.mask = FTK_GC_FG;
		gc.fg.a = 0x00;
		gc.fg.r = 0xf0;
		gc.fg.g = 0x34;
		gc.fg.b = 0xf3;
		ftk_widget_set_gc(button, FTK_WIDGET_ACTIVE, &gc);
		ftk_widget_set_gc(button, FTK_WIDGET_INSENSITIVE, &gc);
		ftk_gc_reset(&gc);
		gc.mask = FTK_GC_BG;
		gc.fg.a = 0x00;
		gc.fg.r = 0x30;
		gc.fg.g = 0xff;
		gc.fg.b = 0x23;
		ftk_widget_set_gc(button, FTK_WIDGET_NORMAL, &gc);
		ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
		ftk_gc_reset(&gc);
		gc.mask = FTK_GC_BG;
		gc.fg.a = 0x00;
		gc.fg.r = 0x76;
		gc.fg.g = 0x08;
		gc.fg.b = 0xff;
		ftk_widget_set_gc(button, FTK_WIDGET_ACTIVE, &gc);
		ftk_widget_set_gc(button, FTK_WIDGET_INSENSITIVE, &gc);
		ftk_gc_reset(&gc);
		//ftk_widget_set_gc(button, FTK_WIDGET_ACTIVE, &gc);
		//ftk_widget_set_gc(button, FTK_WIDGET_INSENSITIVE, &gc);
		ftk_button_set_clicked_listener(button, button_ip_clicked, &num[i]);
		if(i==10){
			sprintf(m,"%s","<-");
		}
		else if(i==11){
			sprintf(m,"%s","->");
		}
		else if(i==12){
			sprintf(m,"%s","ok");
		}
		else sprintf(m,"%d",i);
        ftk_widget_set_text(button, m);
	}
	/*
	for (i = 0; i < 11; i++) {
        sprintf(number,"%s%d%s","/opt/PBAPPS/app/number/",i,".png");
		gc.mask = FTK_GC_BITMAP;
		gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(),
				number);
		//ftk_widget_set_gc(tab, FTK_WIDGET_NORMAL, &gc);
		//ftk_bitmap_unref(gc.bitmap);
		button = ftk_button_create(page, 50 + i * 50 + 30, height / 2 - 30,
				ftk_bitmap_width(gc.bitmap),ftk_bitmap_height(gc.bitmap));
		ftk_widget_set_gc(button, FTK_WIDGET_NORMAL, &gc);
		ftk_widget_set_gc(button, FTK_WIDGET_FOCUSED, &gc);
		ftk_widget_set_gc(button, FTK_WIDGET_ACTIVE, &gc);
		ftk_widget_set_gc(button, FTK_WIDGET_INSENSITIVE, &gc);
		ftk_button_set_clicked_listener(button, button_ip_clicked, &num[i]);
		ftk_bitmap_unref(gc.bitmap);
		//ftk_widget_set_text(button, text);
		//ftk_widget_show(button, 1);
		//ftk_button_set_clicked_listener(button, button_ip_clicked, &i);
	}
	*/
    /*
	button = ftk_button_create(page, width/2, height/2-30, width/2, 60);
	ftk_widget_set_text(button, text);
	ftk_widget_show(button, 1);
	ftk_button_set_clicked_listener(button, button_default_clicked, tab);
    */
	ftk_widget_show_all(page,1);
	return;
}
//FTK_HIDE int FTK_MAIN(int argc, char* argv[])
Ret button_config_clicked(void* ctx, void* obj)
{
	int width = 0;
	int height = 0;
	//FtkWidget* app_wins[APP_WIN_2] = NULL;
	FtkWidget* tab = NULL;
	FtkWidget* button = NULL;
	FtkBitmap* bitmap = NULL;
	FtkGc gc = {0};
	
	bitmap = ftk_theme_load_image(ftk_default_theme(),"/opt/PBAPPS/image/desktop1.png");
	app_wins[APP_WIN_2] = ftk_app_window_create();
	ftk_window_set_animation_hint(app_wins[APP_WIN_2], "app_config_window");
	width = ftk_widget_width(app_wins[APP_WIN_2]);
	height = ftk_widget_height(app_wins[APP_WIN_2]);
	tab = ftk_tab_create(app_wins[APP_WIN_2], 0, 0, width, height - 50);
	/*
	gc.mask = FTK_GC_BITMAP;
	gc.bitmap = ftk_bitmap_factory_load(ftk_default_bitmap_factory(), "/opt/PBAPPS/app/ice.png");
	ftk_widget_set_gc(tab, FTK_WIDGET_NORMAL, &gc);
	ftk_bitmap_unref(gc.bitmap);
    */
	add_gen_page(tab, "General", bitmap);
	add_IP_page(tab, "IP setting", bitmap);
	//add_file_page(tab, "file view", bitmap);
	add_other_page(tab, "other", bitmap);

	ftk_tab_set_active_page(tab, 0);
	
	button = ftk_button_create(app_wins[APP_WIN_2], width/4, height - 50, width/2, 50);
	ftk_widget_set_text(button,"Quit");
	ftk_widget_show(button, 1);

	ftk_button_set_clicked_listener(button, button_quit_clicked, app_wins[APP_WIN_2]);

	ftk_widget_show_all(app_wins[APP_WIN_2], 1);
	//FTK_QUIT_WHEN_WIDGET_CLOSE(app_wins[APP_WIN_2]);
	FTK_BITMAP_UNREF(bitmap);

	return RET_OK;
}
