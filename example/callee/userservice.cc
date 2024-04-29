#include <iostream>
#include <string>

#include "user.pb.h"
#include "myrpcapplication.h"
#include "rpcprovider.h"

class UserService : public fixbug::UserServiceRpc
{
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "doing local service Login" << std::endl;
        std::cout << "name:" << name << "pwd" << pwd << std::endl;
        return true;
    }

    bool Register(uint32_t id, std::string name, std::string pwd)
    {
        std::cout << "doing local service Register" << std::endl;
        std::cout << "id:" << id << "name:" << name << "pwd" << pwd << std::endl;
        return true;
    }

    void Register(::google::protobuf::RpcController *controller,
               const ::fixbug::RegisterRequest *request,
               ::fixbug::RegisterResponse *response,
               ::google::protobuf::Closure *done)
    {
        uint32_t id = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();

        // 做本地业务
        bool register_result = Register(id, name, pwd);

        // 把响应写入
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(register_result);

        // 执行回调操作  执行响应对象数据的序列化和网络发送（都是由框架来完成的）
        done->Run();
    }
    /**
     * 重写基类UserServiceRpc的虚函数
     * 1. caller ===> Login(LoginRequest) => muduo => callee
     * 2. callee 根据上面拿到的Login(LoginRequest) => 交到下面重写的这个Login方法上了
     */
    void Login(::google::protobuf::RpcController *controller,
               const ::fixbug::LoginRequest *request,
               ::fixbug::LoginResponse *response,
               ::google::protobuf::Closure *done)
    {
        // 框架给业务上报了请求参数LoginRequest，应用获取相应数据做本地业务
        std::string name = request->name();
        std::string pwd = request->pwd();

        // 做本地业务
        bool login_result = Login(name, pwd);

        // 把响应写入
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);

        // 执行回调操作  执行响应对象数据的序列化和网络发送（都是由框架来完成的）
        done->Run();
    }
};

int main(int argc, char **argv)
{
    // 调用框架的初始化操作 ./provider -i config.conf
    MyrpcApplication::Init(argc, argv);

    // provider就是rpc网络服务提供者。把UserService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new UserService());

    // 启动一个rpc服务发布节点.Run以后，进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();
    return 0;
}