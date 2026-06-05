#include "clusterserver.hpp"
#include "json.hpp" // 这些地方可以直接添加，不需要写路径，就是因为最外层的CMakeLists.txt写了头文件寻找路径
#include "clusterservice.hpp"

#include <functional>
#include <string>
using namespace std;
using namespace placeholders;
using json = nlohmann::json;

ClusterServer::ClusterServer(EventLoop *loop,
                       const InetAddress &listenAddr,
                       const string &nameArg)
    : _server(loop, listenAddr, nameArg), _loop(loop)
{
    _server.setConnectionCallback(bind(&ClusterServer::onConnection, this, _1));

    _server.setMessageCallback(bind(&ClusterServer::onMessage, this, _1, _2, _3));

    _server.setThreadNum(4);
}

void ClusterServer::start()
{
    _server.start();
}

void ClusterServer::onConnection(const TcpConnectionPtr &conn)
{
    // 用户断开连接
    if (!conn->connected())
    {
        // 客户异常断开
        ClusterService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

void ClusterServer::onMessage(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp time)
{
    string buf = buffer->retrieveAllAsString();
    json js = json::parse(buf);
    // 通过js中的id来获取一个handler，这样把网络模块和业务模块分开
    // 转换成int类型
    auto handler = ClusterService::instance()->getHandler(js["msgid"].get<int>());
    // handler在执行的时候才知道是login还是reg
    handler(conn, js, time);
}