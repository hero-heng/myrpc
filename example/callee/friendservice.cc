#include "friend.pb.h"
#include "myrpcapplication.h"
#include "rpcprovider.h"

#include <iostream>
#include <string>
#include <vector>

class FriendService : public fixbug::FriendServiceRpc
{
public:
    std::vector<std::string> GetFriendsList(uint32_t userid)
    {
        std::cout << "do GetFriendsList service!" << std::endl;
        std::vector<std::string> vec;
        vec.push_back("gao yang");
        vec.push_back("li yong");
        vec.push_back("wang shuo");
        return vec;
    }

    // 重写基类方法
    void GetfriendsList(::google::protobuf::RpcController *controller,
                        const ::fixbug::GetfriendsListRequest *request,
                        ::fixbug::GetfriendsListResponse *response,
                        ::google::protobuf::Closure *done)
    {
        uint32_t userid = request->userid();
        std::vector<std::string> friendsList = GetFriendsList(userid);

        // 把响应写入
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        for(std::string &name : friendsList)
        {
            std::string *p = response->add_friends();
            *p = name;
        }

        // 执行回调操作  执行响应对象数据的序列化和网络发送（都是由框架来完成的）
        done->Run();
    }
};

int main(int argc, char **argv)
{
    // 调用框架的初始化操作 ./provider -i config.conf
    MyrpcApplication::Init(argc, argv);

    // provider就是rpc网络服务提供者。把FriendService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new FriendService());

    // 启动一个rpc服务发布节点.Run以后，进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();
    return 0;
}