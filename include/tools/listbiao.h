#ifndef LISTBIAO_H
#define LISTBIAO_H
#include <bits/stdc++.h>
void printFromLineN(const std::string& filename, int startLine) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开文件" << std::endl;
        return;
    }

    std::string line;
    int currentLine = 0;
    
    // 跳过前 startLine - 1 行
    while (currentLine < startLine - 1 && std::getline(file, line)) {
        currentLine++;
    }
    
    // 如果文件提前结束，说明行数不够
    if (currentLine < startLine - 1) {
        std::cerr << "文件只有 " << currentLine << " 行，不足 " << startLine << " 行" << std::endl;
        return;
    }
    
    // 输出从 startLine 到文件末尾的所有行
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
}
#endif