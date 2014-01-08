#include <ftk.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <ftk_mmap.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <signal.h>

//#include "smartpad_protocol.h"
#include "event_source.h"
#include "protocol.h"
#include "main.h"
#include "sig.h"

typedef struct _PrivInfo {
    sp_src_type type;
    int fd;
    int fd_pipe_out;
    smartpad_data_buf data_buf;
}PrivInfo;
const char* app[] = {
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
		NULL
};
static FtkSource *com_source = NULL;

#define FRM_LEN(frm) (sizeof(smartpad_frame) + frm->len - 1)

static int smartpad_port_fix_write(int fd, unsigned char *data_buf, int fix_len) {
    int ret;
    int idx = 0;
    do {
        ret = write(fd, &data_buf[idx], fix_len - idx);
        if (ret < 0) {
            return ret;
        }
        idx += ret;
    }
    while (fix_len > idx);
    return 0;
}

static int smartpad_port_init_serial(const char *device) {
    int fd;
    int ret;
    struct termios opt;
    printf("signal_init\n");
    signal_init();
    fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        perror("unable to open comport ");
        return fd;
    }
    tcflush(fd, TCIOFLUSH);

    memset(&opt, 0, sizeof(opt));
    opt.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    opt.c_iflag = IGNPAR;
    opt.c_oflag = 0;
    opt.c_lflag = 0;
    opt.c_cc[VMIN] = 0;
    opt.c_cc[VTIME] = 0;
    ret = tcsetattr(fd, TCSANOW, &opt);
    if (ret < 0) {
        close(fd);
        perror("unable to adjust portsettings ");
        return ret;
    }
    tcflush(fd, TCIFLUSH);
    return fd;
}

static int smartpad_port_init_pipe_in(const char *pipe) {
    return open(pipe, O_RDONLY | O_NONBLOCK);
}

static int smartpad_port_init_pipe_out(const char *pipe) {
    return open(pipe, O_WRONLY);
}


static int smartpad_port_init_udp(unsigned short port) {
    int ret;
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        return sockfd;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    ret = bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (ret < 0) {
        close(sockfd);
        return ret;
    }
    return sockfd;
}

static void smart_action(smartpad_cmd *p_cmd, smartpad_frame *p_ack, unsigned char ack) {
    unsigned int ack_data_len = 0;
    static unsigned char ack_buf[CFG_ACK_BUF_SIZE] = { 0, };
    if (ack == p_cmd->cmd) {
        smartpad_cmd_parser(p_cmd, ack_buf, &ack_data_len);
    }
    smartpad_ack(p_ack, ack, ack_buf, ack_data_len);
}

static int ftk_source_smartpad_get_fd(FtkSource *thiz) {
    DECL_PRIV(thiz, priv);
    //printf("find something!\n");
    return priv->fd;
}

static int ftk_source_smartpad_check(FtkSource *thiz) {
	//printf("find something!\n");
    return -1;
}


