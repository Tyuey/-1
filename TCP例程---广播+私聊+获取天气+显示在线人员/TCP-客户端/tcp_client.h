#ifndef TCP_CLIENT_H_
#define TCP_CLIENT_H_
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


int Client_Init(char * ser_ip,int ser_port_num,char * user_name);
bool Mode_Select(int cli_fd);
bool BroadCast(int cli_fd);
bool Private(int cli_fd);
bool Show_Online(int cli_fd);
bool Show_Weather(int cli_fd);
void * Reav_Msg(void * arg);


#endif