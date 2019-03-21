#include <iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<vector>
#include<string>
using namespace std;

/*
全局变量声明
*/
vector<string> helpList;//帮助列表
vector<string> path;//路径列表

/*
初始化帮助列表
*/
void initHelp() {
    helpList.push_back("help : 查询指令功能");
    helpList.push_back("createfile/touch : 建立文件");
    helpList.push_back("createfolder/mkdir : 建立文件夹");
    helpList.push_back("list/ls : 列出当前所有文件和文件夹");
    helpList.push_back("enter/cd : 进入文件夹");
    helpList.push_back("goback : 回到上一级");
    helpList.push_back("delete/rm : 删除文件或文件夹");
    helpList.push_back("open/read : 阅读文件");
    helpList.push_back("add/append : 在某个文件后追加内容");
    helpList.push_back("insert : 在某个文件的中间插入内容");
    helpList.push_back("exit : 退出文件系统");
    helpList.push_back("createuser : 创建用户");
    helpList.push_back("listuser : 列出所有用户");
    helpList.push_back("login : 登录到某个用户");
}
/*
输出帮助说明
*/
void getHelp() {
    cout<<"**************命令列表**************"<<endl;
    for (int i=0; i<helpList.size(); i++)
        cout<<helpList[i]<<endl;
    cout<<"************************************"<<endl;
}


/*
文件类------------------------------------------------------------------------------
*/
struct Node {
    Node* parent;//父节点
    Node* headSon;//第一个子节点
    Node* sibiling;//下一个兄弟节点
    string name;//文件名
    vector<string> content;//内容
    int permission;//权限等级
    int type;//0是文件夹，1是文件

    //初始化函数
    Node() {
        parent = 0;
        headSon = 0;
        sibiling = 0;
        permission = 0;
        type = 0;
    }

    //判断名字是否已经使用，返回节点。
    Node* isExist(const string& findName) {
        Node* now = this->headSon;
        while(now != 0) {
            if (now->name == findName) {
                return now;
            }
            now = now->sibiling;
        }
        return 0;
    }

    //根据文件名、文件内容、文件权限等级创建文件。（插入成第一个儿子）
    bool createFile(const string& newName,const string& newContent,const int level) {
        Node* exist = this->isExist(newName);
        if (exist) {
            printf("文件或文件夹已存在！\n");
            return false;
        }
        Node* file = new Node();
        file->name = newName;
        file->type = 1;
        file->content.push_back(newContent);
        //插入成第一个儿子
        Node* tmp = this->headSon;
        this->headSon = file;
        file->sibiling = tmp;
        file->parent = this;
        file->permission = level;
        return true;
    }

    //根据文件夹、文件夹权限等级创建文件。（插入成第一个儿子）
    bool createFolder(const string& newName,const int level) {
        Node* exist = this->isExist(newName);
        if (exist) {
            printf("文件或文件夹已存在！\n");
            return false;
        }
        Node* folder = new Node();
        folder->name = newName;
        Node* tmp = this->headSon;
        this->headSon = folder;
        folder->sibiling = tmp;
        folder->parent = this;
        folder->permission = level;
        return true;
    }

    //输出本级文件列表
    void listAll() {
        Node* now = this->headSon;
        while (now != 0) {
            cout<<now->name<<" ";
            if (now->type == 0) cout<<"[文件夹]"<<endl;
            else cout<<"[文件]"<<endl;
            now = now->sibiling;
        }
    }

    //进入子文件夹（有权限控制）
    Node* enter(const string& cdName,const int level) {
        Node* now = this->headSon;
        while(now != 0) {
            if (now->name == cdName) {
                if (now->permission > level){
                    cout<<"权限不足！"<<endl;
                    return false;
                }
                if (now->type == 0)
                    return now;
                else {
                    cout<<"你要进入的不是文件夹，无法进入！\n"<<endl;
                    return 0;
                }
            }
            now = now->sibiling;
        }
        cout<<"找不到文件夹，无法进入！\n"<<endl;
        return 0;
    }

    //返回上级目录
    Node* goBack() {
        if (this->parent == 0) {
            //没有上级目录则返回当前目录
            return this;
        } else {
            //有上级目录则返回
            return this->parent;
        }
    }