static Ret ftk_source_smartpad_dispatch_serial(FtkSource *thiz) {
    int ret,j;
    static FILE *fp;
    DECL_PRIV(thiz, priv);
    return_val_if_fail(priv != NULL, RET_FAIL);
    unsigned char ack_buf[CFG_ACK_BUF_SIZE] = { 0, };
    smartpad_frame *p_ack = (smartpad_frame *)ack_buf;
    smartpad_frame *p_frm = (smartpad_frame *)priv->data_buf.data_buf;
    smartpad_cmd *p_cmd = &(p_frm->cmd);
    int n_bufferred;
    static int e_len = 0;
    static int step = 0;
    static int start = 0;
    static int flag = 0;
    FtkWidget* button;
    FtkWidget* label;
    const char *text;
    char buffer[1000];
    char cmd[100];
    static char file1[100] = { 0, };
    static char file2[20] = {0, };
	//char file2[64] = { 0, };
	const char *_buttons[2] = {"OK", NULL};
    int i = 0;
    struct stat _stat;
    static struct  timeval    last;
    struct  timeval    cur;
    unsigned long int time = 0;
    printf("into serial\n");
    if (e_len == 0) {
        e_len = priv->data_buf.data_buf_len;
    }
    ret = ioctl(priv->fd, FIONREAD, &n_bufferred);
        printf("n is %d\n",n_bufferred);
        if (ret < 0) {
            return RET_FAIL;
        }
    gettimeofday(&cur,NULL);
    time = 1000000 * (cur.tv_sec-last.tv_sec)+ (cur.tv_usec-last.tv_usec);
    printf("time is %ul us\n",time);
	if (time > 500000) {
		for (i = 0; i < 99; i++) {
			memset(file1, 0, sizeof(file1));
			sprintf(file1, "%s%d", "/opt/PBAPPS/app/app", i);
			printf("file1 is %s\n", file1);
			ret = stat(file1, &_stat);
			if (ret == 0) {
				printf("%s is exsited\n", file1);
				//break;
			} else {
				printf("%s is empty", file1);
				flag = 1;
				sprintf(file2,"%s%d","app",i);
				//ftk_warning("warning", "receiving a new application,please waiting...", _buttons);
				//raise(SIGUSR1);
				break;
			}
		}
	}
	fp = fopen(file1, "ab+");
		if (NULL == fp) {
			return -1;
		}
    printf("file is %s\n",file1);
    ret = read(priv->fd,buffer,1000);
    if (ret < 0) {
        return RET_FAIL;
    } else if (ret == 0) {
        return RET_OK;
    }
    ret = fwrite(buffer,1, ret, fp);
    fclose(fp);
    priv->data_buf.offset = 0;
    memset(buffer,0,1000);
    last.tv_sec = cur.tv_sec;
    last.tv_usec = cur.tv_usec;
    //sprintf(cmd,"%s%s","chmod 777 ",file1);
    //printf("cmd is %s\n",cmd);
    //system(cmd);
    raise(SIGUSR1);
    /*
    for(i = 0;i < 18;i++){
        label = ftk_widget_lookup(app_wins[APP_WIN_0],i+20);
        text = ftk_widget_get_text(label);
        if(memcmp(text,"empty file",sizeof("empty file"))==0){
        	button = ftk_widget_lookup(app_wins[APP_WIN_0],i);
        	ftk_widget_set_text(label,file2);
        	for(j = 0;j < 30;j++){
        		if(memcmp(file1,app[j],strlen(file2))==0){
        			break;
        		}
        	}
        	//ftk_button_set_clicked_listener(button, app_clicked,pngs[j]);
        	if(j < 30){
        		//ftk_widget_unref_self(button);
        		//set_listener(button,app[j]);
        		//ftk_widget_show_all(button, 0);
        		//ftk_widget_update(app_wins[APP_WIN_0]);
        		ftk_widget_show(label, 0);
        		ftk_widget_show(label, 1);
        		return RET_OK;
        	}
        	else{
        		printf("error to set\n");
        		return RET_FAIL;
        	}
        }
    }
    */
    //priv->data_buf.offset += ret;
    return RET_OK;
    /*
    if (step == 0) {
        if (priv->data_buf.offset < 4) {
            e_len -= ret;
            return RET_OK;
        }
        if (memcmp(priv->data_buf.data_buf, "SYNC", 4)) {
            goto l1;
        }
        step++;
    }

    if (step == 1) {
        if (priv->data_buf.offset < 12) {
            e_len -= ret;
            return RET_OK;
        }

        if (p_frm->len & p_frm->c_len) {
            goto l1;
        }
        step++;
    }

    if (step == 2) {
        if (priv->data_buf.offset < 15) {
            e_len = FRM_LEN(p_frm) - priv->data_buf.offset;
            return RET_OK;
        }
        if (p_frm->len != (p_cmd->len + sizeof(smartpad_cmd))) {
            goto l1;
        }
        step++;
    }

    if (step == 3) {
        if (priv->data_buf.offset < FRM_LEN(p_frm)) {
            e_len = FRM_LEN(p_frm) - priv->data_buf.offset;
            return RET_OK;
        }
        if (memcmp(&p_cmd->data[p_cmd->len], "EN", 2)) {
            goto l1;
        }
        step++;
    }*/
l1:
    switch (step) {
    /*
    case 0:
        smart_action(p_cmd, p_ack, FRAME_ACK_ERR_0);
        break;
    case 1:
        smart_action(p_cmd, p_ack, FRAME_ACK_ERR_1);
        break;
    case 2:
        smart_action(p_cmd, p_ack, FRAME_ACK_ERR_2);
        break;
    case 3:
        smart_action(p_cmd, p_ack, FRAME_ACK_ERR_4);
        break;
    case 4:
        smart_action(p_cmd, p_ack, p_cmd->cmd);
        break;
    */
    }
    step = 0;
    priv->data_buf.offset = 0;
    e_len = priv->data_buf.data_buf_len;

    if (smartpad_port_fix_write(priv->fd, ack_buf, FRM_LEN(p_ack)) < 0) {
        return RET_FAIL;
    }

    return RET_OK;
}


