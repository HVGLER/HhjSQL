#ifndef BIAO_H
#define BIAO_H

#include <fstream>
#include <iostream>
#include <bits/stdc++.h>

// 创建表函数
int createTable(const std::string& tableName) {
    std::ofstream file(tableName);
    if (file.is_open()) {
        file << tableName<<":" << std::endl;
        file.close();
        return 0;
    }
    else {
        return -1;
    }
}
// 创建表内容
int createbiao_neirong(const std::string& tablename, const std::string& name, const std::string& type, const std::string& neirong){
    std::ofstream file(tablename, std::ios::app);
    if (file.is_open()) {
        file <<"内容名:"<< name << " 类型:" << type << " 内容:" << neirong << std::endl;
        file.close();
        return 0;
    }
    else {
        return -1;
    }
}

#endif