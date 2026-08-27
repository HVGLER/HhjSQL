#ifndef DB_CREATE_H
#define DB_CREATE_H

#include <fstream>
#include <iostream>

// 创建数据库函数
int createDatabase() {
    std::ifstream file("database.db");
    if (file.is_open()) {
        file.close();
        return 0;
    }
    else {
        std::ofstream newFile("database.db");
        if (newFile.is_open()) {
            // std::cout << "创建数据库成功" << std::endl;
            newFile.close();
            return 0;
        }
        else {
            // std::cerr << "创建数据库失败" << std::endl;
            return -1;
        }
    }
}

// 初始化数据库函数
int initDatabase() {
    std::ofstream file("database.db");
    if (file.is_open()) {
        file << "HHJ-Database v0.1.0 Beta" << std::endl;
        file << "Created by HHJ" << std::endl;
        file.close();
        return 0;
    }
    else {
        return -1;
    }
}

#endif
