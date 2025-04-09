#include "test.pb.h"
#include <iostream>
#include <string>
using namespace fixbug;

int main()
{

    GetFriendListResponse rsp;
    ResultCode *result = rsp.mutable_result();
    result->set_errcode(0);
    result->set_errmsg("获取好友列表失败了");
    User *user1 = rsp.add_friend_list();
    user1->set_name("zhangxiaoguang");
    user1->set_age(18);
    user1->set_sex(User::MAN);

    User *user2 = rsp.add_friend_list();
    user2->set_name("zhangxiaoguang");
    user2->set_age(18);
    user2->set_sex(User::MAN);

    std::cout << rsp.friend_list_size() << std::endl;
    return 0;
}

int main1()
{
    // 封装了login请求对象的数据
    LoginRequest req;
    req.set_name("zhangxiaoguang");
    req.set_pwd("123456");
    // 对象数据序列化
    std::string serialized_data;
    if (req.SerializeToString(&serialized_data))
    {
        std::cout << "Serialized data: " << serialized_data << std::endl;
    }
    else
    {
        std::cerr << "Failed to serialize LoginRequest." << std::endl;
        return 1;
    }

    // 从serialized_data中反序列化出LoginRequest对象
    LoginRequest req2;
    if (req2.ParseFromString(serialized_data))
    {
        std::cout << "Parsed LoginRequest:" << std::endl;
        std::cout << "Name: " << req2.name() << std::endl;
        std::cout << "Password: " << req2.pwd() << std::endl;
    }
    else
    {
        std::cerr << "Failed to parse LoginRequest." << std::endl;
        return 1;
    }

    return 0;
}