    //删除文件或文件夹（有权限控制）
    bool deleteByName(const string& delName,const int level) {
        if (this->headSon->name == delName) {
            Node* tmp = this->headSon->sibiling;
            delete this->headSon;
            this->headSon = tmp;
            return true;
        }
        Node* now = this->headSon->sibiling;
        Node* pre = this->headSon;
        while (now != 0) {
            if (now->name == delName) {
                if (now->permission > level){
                    cout<<"权限不足！"<<endl;
                    return false;
                }
                Node* tmp = now->sibiling;
                delete now;
                pre->sibiling = tmp;
                return true;
            }
            pre = now;
            now = now->sibiling;
        }
        //printf("找不到要删除的文件或文件夹\n");
        return false;
    }

    //打开文件：查看文件内容（有权限控制）
    bool openFile(const string& findName,const int level) {
        Node* now = this->headSon;
        while (now != 0) {
            if (now->name == findName) {
                if (now->type != 1) {
                    cout<<"你输入的不是文件名而是文件夹名！"<<endl;
                    return false;
                }
                if (now->permission > level){
                    cout<<"权限不足！"<<endl;
                    return false;
                }
                for (int i = 0; i < now->content.size(); i++) {
                    cout<<i<<" "<<now->content[i]<<endl;
                }
                return true;
            }
        }
        cout<<"文件不存在！"<<endl;
        return false;
    }

    //向文件尾部追加内容（有权限控制）
    bool addContent(const string& findName,const string&  newContent,const int level) {
        Node* now = this->headSon;
        while (now != 0) {
            if (now->name == findName) {
                if (now->type != 1) {
                    cout<<"你输入的不是文件名而是文件夹名！"<<endl;
                    return false;
                }
                if (now->permission > level){
                    cout<<"权限不足！"<<endl;
                    return false;
                }
                now->content.push_back(newContent);
                return true;
            }
        }
        cout<<"文件不存在！"<<endl;
        return false;
    }

    ////向文件中间插入内容
    bool insertFile(const string& findName,const int pos,const string& newContent,const int level){
        Node* now = this->headSon;
        while (now != 0) {
            if (now->name == findName) {
                if (now->type != 1) {
                    cout<<"你输入的不是文件名而是文件夹名！"<<endl;
                    return false;
                }
                if (now->permission > level){
                    cout<<"权限不足！"<<endl;
                    return false;
                }
                if (pos >= now->content.size()) {
                    cout<<"你插入的序号太大！"<<endl;
                    return false;
                }
                now->content.insert(now->content.begin() + pos, newContent);
                return true;
            }
        }
        cout<<"文件不存在！"<<endl;
        return false;
    }

    //析构函数
    ~Node() {
        if (this->type == 1) {
            //pass
        } else {
            Node* now=this->headSon;
            while(now != 0) {
                Node* next = now->sibiling;
                delete now;
                now = next;
            }
            this->headSon=0;
        }
    }

};


/*
用户------------------------------------------------------------------------------
*/
struct User {
    string userName;//用户名
    string password;//密码
    int level;//用户权限等级
    User() {
    }
    User(const string& userName,const string&password,const int level) {
        this->userName=userName;
        this->password=password;
        this->level=level;
    }
};


/*
全局变量声明
*/
vector<User> userList;//用户列表
//初始化------------------------------------------------------------------------------
Node* root = new Node();  //根目录
Node* now = root; //当前目录
User* nowUser;
bool flag = false;
bool init_by_flag() {
    if (flag == false) {
        userList.push_back(User("guest","guest",0));    //加入客户端账号
        userList.push_back(User("root","root",255));    //加入管理员账号
        nowUser = &userList[0];   //设置当前用户为客户账号
        initHelp(); //初始化帮助列表
        flag = true;
    }
}
//初始化结束------------------------------------------------------------------------------


//在用户列表找用户，返回用户结构体地址
User* findUser(const string& userName,const string&password) {
    for (int i = 0; i < userList.size(); i++) {
        if (userList[i].userName == userName) {
            if (userList[i].password == password) {
                return &userList[i];
            } else {
                cout<<"密码不正确！"<<endl;
                return 0;
            }
        }
    }
    cout<<"该用户不存在！"<<endl;
    return 0;
}

//创建用户
bool createUser(const string& userName,const string&password,const int level,const int nowLevel){
    if (level > nowLevel){
        cout<<"无法创建比当前用户权限更高的用户！"<<endl;
        return false;
    }
    for (int i = 0; i < userList.size(); i++) {
        if (userList[i].userName == userName) {
            cout<<"该用户已存在！"<<endl;
            return false;
        }
    }
    userList.push_back(User(userName,password,level));
    cout<<"用户创建成功！";
    return true;
}

//查看用户列表
void listUser() {
    for (int i=0;i<userList.size();i++) {
        cout<<"用户名："<<userList[i].userName<<" 权限："<<userList[i].level<<endl;
    }
}



