#include <iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<vector>
#include<string>
using namespace std;

/*
ȫ�ֱ�������
*/
vector<string> helpList;//�����б�
vector<string> path;//·���б�

/*
��ʼ�������б�
*/
void initHelp() {
    helpList.push_back("help : ��ѯָ���");
    helpList.push_back("createfile/touch : �����ļ�");
    helpList.push_back("createfolder/mkdir : �����ļ���");
    helpList.push_back("list/ls : �г���ǰ�����ļ����ļ���");
    helpList.push_back("enter/cd : �����ļ���");
    helpList.push_back("goback : �ص���һ��");
    helpList.push_back("delete/rm : ɾ���ļ����ļ���");
    helpList.push_back("open/read : �Ķ��ļ�");
    helpList.push_back("add/append : ��ĳ���ļ���׷������");
    helpList.push_back("insert : ��ĳ���ļ����м��������");
    helpList.push_back("exit : �˳��ļ�ϵͳ");
    helpList.push_back("createuser : �����û�");
    helpList.push_back("listuser : �г������û�");
    helpList.push_back("login : ��¼��ĳ���û�");
}
/*
�������˵��
*/
void getHelp() {
    cout<<"**************�����б�**************"<<endl;
    for (int i=0; i<helpList.size(); i++)
        cout<<helpList[i]<<endl;
    cout<<"************************************"<<endl;
}


/*
�ļ���------------------------------------------------------------------------------
*/
struct Node {
    Node* parent;//���ڵ�
    Node* headSon;//��һ���ӽڵ�
    Node* sibiling;//��һ���ֵܽڵ�
    string name;//�ļ���
    vector<string> content;//����
    int permission;//Ȩ�޵ȼ�
    int type;//0���ļ��У�1���ļ�

    //��ʼ������
    Node() {
        parent = 0;
        headSon = 0;
        sibiling = 0;
        permission = 0;
        type = 0;
    }

    //�ж������Ƿ��Ѿ�ʹ�ã����ؽڵ㡣
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

    //�����ļ������ļ����ݡ��ļ�Ȩ�޵ȼ������ļ���������ɵ�һ�����ӣ�
    bool createFile(const string& newName,const string& newContent,const int level) {
        Node* exist = this->isExist(newName);
        if (exist) {
            printf("�ļ����ļ����Ѵ��ڣ�\n");
            return false;
        }
        Node* file = new Node();
        file->name = newName;
        file->type = 1;
        file->content.push_back(newContent);
        //����ɵ�һ������
        Node* tmp = this->headSon;
        this->headSon = file;
        file->sibiling = tmp;
        file->parent = this;
        file->permission = level;
        return true;
    }

