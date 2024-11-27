#include "tcp_server.h"


int main(int argc,char * argv[])
{
    if(argc != 2) return -1;

    P_SI p_si = Tcp_Server_Init(atoi(argv[1]));
    if(p_si == (P_SI)-1)
    {
        printf("Tcp服务器初始化失败！\n");
        return -1;
    }

    //等待客户端连接  while(1) accept
    if(Wait_For_Client_Connect(p_si) == false)
    {
        printf("服务器等待客户端连接失败！\n");
        return -1;
    }

    return 0;
}