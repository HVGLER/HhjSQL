#include "init/db_create.h"
#include "tools/bianma.h"
#include <bits/stdc++.h>
#include <windows.h>
#include "tools/biao.h"
#include "tools/isChinese.h"
using namespace std;

int main(){
    fixEncoding(8);
    CreateDirectoryA("data", NULL);
    
    // 检查数据库是否存在，不存在则创建
    if (GetFileAttributesA("database.db") == INVALID_FILE_ATTRIBUTES) {
        createDatabase();
        initDatabase();
    }
    
    cout<<"欢迎使用 HhjSQL 监视器。"<<endl
        <<"Server version: 0.1.0-log HhjSQL Community Server (GPL)"<<endl
        <<"版权所有 (c) 2000, 2018, Nicesost/HHJ 或其关联公司保留所有权利。"<<endl<<endl
        <<"请输入命令或输入 help 获取帮助。"<<endl;
    
    while (1){
        cout<<"$ ";
        string command;
        getline(cin, command);
        
        if (command == "exit"){
            cout<<"byebye"<<endl;
            break;
        }
        else if (command == "help"){
            cout<<"CREATE TABLE: 创建表"<<endl;
            cout<<"CREATE TABLE CONTENT: 创建表内容"<<endl;
            cout<<"READ TABLE: 读取表内容"<<endl;
            cout<<"exit: 退出程序"<<endl;
            system("pause");
        }
        else if (command == "CREATE TABLE"){
            string tableName;
            system("cls");
            cout<<"请输入表名或id：";
            cin>>tableName;
            if (containsChinese(tableName) || isAllChinese(tableName)){
                cout<<"不能用中文！"<<endl;
                cin.ignore();
                continue;
            }
            if (createTable(tableName) == 0) {
                cout<<"创建成功！"<<endl;
            } else {
                cout<<"创建失败！"<<endl;
            }
            Sleep(1000);
        }
        else if (command == "CREATE TABLE CONTENT"){
            string tableName, name, type, neirong;
            system("cls");
            cout<<"请输入表名或id：";
            cin>>tableName;
            cout<<"请输入字段名：";
            cin>>name;
            cout<<"请输入字段类型：";
            cin>>type;
            cout<<"请输入字段内容：";
            cin>>neirong;
            createbiao_neirong(tableName, name, type, neirong);
            Sleep(1000);
        }
        else if (command == "READ TABLE"){
            string tableName;
            system("cls");
            cout<<"请输入表名：";
            cin>>tableName;
            readTable(tableName);
            system("pause");
        }
        else {
            cout<<"未知命令，请输入 help 获取帮助。"<<endl;
            Sleep(1000);
        }
    }
    
    return 0;
}
