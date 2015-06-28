#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MAXBUF 1024
#define MAXEPOLLSIZE 100


#define NOSKILL  '!'

//接收最大的玩家数
#define PLAYERFDNUM 40

//文件描述符的空定义
#define NULLFD 0

//初始坐标的个数
#define COORDNUM 4
//发送战斗协议
struct FightProtocal
{
    int hero_id;		//根据id识别是自己的英雄还是对方英雄的消息
    float x;                   //源坐标
    float y;
    float z;
    float dx;                   //目的地坐标
    float dy;
    float dz;
    float angle;
    int HP;				//当HP为0时，即该英雄已经死了
    char skill;			//使用的是何种技能
}sendPro,recvPro;

//文件描述符与英雄ID的映射
struct HeroMap
{
    int fd;
    int hero_id;
}map[PLAYERFDNUM];

struct Coord
{
    int hero_id;
    int x;
    int z;
}coord[COORDNUM];

struct PointCoord
{
        float x;
        float z;
}P_Coord;

unsigned int myport = 6000;
int num=0;      //当前人数
int listener;                       //本机fd也就是服务监听的fd
int kdpfd;                          //最大fd
socklen_t len;
struct epoll_event ev;
char recvbuf[MAXBUF];
char sendbuf[MAXBUF];

void InitCoord()
{
    coord[0].x=0;
    coord[0].z=400;

    coord[1].x=0;
    coord[1].z=390;

    coord[2].x=-400;
    coord[2].z=100;

    coord[3].x=400;
    coord[3].z=-100;

}

/*
   setnonblocking - 设置句柄为非阻塞方式
   */
int setnonblocking(int sockfd)
{
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1)
    {
        return -1;
    }
    return 0;
}


//群发信息给其他的玩家,转发战斗信息
void SendMsg(char *msg)
{
    //bzero(sendbuf,MAXBUF);
    memcpy(sendbuf,recvbuf,sizeof(recvbuf));
    int len=sizeof("FIGHTINFO#")-1;
    //memcpy(sendbuf,"FIGHTINFO#",len);
    //memcpy(sendbuf+len,msg,sizeof(sendbuf)-len);
    memcpy(&sendPro,sendbuf+len,sizeof(sendPro));
    if (sendPro.skill=='Z')
    {
        memcpy(&P_Coord,sendbuf+sizeof(sendPro),sizeof(P_Coord));
    }

    //strcat(sendbuf,msg);
    for(int i=0;i<PLAYERFDNUM;++i)
    {
        if(map[i].fd!=NULLFD)
        {
            if(send(map[i].fd,sendbuf,sizeof(sendbuf),0) == -1)
            {
                printf("%d :SendMsg can not send\n",map[i].fd);
            }
        }
    }
}


void SendHerosInfo()
{
    bzero(sendbuf,MAXBUF);
    int playernum=0;
    int fdsend[16];
    char temp[256];

    memcpy(sendbuf,"HEROID#",sizeof("HEROID#"));
    char *tempbuf=sendbuf+sizeof("HEROID#")+1;
    for(int i=0;i<PLAYERFDNUM;++i)
    {
        if(map[i].fd!=NULLFD)
        {
            fdsend[playernum]=map[i].fd;
            coord[playernum].hero_id=map[i].hero_id;
            memcpy(temp,&coord[playernum],sizeof(Coord));
            memcpy(tempbuf,temp,sizeof(Coord));
            tempbuf=tempbuf+sizeof(Coord);

            //memcpy(tempbuf,",",1);
            //tempbuf=tempbuf+1;
            playernum++;

        }
    }
    sprintf(sendbuf+sizeof("HEROID#")-1,"%d",playernum);
    memcpy(sendbuf+sizeof("HEROID#"),"#",1);
    for(int i=0;i<playernum;i++)
    {
        if(send(fdsend[i],sendbuf,sizeof(sendbuf),0) == -1)
        {
            printf("%d :can not send\n",fdsend[i]);
        }
        //printf("你妹\n");
    }
    //printf("%s\n",sendbuf);
}

void EmptyFD(int fd)
{
    for(int i=0;i<PLAYERFDNUM;++i)
    {
        if(map[i].fd == fd)
        {
            map[i].fd = NULLFD;
            map[i].hero_id = NULLFD;
            break;
        }
    }
}