int main2() {

    init_by_flag();

    string order,value1,value2; //分别为命令，参数1，参数2

    cout<<"------------------------------------------------------------------------------\n"
        <<"******************************************************************************\n"
        <<"***************************** 欢迎来到  文件系统 *****************************\n"
        <<"******************************************************************************\n"
        <<"------------------------------------------------------------------------------\n"
        <<endl;

    cout<<"------------------------------------------------------------------------------\n"
        <<"初始用户是guest，密码是guest。另有超级管理员账号root，密码root\n"
        <<"输入help获取命令详情。\n"
        <<"------------------------------------------------------------------------------\n"
        <<endl;

    while(true) {
        //输出 路径 and 用户及权限
        cout<<"\n~";
        for (int i = 0; i < path.size(); i++) {
            cout<<"/"<<path[i];
        }
        cout<<" ["<<nowUser->userName
            <<" 权限:"<<nowUser->level<<"]";
        cout<<": ";

        //获取输入命令
        cin>>order;
        if (order == "help") {
        //输出帮助命令
            getHelp();
        } else if (order == "createfile" || order == "touch") {
        //创建文件
            cout<<"输入文件名 : ";
            cin>>value1;
            if (now->isExist(value1) != 0) {
                cout<<"文件已存在！"<<endl;
            } else {
                cout<<"输入新建文件内容 : ";
                cin.get();//去掉\r\n
                getline(cin,value2);
                if (now->createFile(value1,value2,nowUser->level)) {
                    cout<<"文件创建成功。"<<endl;
                }
            }
        } else if (order == "createfolder" || order == "mkdir") {
        //创建文件夹
            cout<<"输入文件夹名称 : ";
            cin>>value1;
            if (now->createFolder(value1,nowUser->level)) {
                cout<<"文件夹创建成功。"<<endl;
            }
        } else if (order == "list" || order == "ls") {
        //查看列表
            now->listAll();
        } else if (order == "enter" || order == "cd") {
        //进入子文件夹
            cin>>value1;
            Node* next = now->enter(value1,nowUser->level);
            if (next != 0) {
                //扔入当前路径
                path.push_back(value1);
                now = next;
            }
        } else if (order == "goback") {
        //返回上级目录
            if (now != root)
                path.pop_back();    //非根目录的话，弹出当前路径
            now = now->goBack();
        } else if (order == "delete" || order == "rm") {
        //删除文件或文件夹
            cin>>value1;
            if (now->deleteByName(value1,nowUser->level)) {
                cout<<"删除成功。"<<endl;
            } else {
                cout<<"删除失败！"<<endl;
            }
        } else if (order == "open" || order == "read") {
        //打开文件：查看文件内容
            cout<<"输入文件名称 : ";
            cin>>value1;
            now->openFile(value1,nowUser->level);
        } else if (order == "add" || order == "append") {
        //向文件尾部追加文件内容
            cout<<"输入文件名称 : ";
            cin>>value1;
            cout<<"追加的文件内容 : ";
            cin.get();
            getline(cin,value2);
            now->addContent(value1,value2,nowUser->level);
        } else if (order == "insert"){
        //向文件中间插入内容
            cout<<"输入文件名称 : ";
            cin>>value1;
            if (now->openFile(value1,nowUser->level)){
                int vt;
                cout<<"输入要插在哪一行之前 : ";
                cin>>vt;
                cout<<"输入要插入的内容 : "<<endl;
                cin.get();
                getline(cin,value2);
                now->insertFile(value1,vt,value2,nowUser->level);
            }
        } else if (order == "login") {
        //切换用户登录
            cout<<"请输入用户名 : ";
            cin>>value1;
            cout<<"请输入密码 : ";
            cin>>value2;
            User* tmp = findUser(value1,value2);
            if (tmp != 0) {
                nowUser = tmp;
            }
        } else if (order == "createuser") {
        //创建用户
            cout<<"请输入要创建的用户名 : "<<endl;
            cin>>value1;
            cout<<"请输入要创建用户的密码 : "<<endl;
            cin>>value2;
            cout<<"请输入要创建用户的权限。不能高于当前用户的权限 : "<<endl;
            int vt;
            cin>>vt;
            createUser(value1,value2,vt,nowUser->level);
        } else if (order=="listuser") {
        //查看用户列表
            listUser();
        } else if (order == "exit") {
        //退出文件系统
            break;
        } else {
            cout<<"无法识别命令！"<<endl;
        }

    }

    return 0;
}
