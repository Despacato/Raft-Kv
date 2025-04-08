#include <iostream>
#include "mprpcapplication.h"

void MprpcApplication::Init(int argc, char **argv)
{
    std::cout << "MprpcApplication::Init" << std::endl;
}
MprpcApplication &MprpcApplication::GetInstance()
{
    static MprpcApplication instance;
    return instance;
}