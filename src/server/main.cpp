#include "clusterserver.hpp"
#include "clusterservice.hpp"

#include <iostream>
#include <signal.h>

using namespace std;

// 处理服务器ctrl+c结束后，处理user的状态信息
void resetHandler(int)
{
    ClusterService::instance()->reset();
    exit(0);
}

// 网络模块，业务模块，数据库模块三者分开
int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cerr << "command invalied! example: ./ClusterServer 192.168.42.129 6000" << endl;
        exit(-1);
    }

    // 解析通过命令行参数传递的ip和port
    char *ip = argv[1];
    uint16_t port = atoi(argv[2]);
    signal(SIGINT, resetHandler);
    EventLoop loop;
    InetAddress addr(ip, port);

    ClusterServer server(&loop, addr, "ClusterServer");

    server.start();
    loop.loop();

    return 0;
}