static Ret ftk_source_smartpad_dispatch_pipe(FtkSource *thiz) {
    int ret;
    DECL_PRIV(thiz, priv);
    return_val_if_fail(priv != NULL, RET_FAIL);
    unsigned char ack_buf[CFG_ACK_BUF_SIZE] = { 0, };
    smartpad_frame *p_ack = (smartpad_frame *)ack_buf;
    smartpad_frame *p_frm = (smartpad_frame *)priv->data_buf.data_buf;
    smartpad_cmd *p_cmd = &(p_frm->cmd);
    static int e_len = 0;
    static int step = 0;

    if (e_len == 0) {
        e_len = priv->data_buf.data_buf_len;
    }
    ret = read(priv->fd,
               priv->data_buf.data_buf + priv->data_buf.offset,
               e_len);
    if (ret < 0) {
        return RET_FAIL;
    } else if (ret == 0) {
        return RET_OK;
    }
    priv->data_buf.offset += ret;


    if (step == 0) {
        if (priv->data_buf.offset < 4) {
            e_len -= ret;
            return RET_OK;
        }
        if (memcmp(priv->data_buf.data_buf, "SYNC", 4)) {
            goto l1;
        }
        step++;
    }

    if (step == 1) {
        if (priv->data_buf.offset < 12) {
            e_len -= ret;
            return RET_OK;
        }

        if (p_frm->len & p_frm->c_len) {
            goto l1;
        }
        step++;
    }

    if (step == 2) {
        if (priv->data_buf.offset < 15) {
            e_len = FRM_LEN(p_frm) - priv->data_buf.offset;
            return RET_OK;
        }
        if (p_frm->len != (p_cmd->len + sizeof(smartpad_cmd))) {
            goto l1;
        }
        step++;
    }

    if (step == 3) {
        if (priv->data_buf.offset < FRM_LEN(p_frm)) {
            e_len = FRM_LEN(p_frm) - priv->data_buf.offset;
            return RET_OK;
        }
        if (memcmp(&p_cmd->data[p_cmd->len], "EN", 2)) {
            goto l1;
        }
        step++;
    }

l1:
    switch (step) {
    case 0:
        smart_action(p_cmd, p_ack, FRAME_ACK_ERR_0);
        break;
    case 1:
        smart_action(p_cmd, p_ack, FRAME_ACK_ERR_1);
        break;
    case 2:
        smart_action(p_cmd, p_ack, FRAME_ACK_ERR_2);
        break;
    case 3:
        smart_action(p_cmd, p_ack, FRAME_ACK_ERR_4);
        break;
    case 4:
        smart_action(p_cmd, p_ack, p_cmd->cmd);
        break;
    }
    step = 0;
    priv->data_buf.offset = 0;
    e_len = priv->data_buf.data_buf_len;

    if (smartpad_port_fix_write(priv->fd_pipe_out, ack_buf, FRM_LEN(p_ack)) < 0) {
        return RET_FAIL;
    }

    return RET_OK;
}

