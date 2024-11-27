#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define DATA_MAX_LEN 256
#define MODE_BROADCAST   "群聊"
#define MODE_PRIVATE     "私聊"
#define MODE_SHOW_ONLINE "显示在线人员"
#define MODE_WEATHER     "显示天气"
#define MODE_EXIT        "退出"
#define FINISH           "OK"
#define CONTINUE         "continue"

struct client_list;

typedef struct server_inf
{
    int ser_fd;//服务器套接字
    //链表头节点 --- 存放每一个在线人员的信息
    struct client_list * client_list_head;

    pthread_mutex_t mutex;

}SI,*P_SI;

typedef struct client_list
{
    int  cli_fd;
    char cli_ip[16];
    char user_name[DATA_MAX_LEN];
    pthread_t cli_pid;
    P_SI p_si;
    struct client_list * next, * prev;

}client_node,*client_link;

P_SI Tcp_Server_Init(int prot_num);
client_link Create_Client_Node();
bool  Wait_For_Client_Connect(P_SI p_si);
bool  Add_Client_Node(client_link client_list_head,client_link new_client_node);
void * Client_Pthread_Mode_Select(void * arg);
bool  Send_Online_Inf(client_link present_client_node);
bool  Del_Client_Node(client_link del_node);
bool  Send_All_Private(client_link present_client_node,char * SEND_MODE);
#endif