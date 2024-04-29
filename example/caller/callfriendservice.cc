#include <iostream>

#include <myrpcapplication.h>
#include <friend.pb.h>

int main(int argc, char **argv)
{
    // 整个程序启动以后，想使用myrpc框架来享受rpc服务调用，一定需要先调用框架的初始化函数（只初始化一次）
    MyrpcApplication::Init(argc, argv);

    // 演示调用远程发布的rpc方法Login
    fixbug::FriendServiceRpc_Stub stub(new MyrpcChannel());
    fixbug::GetfriendsListRequest request;
    request.set_userid(123);

    fixbug::GetfriendsListResponse response;
    // 以同步的方式发起rpc调用请求，等待返回结果
    MyrpcController controller;
    stub.GetfriendsList(&controller, &request, &response, nullptr); // RpcChannel->RpcChannel::callMethod  这个方法集中来做所有rpc方法调用的参数序列化和网络发送

    // 一次rpc调用完成，读调用的结果
    if (controller.Failed())
    {
        std::cout << controller.ErrorText() << std::endl;
    }
    else
    {
        if (0 == response.result().errcode())
        {
            std::cout << "rpc login response success!" << std::endl;
            int size = response.friends_size();
            for (int i = 0; i < size; ++i)
            {
                std::cout << "friend index :" << i + 1 << response.friends(i) << std::endl;
            }
        }
        else
        {
            std::cout << "rpc login response error:" << response.result().errmsg() << std::endl;
        }
    }

    return 0;
}