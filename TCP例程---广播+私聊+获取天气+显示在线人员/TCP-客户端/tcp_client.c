#include "tcp_client.h"


int Client_Init(char * ser_ip,int ser_port_num,char * user_name)
{

    int cli_fd = socket(AF_INET,SOCK_STREAM,0);
    if(cli_fd == -1)
    {
        perror("socket ...");
        return -1;
    }

    struct sockaddr_in ser_inf;
    memset(&ser_inf,0,sizeof(ser_inf));

    ser_inf.sin_family        = AF_INET;
    ser_inf.sin_port          = htons(ser_port_num);
    ser_inf.sin_addr.s_addr   = inet_addr(ser_ip);

    if(connect(cli_fd,(struct sockaddr *)&ser_inf,sizeof(ser_inf)) == -1)
    {
        perror("connect ...");
        return -1;
    }

    //把自己的名字发送给服务器
    if(write(cli_fd,user_name,strlen(user_name)) == -1)
    {
        perror("write ...");
        return -1;
    }
  
    return cli_fd;
}


bool Mode_Select(int cli_fd)
{
    int mode_num;
    while(1)
    {
        
        printf("欢迎进入XXXX聊天系统！\n");
        printf("1)群聊\n");
        printf("2)私聊\n");
        printf("3)显示在线人员\n");
        printf("4)显示天气\n");
        printf("5)退出\n");
        if(scanf("%d",&mode_num) != 1)
        {
            while(getchar() != 0);
            continue;
        }

        switch (mode_num)
        {
            case 1: 
                if(BroadCast(cli_fd) == false)
                {
                    printf("广播失败！\n");
                    return false;
                }
            break;
            
            case 2: 
                if(Private(cli_fd) == false)
                {
                    printf("私聊失败!\n");
                    return false;
                }
            break;

            case 3: 
                if(Show_Online(cli_fd) == false)
                {
                    printf("显示在线人员失败！\n");
                    return false;
                }
            break;

            case 4: 
                if(Show_Weather(cli_fd) == false)
                {
                    printf("显示天气失败！\n");
                    return false;
                }
                break;

            case 5: goto exit;

            default:printf("没有这个功能！\n");break;
        }


    }

    exit:
    //发送功能需要给服务器
    if(write(cli_fd,MODE_EXIT,strlen(MODE_EXIT)) == -1)
    {
        perror("write ...");
        return false;
    }
    return true;
}


bool BroadCast(int cli_fd) //connect之后，客户端可以使用自己的套接字给服务器发送接收信息
{
    printf("进入群聊，【输入退出返回功能界面】\n");
    //发送功能需要给服务器
    if(write(cli_fd,MODE_BROADCAST,strlen(MODE_BROADCAST)) == -1)
    {
        perror("write ...");
        return false;
    }

    char data[DATA_MAX_LEN];
    //创建线程去读取其他人的群聊消息
    pthread_t pid;

    if(pthread_create(&pid,NULL,Reav_Msg,(void *)&cli_fd) != 0)
    {
        perror("pthread_create ...");
        return false;
    }

    while(1)
    {
        memset(data,0,DATA_MAX_LEN);
        printf("请输入群聊消息：");
        scanf("%s",data);
        while(getchar() != '\n');

        if(write(cli_fd,data,strlen(data)) == -1)
        {
            perror("write ...");
            return false;
        }

        if(strcmp(data,MODE_EXIT) == 0)
        {
            pthread_cancel(pid);
            break;
        }

    }

    return true;
}


bool Private(int cli_fd)
{
    //发送功能需要给服务器
    if(write(cli_fd,MODE_PRIVATE,strlen(MODE_PRIVATE)) == -1)
    {
        perror("write ...");
        return false;
    }

    //创建线程去读取其他人的群聊消息
    pthread_t pid;

    if(pthread_create(&pid,NULL,Reav_Msg,(void *)&cli_fd) != 0)
    {
        perror("pthread_create ...");
        return false;
    }

    char data[DATA_MAX_LEN];
    while(1)
    {
        memset(data,0,DATA_MAX_LEN);
        printf("请输入要发送的私聊消息@对象名字:"); 
        scanf("%s",data);
        while(getchar() != '\n');

        if(write(cli_fd,data,strlen(data)) == -1)
        {
            perror("write ...");
            return false;
        }

        if(strcmp(data,MODE_EXIT) == 0)
        {
            pthread_cancel(pid);
            break;
        }
    }


    return true;
}


bool Show_Online(int cli_fd)
{
    //发送功能需要给服务器
    if(write(cli_fd,MODE_SHOW_ONLINE,strlen(MODE_SHOW_ONLINE)) == -1)
    {
        perror("write ...");
        return false;
    }

    //循环接收服务器发送过来的在线人员IP信息 --- 当收到finish
    char data[DATA_MAX_LEN];
    printf("-------------------------------------------\n");
    while(1)
    {
        memset(data,0,DATA_MAX_LEN);

        if(read(cli_fd,data,DATA_MAX_LEN) == -1)
        {
            perror("read ...");
            return false;
        }

        if(strcmp(data,FINISH) == 0) 
        {
            printf("-------------------------------------------\n");
            break;
        }

        if(!(strstr(data,"在线人员")))//判断该消息不是在线人员信息
        {
            printf("%s\n",data);
            continue;
        }
    
        printf("%s\n",data);

        //把continue发送告诉服务器继续发
        if(write(cli_fd,CONTINUE,strlen(CONTINUE)) == -1)
        {
            perror("write ...");
            return false;
        }
    }

    return true;
}


bool Show_Weather(int cli_fd)
{
    //发送功能需要给服务器
    if(write(cli_fd,MODE_WEATHER,strlen(MODE_WEATHER)) == -1)
    {
        perror("write ...");
        return false;
    }


    return true;
}


void * Reav_Msg(void * arg)
{
    int cli_fd = *((int *)arg);
    pthread_detach(pthread_self());

    char data[DATA_MAX_LEN];
    while(1)
    {
        memset(data,0,DATA_MAX_LEN);

        if(read(cli_fd,data,DATA_MAX_LEN) == -1)
        {
            perror("Read ...");
            pthread_exit((void *)-1);
        }

        printf("%s\n",data);
    }

    pthread_exit((void *)0);
}
