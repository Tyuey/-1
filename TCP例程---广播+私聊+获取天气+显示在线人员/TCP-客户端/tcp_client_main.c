#include "tcp_client.h"


int main(int argc, char * argv[]) //./xxx ip num 名字
{
    if(argc != 4) return -1;


    int cli_fd = Client_Init(argv[1],atoi(argv[2]),argv[3]);
    if(cli_fd == -1)
    {
        printf("TCP客户端初始化失败！\n");
        return -1;
    }

    if(Mode_Select(cli_fd) == false)
    {
        printf("客户端运行失败！\n");
        return -1;
    }

    return 0;
}