    //�����ļ��С��ļ���Ȩ�޵ȼ������ļ���������ɵ�һ�����ӣ�
    bool createFolder(const string& newName,const int level) {
        Node* exist = this->isExist(newName);
        if (exist) {
            printf("�ļ����ļ����Ѵ��ڣ�\n");
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

    //��������ļ��б�
    void listAll() {
        Node* now = this->headSon;
        while (now != 0) {
            cout<<now->name<<" ";
            if (now->type == 0) cout<<"[�ļ���]"<<endl;
            else cout<<"[�ļ�]"<<endl;
            now = now->sibiling;
        }
    }

    //�������ļ��У���Ȩ�޿��ƣ�
    Node* enter(const string& cdName,const int level) {
        Node* now = this->headSon;
        while(now != 0) {
            if (now->name == cdName) {
                if (now->permission > level){
                    cout<<"Ȩ�޲��㣡"<<endl;
                    return false;
                }
                if (now->type == 0)
                    return now;
                else {
                    cout<<"��Ҫ����Ĳ����ļ��У��޷����룡\n"<<endl;
                    return 0;
                }
            }
            now = now->sibiling;
        }
        cout<<"�Ҳ����ļ��У��޷����룡\n"<<endl;
        return 0;
    }

    //�����ϼ�Ŀ¼
    Node* goBack() {
        if (this->parent == 0) {
            //û���ϼ�Ŀ¼�򷵻ص�ǰĿ¼
            return this;
        } else {
            //���ϼ�Ŀ¼�򷵻�
            return this->parent;
        }
    }

    //ɾ���ļ����ļ��У���Ȩ�޿��ƣ�
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
                    cout<<"Ȩ�޲��㣡"<<endl;
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
        //printf("�Ҳ���Ҫɾ�����ļ����ļ���\n");
        return false;
    }

    //���ļ����鿴�ļ����ݣ���Ȩ�޿��ƣ�
    bool openFile(const string& findName,const int level) {
        Node* now = this->headSon;
        while (now != 0) {
            if (now->name == findName) {
                if (now->type != 1) {
                    cout<<"������Ĳ����ļ��������ļ�������"<<endl;
                    return false;
                }
                if (now->permission > level){
                    cout<<"Ȩ�޲��㣡"<<endl;
                    return false;
                }
                for (int i = 0; i < now->content.size(); i++) {
                    cout<<i<<" "<<now->content[i]<<endl;
                }
                return true;
            }
        }
        cout<<"�ļ������ڣ�"<<endl;
        return false;
    }

    //���ļ�β��׷�����ݣ���Ȩ�޿��ƣ�
    bool addContent(const string& findName,const string&  newContent,const int level) {
        Node* now = this->headSon;
        while (now != 0) {
            if (now->name == findName) {
                if (now->type != 1) {
                    cout<<"������Ĳ����ļ��������ļ�������"<<endl;
                    return false;
                }
                if (now->permission > level){
                    cout<<"Ȩ�޲��㣡"<<endl;
                    return false;
                }
                now->content.push_back(newContent);
                return true;
            }
        }
        cout<<"�ļ������ڣ�"<<endl;
        return false;
    }

    ////���ļ��м��������
    bool insertFile(const string& findName,const int pos,const string& newContent,const int level){
        Node* now = this->headSon;
        while (now != 0) {
            if (now->name == findName) {
                if (now->type != 1) {
                    cout<<"������Ĳ����ļ��������ļ�������"<<endl;
                    return false;
                }
                if (now->permission > level){
                    cout<<"Ȩ�޲��㣡"<<endl;
                    return false;
                }
                if (pos >= now->content.size()) {
                    cout<<"���������̫��"<<endl;
                    return false;
                }
                now->content.insert(now->content.begin() + pos, newContent);
                return true;
            }
        }
        cout<<"�ļ������ڣ�"<<endl;
        return false;
    }

    //��������
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
�û�------------------------------------------------------------------------------
*/
struct User {
    string userName;//�û���
    string password;//����
    int level;//�û�Ȩ�޵ȼ�
    User() {
    }
    User(const string& userName,const string&password,const int level) {
        this->userName=userName;
        this->password=password;
        this->level=level;
    }
};


/*
ȫ�ֱ�������
*/
vector<User> userList;//�û��б�
//��ʼ��------------------------------------------------------------------------------
Node* root = new Node();  //��Ŀ¼
Node* now = root; //��ǰĿ¼
User* nowUser;
bool flag = false;
bool init_by_flag() {
    if (flag == false) {
        userList.push_back(User("guest","guest",0));    //����ͻ����˺�
        userList.push_back(User("root","root",255));    //�������Ա�˺�
        nowUser = &userList[0];   //���õ�ǰ�û�Ϊ�ͻ��˺�
        initHelp(); //��ʼ�������б�
        flag = true;
    }
}
//��ʼ������------------------------------------------------------------------------------


//���û��б����û��������û��ṹ���ַ
User* findUser(const string& userName,const string&password) {
    for (int i = 0; i < userList.size(); i++) {
        if (userList[i].userName == userName) {
            if (userList[i].password == password) {
                return &userList[i];
            } else {
                cout<<"���벻��ȷ��"<<endl;
                return 0;
            }
        }
    }
    cout<<"���û������ڣ�"<<endl;
    return 0;
}

//�����û�
bool createUser(const string& userName,const string&password,const int level,const int nowLevel){
    if (level > nowLevel){
        cout<<"�޷������ȵ�ǰ�û�Ȩ�޸��ߵ��û���"<<endl;
        return false;
    }
    for (int i = 0; i < userList.size(); i++) {
        if (userList[i].userName == userName) {
            cout<<"���û��Ѵ��ڣ�"<<endl;
            return false;
        }
    }
    userList.push_back(User(userName,password,level));
    cout<<"�û������ɹ���";
    return true;
}

//�鿴�û��б�
void listUser() {
    for (int i=0;i<userList.size();i++) {
        cout<<"�û�����"<<userList[i].userName<<" Ȩ�ޣ�"<<userList[i].level<<endl;
    }
}



int main2() {

    init_by_flag();

    string order,value1,value2; //�ֱ�Ϊ�������1������2

    cout<<"------------------------------------------------------------------------------\n"
        <<"******************************************************************************\n"
        <<"***************************** ��ӭ����  �ļ�ϵͳ *****************************\n"
        <<"******************************************************************************\n"
        <<"------------------------------------------------------------------------------\n"
        <<endl;

    cout<<"------------------------------------------------------------------------------\n"
        <<"��ʼ�û���guest��������guest�����г�������Ա�˺�root������root\n"
        <<"����help��ȡ�������顣\n"
        <<"------------------------------------------------------------------------------\n"
        <<endl;

    while(true) {
        //��� ·�� and �û���Ȩ��
        cout<<"\n~";
        for (int i = 0; i < path.size(); i++) {
            cout<<"/"<<path[i];
        }
        cout<<" ["<<nowUser->userName
            <<" Ȩ��:"<<nowUser->level<<"]";
        cout<<": ";

        //��ȡ��������
        cin>>order;
        if (order == "help") {
        //�����������
            getHelp();
        } else if (order == "createfile" || order == "touch") {
        //�����ļ�
            cout<<"�����ļ��� : ";
            cin>>value1;
            if (now->isExist(value1) != 0) {
                cout<<"�ļ��Ѵ��ڣ�"<<endl;
            } else {
                cout<<"�����½��ļ����� : ";
                cin.get();//ȥ��\r\n
                getline(cin,value2);
                if (now->createFile(value1,value2,nowUser->level)) {
                    cout<<"�ļ������ɹ���"<<endl;
                }
            }
        } else if (order == "createfolder" || order == "mkdir") {
        //�����ļ���
            cout<<"�����ļ������� : ";
            cin>>value1;
            if (now->createFolder(value1,nowUser->level)) {
                cout<<"�ļ��д����ɹ���"<<endl;
            }
        } else if (order == "list" || order == "ls") {
        //�鿴�б�
            now->listAll();
        } else if (order == "enter" || order == "cd") {
        //�������ļ���
            cin>>value1;
            Node* next = now->enter(value1,nowUser->level);
            if (next != 0) {
                //���뵱ǰ·��
                path.push_back(value1);
                now = next;
            }
        } else if (order == "goback") {
        //�����ϼ�Ŀ¼
            if (now != root)
                path.pop_back();    //�Ǹ�Ŀ¼�Ļ���������ǰ·��
            now = now->goBack();
        } else if (order == "delete" || order == "rm") {
        //ɾ���ļ����ļ���
            cin>>value1;
            if (now->deleteByName(value1,nowUser->level)) {
                cout<<"ɾ���ɹ���"<<endl;
            } else {
                cout<<"ɾ��ʧ�ܣ�"<<endl;
            }
        } else if (order == "open" || order == "read") {
        //���ļ����鿴�ļ�����
            cout<<"�����ļ����� : ";
            cin>>value1;
            now->openFile(value1,nowUser->level);
        } else if (order == "add" || order == "append") {
        //���ļ�β��׷���ļ�����
            cout<<"�����ļ����� : ";
            cin>>value1;
            cout<<"׷�ӵ��ļ����� : ";
            cin.get();
            getline(cin,value2);
            now->addContent(value1,value2,nowUser->level);
        } else if (order == "insert"){
        //���ļ��м��������
            cout<<"�����ļ����� : ";
            cin>>value1;
            if (now->openFile(value1,nowUser->level)){
                int vt;
                cout<<"����Ҫ������һ��֮ǰ : ";
                cin>>vt;
                cout<<"����Ҫ��������� : "<<endl;
                cin.get();
                getline(cin,value2);
                now->insertFile(value1,vt,value2,nowUser->level);
            }
        } else if (order == "login") {
        //�л��û���¼
            cout<<"�������û��� : ";
            cin>>value1;
            cout<<"���������� : ";
            cin>>value2;
            User* tmp = findUser(value1,value2);
            if (tmp != 0) {
                nowUser = tmp;
            }
        } else if (order == "createuser") {
        //�����û�
            cout<<"������Ҫ�������û��� : "<<endl;
            cin>>value1;
            cout<<"������Ҫ�����û������� : "<<endl;
            cin>>value2;
            cout<<"������Ҫ�����û���Ȩ�ޡ����ܸ��ڵ�ǰ�û���Ȩ�� : "<<endl;
            int vt;
            cin>>vt;
            createUser(value1,value2,vt,nowUser->level);
        } else if (order=="listuser") {
        //�鿴�û��б�
            listUser();
        } else if (order == "exit") {
        //�˳��ļ�ϵͳ
            break;
        } else {
            cout<<"�޷�ʶ�����"<<endl;
        }

    }

    return 0;
}
