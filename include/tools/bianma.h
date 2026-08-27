#ifndef BIANMA_H
#define BIANMA_H
#include <windows.h>
#include <bits/stdc++.h>
// 大战微软的powershell和cmd显示中文乱码的问题
// 没用的传参
int fixEncoding(int codePage = 8) {
    // 设置控制台输出编码为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    return 0;
}

#endif // BIANMA_H
