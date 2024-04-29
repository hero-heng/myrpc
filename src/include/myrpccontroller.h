#pragma once

#include <google/protobuf/service.h>
#include <string>

class MyrpcController : public google::protobuf::RpcController
{
public:
    MyrpcController();
    void Reset();
    bool Failed() const;
    std::string ErrorText() const;
    void SetFailed(const std::string& reason);

    //未实现具体的方法
    void StartCancel();
bool IsCanceled() const;
void NotifyOnCancel(google::protobuf::Closure* callback);
private:
    bool m_failed; // rpc方法执行过程中的状态
    std::string m_errText; // rpc方法执行过程中的错误信息
};