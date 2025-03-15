#include "../include/skipList.h"
#include <iostream>
#include <string>

void testBasicOperations() {
    std::cout << "开始测试基本操作..." << std::endl;
    
    Skiplist<int, std::string> list;
    
    // 测试插入
    list.insert(1, "one");
    list.insert(2, "two");
    list.insert(3, "three");
    
    // 测试搜索
    std::cout << "搜索测试:" << std::endl;
    std::cout << "查找 key=1: " << (list.search(1, "one") ? "成功" : "失败") << std::endl;
    std::cout << "查找 key=2: " << (list.search(2, "two") ? "成功" : "失败") << std::endl;
    std::cout << "查找不存在的 key=4: " << (list.search(4, "four") ? "成功" : "失败") << std::endl;
    
    // 测试删除
    std::cout << "删除测试:" << std::endl;
    std::cout << "删除 key=2: " << (list.remove(2) ? "成功" : "失败") << std::endl;
    std::cout << "验证删除后查找 key=2: " << (list.search(2, "two") ? "仍然存在" : "已删除") << std::endl;
}

int main() {
    testBasicOperations();
    return 0;
}