int RecvMessage(int new_fd)
{
    int len;
    len = recv(new_fd, recvbuf, MAXBUF, 0);

    if (len > 0)
    {

        recvbuf[len]='\0';
        char *type=NULL;
        //printf("%s\n",recvbuf);
        type=strtok(recvbuf,"#");

        if(strcmp(type,"FIGHTINFO")==0)
        {
            //type=strtok(NULL,",");
            //type=recvbuf+sizeof("FIGHTINFO#")-1;
            //memcpy(tt,type,strlen(type));
            //memcpy(&pro,type,sizeof(pro));
            //printf("angle:%f\n",pro.angle);
            //printf("skill:%c\n",pro.skill);
            SendMsg(type);
        }
        if(strcmp(type,"WEREADY")==0)
        {
            SendHerosInfo();
            printf("WEREADY\n");
        }
    }
    else
    {
        if (len < 0)
        {
            printf("消息接收失败！错误代码是%d，错误信息是'%s'\n",errno, strerror(errno));
        }
        EmptyFD(new_fd);
        close(new_fd);
        return -1;
    }
    /* 处理每个新连接上的数据收发结束 */
    return len;
}



int InitSocket()
{
    unsigned int lisnum=3;
    struct sockaddr_in my_addr, their_addr;
    struct rlimit rt;
     /* 设置每个进程允许打开的最大文件数 */
    rt.rlim_max = rt.rlim_cur = MAXEPOLLSIZE;
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1)
    {
        perror("setrlimit");
        exit(1);
    }
    else
    {
        printf("设置系统资源参数成功！\n");
    }
    /* 开启 socket 监听 */
    if ((listener = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    else
    {
        printf("socket 创建成功！\n");
    }
    setnonblocking(listener);
    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = PF_INET;
    my_addr.sin_port = htons(myport);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(listener, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }
    else
    {
        printf("IP 地址和端口绑定成功\n");
    }
    if (listen(listener, lisnum) == -1)
    {
        perror("listen");
        exit(1);
    }
    else
    {
        printf("开启gameserver服务成功！\n");
    }
    /* 创建 epoll 句柄，把监听 socket 加入到 epoll 集合里 */
    kdpfd = epoll_create(MAXEPOLLSIZE);
    len = sizeof(struct sockaddr_in);
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listener;
    if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, listener, &ev) < 0)
    {
        fprintf(stderr, "epoll set insertion error: fd=%d\n", listener);
        return -1;
    }
    else
    {
        printf("监听 socket 加入 epoll 成功！\n");
    }
}

void SetHeroID(int fd)
{
    for(int i=0;i<PLAYERFDNUM;++i)
    {
        if(map[i].fd==NULLFD)
        {
            map[i].fd=fd;
            map[i].hero_id=fd;
            break;
        }
    }
}


int HandleMessage()
{
    int  new_fd, nfds, ret, curfds;
    socklen_t len;
    struct sockaddr_in my_addr, their_addr;
    struct epoll_event events[MAXEPOLLSIZE];
    curfds = 1;
    while (1)
    {
        /* 等待有事件发生 */
        nfds = epoll_wait(kdpfd, events, curfds, -1);
        if (nfds == -1)
        {
            perror("epoll_wait");
            break;
        }
        /* 处理所有事件 */
        for (int n = 0; n < nfds; ++n)
        {
            //当为自己文件描述符时
            if (events[n].data.fd == listener)
            {
                new_fd = accept(listener, (struct sockaddr *) &their_addr,&len);
                if(new_fd<0)
                {
                   printf("link error\n");
                }
                SetHeroID(new_fd);
                char id[64];
                char ownid[64]="OWNID#";
                sprintf(id,"%d",new_fd);
                strcat(ownid,id);
                send(new_fd,ownid,sizeof(ownid),0);
                //num++;
                if (new_fd < 0)
                {
                    perror("accept");
                    continue;
                }
                else
                {
                    printf("有连接来自于： %d:%d， 分配的 socket 为:%d\n",
                            inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), new_fd);
                }
                setnonblocking(new_fd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = new_fd;
                if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, new_fd, &ev) < 0)
                {
                    fprintf(stderr, "把 socket '%d' 加入 epoll 失败！%s\n",
                            new_fd, strerror(errno));
                    return -1;
                }
                curfds++;
            }
            else
            {
                ret = RecvMessage(events[n].data.fd);
                if (ret < 1 && errno != 11)
                {
                    epoll_ctl(kdpfd, EPOLL_CTL_DEL, events[n].data.fd,&ev);

                    curfds--;
                }
            }
        }
    }
    close(listener);
}

int main(int argc, char **argv)
{
    //pid_t pid;
    //if((pid=fork()) == 0)
    //{
        //char *temp[]={"0","1"};
        //if(execv("AcceptPlayer",temp) < 0)
        //{
        //    printf("fail to open AcceptPlayer\n");
        //}
    //}
    //else if(pid > 0)
    //{
        //memset(fdset,NULLFD,sizeof(fdset));
        InitCoord();
        InitSocket();
        HandleMessage();
    //}
    return 0;
}
