#include "init/db_create.h"
#include "tools/bianma.h"
#include <bits/stdc++.h>
#include <windows.h>
#include "tools/biao.h"
#include "tools/isChinese.h"
using namespace std;
int main(){
    // fix 编码
    int aa = 8;// 指UTF-8
    fixEncoding(aa);
    string command = "";
    ifstream file("database.db");
    if (file.is_open()) {
        // 还是没用
    }
    else {
        // 创建数据库
        createDatabase();
        // 初始化数据库
        initDatabase();
    }
    cout<<"欢迎使用 HhjSQL 监视器。"<<endl<<"Server version: 0.1.0-log HhjSQL Community Server (GPL)"<<endl<<"版权所有 (c) 2000, 2018, Nicesost/HHJ 或其关联公司保留所有权利。"<<endl<<endl<<"请输入命令或输入 help 获取帮助。"<<endl;
    chonghui:
    while (1){
        
        cout<<"$ ";
        getline(cin, command);
        if (command == "exit"){
            cout<<"byebye"<<endl;
            break;
        }
        else if (command == "help"){
            cout<<"CREATE TABLE: 创建表"<<endl;
            cout<<"CREATE TABLE CONTENT: 创建表内容"<<endl;
            cout<<"exit: 退出程序"<<endl;
            system("pause");
        }

        else if (command == "CREATE TABLE"){
            cl:
            string tableName = "";
            system("cls");
            cout<<"请输入表名或id：";
            cin>>tableName;
            if (containsChinese(tableName)){
                cout<<"不能用中文！"<<endl;
                goto cl;
            }
            if (isAllChinese(tableName)){
                cout<<"不能用中文！"<<endl;
                goto cl;
            }
            createTable(tableName);
            cout<<"创建成功！"<<endl;
            Sleep(1000);
            goto chonghui;
        }
        else if (command == "CREATE TABLE CONTENT"){
            string tableName = "";
            string name = "";
            string type = "";
            string neirong = "";
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
            cout<<"创建成功！"<<endl;
            Sleep(1000);
            goto chonghui;
        }
        else {
            cout<<"未知命令，请输入 help 获取帮助。"<<endl;
            Sleep(1000);
            goto chonghui;
        }
    }
    

    
    return 0;
}