static Ret ftk_source_smartpad_dispatch_udp(FtkSource *thiz) {
    int ret;
    DECL_PRIV(thiz, priv);
    return_val_if_fail(priv != NULL, RET_FAIL);
    unsigned char ack_buf[CFG_ACK_BUF_SIZE] = { 0, };
    smartpad_frame *p_ack = (smartpad_frame *)ack_buf;
    smartpad_frame *p_frm = (smartpad_frame *)priv->data_buf.data_buf;
    smartpad_cmd *p_cmd = &(p_frm->cmd);
    static int step = 0;

    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    ret = recvfrom(priv->fd,
                   priv->data_buf.data_buf + priv->data_buf.offset,
                   priv->data_buf.data_buf_len - priv->data_buf.offset,
                   0, 
                   (__SOCKADDR_ARG)(&cli_addr),
                   &cli_len);
    if (ret < 0) {
        return RET_FAIL;
    } else if (ret == 0) {
        return RET_OK;
    }
    priv->data_buf.offset += ret;

    if (step == 0) {
        if (priv->data_buf.offset < 4) {
            return RET_OK;
        }
        if (memcmp(priv->data_buf.data_buf, "SYNC", 4)) {
            goto l1;
        }
        step++;
    }

    if (step == 1) {
        if (priv->data_buf.offset < 12) {
            return RET_OK;
        }

        if (p_frm->len & p_frm->c_len) {
            goto l1;
        }
        step++;
    }

    if (step == 2) {
        if (priv->data_buf.offset < 15) {
            return RET_OK;
        }
        if (p_frm->len != (p_cmd->len + sizeof(smartpad_cmd))) {
            goto l1;
        }
        step++;
    }

    if (step == 3) {
        if (priv->data_buf.offset < FRM_LEN(p_frm)) {
            return RET_OK;
        }
        if (memcmp(&p_cmd->data[p_cmd->len], "EN", 2)) {
            goto l1;
        }
        step++;
    }

l1:
    switch (step) {
    case 0:
        smart_action(p_cmd, p_ack, FRAME_ACK_ERR_0);
        break;
    case 1:
        smart_action(p_cmd, p_ack, FRAME_ACK_ERR_1);
        break;
    case 2:
        smart_action(p_cmd, p_ack, FRAME_ACK_ERR_2);
        break;
    case 3:
        smart_action(p_cmd, p_ack, FRAME_ACK_ERR_4);
        break;
    case 4:
        smart_action(p_cmd, p_ack, p_cmd->cmd);
        break;
    }
    step = 0;
    priv->data_buf.offset = 0;

    if (sendto(priv->fd, ack_buf, FRM_LEN(p_ack), 0, (__SOCKADDR_ARG)(&cli_addr), cli_len) < 0) {
        return RET_FAIL;
    }
    
    return RET_OK;
}

static void ftk_source_smartpad_destroy(FtkSource *thiz) {
    if (thiz != NULL) {
        DECL_PRIV(thiz, priv);
        close(priv->fd);
        if (priv->type == SP_SRC_PIPE) {
            close(priv->fd_pipe_out);
        }
        free(priv->data_buf.data_buf);
        FTK_ZFREE(thiz, sizeof(thiz) + sizeof(PrivInfo));
    }
    return;
}

static FtkSource* ftk_source_smartpad_create(sp_src_type type, size_t buf_size) {
    unsigned char *data_buf = NULL;
    FtkSource *thiz = NULL;

    data_buf = (unsigned char *)malloc(buf_size);
    if (!data_buf) {
        return NULL;
    }
    thiz = (FtkSource *)FTK_ZALLOC(sizeof(FtkSource) + sizeof(PrivInfo));
    if (thiz != NULL) {
        DECL_PRIV(thiz, priv);
        thiz->get_fd   = ftk_source_smartpad_get_fd;
        thiz->check    = ftk_source_smartpad_check;
        thiz->destroy  = ftk_source_smartpad_destroy;
        priv->fd = -1;
        priv->fd_pipe_out = 0;
        priv->type = type;
        switch (type) {
        /*
        case SP_SRC_PIPE:
            thiz->dispatch = ftk_source_smartpad_dispatch_pipe;
            priv->fd = smartpad_port_init_pipe_in("/tmp/fifo_in");
            priv->fd_pipe_out = smartpad_port_init_pipe_out("/tmp/fifo_out");
            break;
        case SP_SRC_UDP:
            thiz->dispatch = ftk_source_smartpad_dispatch_udp;
            priv->fd = smartpad_port_init_udp(8888);
            break;
        */
        default:
            thiz->dispatch = ftk_source_smartpad_dispatch_serial;
            priv->fd = smartpad_port_init_serial("/dev/ttyGS0");
            printf("into serial\n");
        }

        if (priv->fd < 0 || priv->fd_pipe_out < 0) {
            FTK_ZFREE(thiz, sizeof(thiz) + sizeof(PrivInfo));
            free(data_buf);
            return NULL;
        }

        thiz->ref = 1;
        priv->data_buf.data_buf = data_buf;
        priv->data_buf.offset = 0;
        priv->data_buf.data_buf_len = buf_size;
    } else {
        free(data_buf);
    }
    return thiz;
}

int creat_comm(sp_src_type type) {
    if (com_source) {
        return 0;
    }
    com_source = ftk_source_smartpad_create(type, CFG_FRAME_BUF_SIZE);
    if (com_source != NULL) {
        ftk_sources_manager_add(ftk_default_sources_manager(), com_source);
        return 0;
    } else {
        return -1;
    }
}


void destroy_comm(void) {
    if (!com_source) {
        return;
    }
    ftk_sources_manager_remove(ftk_default_sources_manager(), com_source);
    com_source = NULL;
}



