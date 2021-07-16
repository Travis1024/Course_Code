#include <iostream>
#include <sstream>
#include<fstream>
#include<string>
#include <stdio.h>
#include <string.h>
#include <deque>
#include <map>
#define Max 10000
using namespace std;
map<string,string> p_manager;
map<string,string> p_reader;
/*----------------书籍信息,读者借阅记录，读者密码，管理员密码类------------------*/
class BookInfo{
public:
    BookInfo(string book_id,string book_name,int book_total_number,int book_free_number){
        this->book_id=book_id;
        this->book_name=book_name;
        this->book_total_number=book_total_number;
        this->book_free_number=book_free_number;
    }
    string InputId();
    string InputName();
    int InputTotalNumber();
    int InputFreeNumber();
protected:
    string book_id,book_name;
    int book_total_number;
    int book_free_number;
};
class ReaderInfo{
public:
    ReaderInfo(string reader_id,string borrow_book_name,int number_borrowed){
        this->reader_id=reader_id;
        this->borrow_book_name=borrow_book_name;
        this->number_borrowed=number_borrowed;
    }
    string InputId();
    string InputBorrowBookName();
    int InputNumberBorrowed();
protected:
    string reader_id;
    string borrow_book_name;
    int number_borrowed;
};
class ManagerPassword{
public:
    ManagerPassword(string manager_id,string manager_password,string manager_name){
        this->manager_id=manager_id;
        this->manager_password=manager_password;
        this->manager_name=manager_name;
    }
    string InputID();
    string InputPassword();
    string InputName();
protected:
    string manager_id,manager_password,manager_name;
};
class ReaderPassword{
public:
    ReaderPassword(string reader_id,string reader_password,string reader_name) {
        this->reader_id = reader_id;
        this->reader_password = reader_password;
        this->reader_name=reader_name;
    }
    string InputID();
    string InputPassword();
    string InputName();
protected:
    string reader_id,reader_password,reader_name;
};
/*-------------------其他函数---------------------*/
void UpdataMap(){
    p_manager.clear();
    p_reader.clear();
    ifstream ins_manager("Manager.txt");
    ifstream ins_reader("Reader.txt");
    while (!ins_manager.eof()){
        string id,password,name;
        ins_manager>>id>>password>>name;
        p_manager.insert(make_pair(id,name));
    }
    while (!ins_reader.eof()){
        string id,password,name;
        ins_reader>>id>>password>>name;
        p_reader.insert(make_pair(id,name));
    }
}
/*------------------书籍信息类功能--------------------*/
string BookInfo::InputId() {
    return book_id;
}
string BookInfo::InputName() {
    return book_name;
}
int BookInfo::InputTotalNumber() {
    return book_total_number;
}
int BookInfo::InputFreeNumber() {
    return book_free_number;
}
/*------------------读者借阅记录类功能--------------------*/
string ReaderInfo::InputId() {
    return reader_id;
}
string ReaderInfo::InputBorrowBookName() {
    return borrow_book_name;
}
int ReaderInfo::InputNumberBorrowed() {
    return number_borrowed;
}
/*------------------管理员密码类------------------*/
string ManagerPassword::InputID() {
    return manager_id;
}
string ManagerPassword::InputPassword() {
    return manager_password;
}
string ManagerPassword::InputName() {
    return manager_name;
}
/*------------------读者密码类--------------------*/
string ReaderPassword::InputID() {
    return reader_id;
}
string ReaderPassword::InputPassword() {
    return reader_password;
}
string ReaderPassword::InputName() {
    return reader_name;
}
/*------------------Common,读者功能,管理员功能类--------------------*/
class Commonfunction{
public:
    BookInfo *b[Max];
    ReaderInfo *g[Max];
    ManagerPassword *m[Max];
    ReaderPassword *r[Max];
    bool Login_Judge(string,int);
    bool ReadData(string,string,int);
    void WriteBookInfo(BookInfo *b);
    void WriteReaderInfo(ReaderInfo *g);
    void WriteReaderPassword(ReaderPassword *r);
    bool Update_BookFreeNumber(string,int,int);
    bool JudgeRepeat(string);
    void Search_BookInfo();
    void Enter_Borrowing_Record();
    void Delete_Borrowing_Record();
    void Alter_ReaderInfo();
    string Deal_Black(string);
    void RemoveBlack(string);
};
class Reader: public Commonfunction{
public:
    void Search_Menu();
};
class Manager: public Commonfunction{
public:
    void Search_Menu();
    void Enter_BookInfo();
    void Alter_BookInfo();
    void Delete_BookInfo();
    void Alter_Borrowing_Record();
    void Enter_ReaderInfo();
    void Delete_ReaderInfo();
    void Search_BorrowHistory();
    void Search_ReaderInfo();
};
/*-------------------Common类功能----------------------*/
string Commonfunction::Deal_Black(string name) {
    for (int i = 0; i < name.size(); ++i) {
        if(name[i]==' ') name[i]='_';
    }
    return name;
}
void Commonfunction::RemoveBlack(string file) {
    fstream targetFile(file, fstream::in | fstream::out);
    string line;//作为读取的每一行
    string temp;//作为缓存使用
    deque<string> noBlankLineQueue;//双向队列,只在队首和队尾操作时性能较好
    //判断文件是否打开
    if (!targetFile) {
        cerr << "Can't Open File!" << endl;
        return;
    }
    //记录文件开始位置
    auto StartPos = targetFile.tellp();
//循环读取判断文件的空行并放入队列
    while (getline(targetFile, line)) {
        if (line.empty()) {
        } else {
            noBlankLineQueue.push_back(line);
        }
    }
    targetFile.close();
    //使用ofstream打开再关闭文件是为了清空源文件
    ofstream emptyFile(file);
    emptyFile.close();
    //重新打开文本文件
    fstream target(file, fstream::out | fstream::in);
    if (!target) {
        cerr << "Can't Open File" << endl;
    }
    //获取队首和队尾
    auto begin = noBlankLineQueue.begin();
    auto end = noBlankLineQueue.end();
    //遍历双向队列中的元素
    //并写回文件
    while (begin != end) {
        temp = *begin;
        target << temp << endl;
        ++begin;
    }
    target.close();
}
bool Commonfunction::Login_Judge(string id,int identity){
    string password;
    cout<<"Please enter your password: ";
    cin>>password;
    if(ReadData(id,password,identity)) {
        cout<<"Login Successful"<<endl;
        return true;
    }
    else {
        cout<<"Incorrect user name or password"<<endl;
        return false;
    }
}
bool Commonfunction::ReadData(string id,string password,int identity) {
    fstream Rin;
    bool judge = false;
    string scanid, scanpassword,scanname;
    if (identity == 1) {
        Rin.open("Manager.txt", ios::in);
        if (!Rin) {
            cout << "No file found, please create file first!" << endl;
            return false;
        }
    } else if (identity == 2){
        Rin.open("Reader.txt", ios::in);
        if (!Rin) {
            cout << "No file found, please create file first!" << endl;
            return false;
        }
    }
    while (!Rin.eof()) {
        Rin >> scanid >> scanpassword>>scanname;
        if (id == scanid && password == scanpassword) judge = true;
    }
    return judge;
}
void Commonfunction::WriteBookInfo(BookInfo *b) {
    ofstream Gout("BookList.txt", ios::app);//打开顾客文件，追加读写，不会覆盖掉之前的所有信息
    Gout << b->InputId() << "\t" << b->InputName() << "\t" << b->InputTotalNumber() << "\t"  <<b->InputFreeNumber()<<endl;
    Gout.close();
    RemoveBlack("BookList.txt");
    cout<<"Operation is successful!"<<endl;
}
void Commonfunction::WriteReaderInfo(ReaderInfo *g) {
    ofstream Gout("ReaderInfoRecord.txt", ios::app);//打开顾客文件，追加读写，不会覆盖掉之前的所有信息
    Gout << g->InputId()<<"\t"<<g->InputBorrowBookName()<<"\t"<<g->InputNumberBorrowed()<<endl;
    Gout.close();
    RemoveBlack("ReaderInfoRecord.txt");
    cout<<"Operation is successful!"<<endl;
}
void Commonfunction::WriteReaderPassword(ReaderPassword *r) {
    ofstream Gout("Reader.txt", ios::app);//打开顾客文件，追加读写，不会覆盖掉之前的所有信息
    Gout << r->InputID() << "\t" << r->InputPassword() << "\t" << r->InputName()<<endl;
    Gout.close();
    RemoveBlack("Reader.txt");
    cout<<"Operation is successful!"<<endl;
}
bool Commonfunction::Update_BookFreeNumber(string book_name,int type, int number) {
    string id,name,totalnumber;int a=1,freenumber;
    fstream Rin;
    ofstream Ron;
    Rin.open("BookList.txt", ios::in);
    if (!Rin) {
        cout << "No file found, please create file first!" << endl;
        return false;
    }
    while (!Rin.eof()){
        Rin >> id >> name >> totalnumber >> freenumber;
        if(name==book_name){
            if(freenumber<number&&type==2){
                cout<<"Lack of books and failure to borrow books!"<<endl;
                Rin.close();
                return false;
            }
            ifstream file("BookList.txt");
            string line;
            int n=a,count = 0;
            ofstream outfile("test.txt", ios::out | ios::trunc);
            while (!file.eof()) {
                getline(file, line);
                if (count != n - 1)
                    outfile << line << endl;
                count++;
            }
            outfile.close();
            file.close();
            ofstream outfile1("BookList.txt", ios::out | ios::trunc);
            fstream file1("test.txt");
            while (!file1.eof()) {
                getline(file1, line);
                outfile1 << line << endl;
            }
            outfile1.close();
            file1.close();
            system("del test.txt");
            Ron.open("BookList.txt", ios::app);
            int c=freenumber;
            if(type==1)
                c+=number;
            else if(type==2)
                c-=number;
            Ron << id << "\t" << name << "\t" << totalnumber << "\t" << c<<endl;
            Ron.close();
            break;
        }
        a++;
    }
    Rin.close();
    RemoveBlack("BookList.txt");
    return true;
}
bool Commonfunction::JudgeRepeat(string Inid) {
    string id,name,password;
    fstream Rin;
    Rin.open("Reader.txt", ios::in);
    while (!Rin.eof()){
        Rin >> id >>password >>name;
        if(id==Inid){
            Rin.close();
            return true;
        }
    }
    Rin.close();
    return false;
}
void Commonfunction::Search_BookInfo() {
    string book_id,book_name,book_total_number,book_free_number;
    int a=1;
    fstream Rin;
    ofstream Ron;
    Rin.open("BookList.txt", ios::in);
    if (!Rin) {
        cout << "No file found, please create file first!" << endl;
        return;
    }
    while (!Rin.eof()){
        Rin>>book_id>>book_name>>book_total_number>>book_free_number;
        if(book_id!="-1") {
            cout<<"Book ID:"<<book_id<<" | Book Name:"<<book_name<<" | Book Total amount:"<<book_total_number<<" | Book Free amount:"<<book_free_number<<endl;
            book_id="-1";
        }
    }
}
void Commonfunction::Enter_Borrowing_Record() {
    string reader_id,borrow_book_name;
    int number_borrowed;
    cout<<"Please enter the reader id: ";
    cin>>reader_id;
    cout<<"Please enter the name of the book you want to borrow: ";
    string str="\n";
    getline(cin,str);
    getline(cin,borrow_book_name);
    borrow_book_name=Deal_Black(borrow_book_name);
    cout<<"Please enter the amount you want to borrow: ";
    cin>>number_borrowed;
    if(Update_BookFreeNumber(borrow_book_name,2,number_borrowed)){
        WriteReaderInfo(new ReaderInfo(reader_id,borrow_book_name,number_borrowed));
    }
}
void Commonfunction::Delete_Borrowing_Record() {
    string targetid,id,name,return_book_name;int xh,a=1,number;
    cout<<"Please enter the Reader ID number you want to delete: ";
    cin>>targetid;
    cout<<"Please enter the name of the book you want to return: ";
    string str="\n";
    getline(cin,str);
    getline(cin,return_book_name);
    return_book_name=Deal_Black(return_book_name);
    fstream Rin;
    Rin.open("ReaderInfoRecord.txt", ios::in);
    if (!Rin) {
        cout << "No file found, please create file first!" << endl;
        return;
    }
    while (!Rin.eof()){
        Rin >> id >> name >> number;
        if (id==targetid&&name==return_book_name){
            ifstream file("ReaderInfoRecord.txt");
            string line;
            int n=a, count = 0;
            ofstream outfile("test.txt", ios::out | ios::trunc);
            while (!file.eof()) {
                getline(file, line);
                if (count != n - 1)//如果要修改内容就在这修改line的内容,再存到文件中就行了
                    outfile << line << endl;
                count++;
            }
            outfile.close();
            file.close();
            ofstream outfile1("ReaderInfoRecord.txt", ios::out | ios::trunc);
            fstream file1("test.txt");
            while (!file1.eof()) {
                getline(file1, line);
                outfile1 << line << endl;
            }
            outfile1.close();
            file1.close();
            system("del test.txt");
            Update_BookFreeNumber(return_book_name,1,number);
            cout<<"Operation is successful!"<<endl;
            break;
        }
        a++;
    }
    Rin.close();
    RemoveBlack("ReaderInfoRecord.txt");
}
void Commonfunction::Alter_ReaderInfo() {
    string targetid,id,name,password;int xh,a=1;
    cout<<"Please enter the reader ID number you want to modify: ";
    cin>>targetid;
    fstream Rin;
    ofstream Ron;
    Rin.open("Reader.txt", ios::in);
    if (!Rin) {
        cout << "No file found, please create file first!" << endl;
        return;
    }
    while (!Rin.eof()){
        Rin >> id >> password>>name;
        if(id==targetid){
            cout<<"ID:"<<id <<" | Password:"<< password <<" | Name:"<< name <<endl;
            cout<<"----------------------------"<<endl;
            cout<<"<1> Change Name"<<endl;
            cout<<"<2> Change Password"<<endl;
            cout<<"----------------------------"<<endl;
            cout<<"Select the modification options you want to perform: ";
            cin>>xh;
            ifstream file("Reader.txt");
            string line;
            int n=a,count = 0;
            ofstream outfile("test.txt", ios::out | ios::trunc);
            while (!file.eof()) {
                getline(file, line);
                if (count != n - 1)
                    outfile << line << endl;
                count++;
            }
            outfile.close();
            file.close();
            ofstream outfile1("Reader.txt", ios::out | ios::trunc);
            fstream file1("test.txt");
            while (!file1.eof()) {
                getline(file1, line);
                outfile1 << line << endl;
            }
            outfile1.close();
            file1.close();
            system("del test.txt");
            Ron.open("Reader.txt", ios::app);
            switch (xh) {
                case 1:{
                    string NewName;
                    cout<<"Please enter the name you want to change: ";
                    string str="\n";
                    getline(cin,str);
                    getline(cin,NewName);
                    NewName=Deal_Black(NewName);
                    Ron << id << "\t" <<password<<"\t"<< NewName<<endl;
                    break;
                }
                case 2:{
                    string NewPassword;
                    cout<<"Please enter the password you want to change: ";
                    cin>>NewPassword;
                    Ron << id << "\t" << NewPassword << "\t" << name<<endl;
                    break;
                }
                default:{
                    cout << "Abnormal input, the system has automatically quit!" << endl;
                    exit(0);
                }
            }
            Ron.close();
        }
        a++;
    }
    cout<<"Operation is successful!"<<endl;
    UpdataMap();
    Rin.close();
    RemoveBlack("Reader.txt");
}
/*-------------------读者类功能----------------------*/
void Reader::Search_Menu() {
    while (true) {
        cout <<endl<< "---------------Library Management System-------------" << endl;
        cout<<"<1> Borrow Books"<<endl;
        cout<<"<2> Return Books"<<endl;
        cout<<"<3> Get the Information of Books"<<endl;
        cout<<"<4> Modify Personal Information"<<endl;
        cout<<"<5> Exit"<<endl;
        cout<<"Please enter the function number you want to select: ";
        int n; cin >> n;
        switch (n) {
            case 1:
                Enter_Borrowing_Record();break;
            case 2:
                Delete_Borrowing_Record();break;
            case 3:
                Search_BookInfo();break;
            case 4:
                Alter_ReaderInfo();break;
            case 5:
                cout << "Thank you for your use, goodbye!" << endl;
                exit(0);
            default:
                cout << "Abnormal input, the system has automatically quit!" << endl;
                exit(0);
        }
    }
}
/*-------------------管理员类功能--------------------*/
void Manager::Search_Menu() {
    while (true) {
        cout <<endl<< "---------------Library Management System-------------" << endl;
        cout<<"<1> Increase Book Information"<<endl;
        cout<<"<2> Change Book Information"<<endl;
        cout<<"<3> Delete Book Information"<<endl;
        cout<<"<4> Increase the Borrowing Record of Readers"<<endl;
        cout<<"<5> Change the Borrowing Record of Readers"<<endl;
        cout<<"<6> Delete the Borrowing Record of Readers"<<endl;
        cout<<"<7> Increase Reader Information"<<endl;
        cout<<"<8> Change Reader Information"<<endl;
        cout<<"<9> Delete Reader Information"<<endl;
        cout<<"<10> Search for Book Information"<<endl;
        cout<<"<11> Search for Borrowing History"<<endl;
        cout<<"<12> Search for Reader Information"<<endl;
        cout<<"<13> Exit"<<endl;
        cout<<"Please enter the function number you want to select: ";
        int n; cin >> n;
        switch (n) {
            case 1:
                Enter_BookInfo();break;
            case 2:
                Alter_BookInfo();break;
            case 3:
                Delete_BookInfo();break;
            case 4:
                Enter_Borrowing_Record();break;
            case 5:
                Alter_Borrowing_Record();break;
            case 6:
                Delete_Borrowing_Record();break;
            case 7:
                Enter_ReaderInfo();break;
            case 8:
                Alter_ReaderInfo();break;
            case 9:
                Delete_ReaderInfo();break;
            case 10:
                Search_BookInfo();break;
            case 11:
                Search_BorrowHistory();break;
            case 12:
                Search_ReaderInfo();break;
            case 13:
                cout << "Thank you for your use, goodbye!" << endl;
                exit(0);
            default:
                cout << "Abnormal input, the system has automatically quit!" << endl;
                exit(0);
        }
    }
}
void Manager::Enter_BookInfo() {
    string id,name;
    int total_number,free_number;
    cout<<"Please enter the book ID number: ";
    cin>>id;
    cout<<"Please enter the book name: ";
    string str="\n";
    getline(cin,str);
    getline(cin,name);
    name=Deal_Black(name);
    cout<<"Please enter the total number of books: ";
    cin>>total_number;
    cout<<"Please enter the total number of free books: ";
    cin>>free_number;
    WriteBookInfo(new BookInfo(id,name,total_number,free_number));
}
void Manager::Alter_BookInfo() {
    string targetid,id,name,totalnumber,freenumber;int xh,a=1;
    cout<<"Please enter the book ID number you want to modify: ";
    cin>>targetid;
    fstream Rin;
    ofstream Ron;
    Rin.open("BookList.txt", ios::in);
    if (!Rin) {
        cout << "No file found, please create file first!" << endl;
        return;
    }
    while (!Rin.eof()){
        Rin >> id >> name >> totalnumber >> freenumber;
        if(id==targetid){
            cout<<"ID:"<<id <<" | Name:"<< name <<" | Total Number:"<< totalnumber <<" | Free Number:"<< freenumber<<endl;
            cout<<"----------------------------"<<endl;
            cout<<"<1> Change Name"<<endl;
            cout<<"<2> Change Total Number"<<endl;
            cout<<"<3> Change Free Number"<<endl;
            cout<<"----------------------------"<<endl;
            cout<<"Select the modification options you want to perform: ";
            cin>>xh;
            ifstream file("BookList.txt");
            string line;
            int n=a,count = 0;
            ofstream outfile("test.txt", ios::out | ios::trunc);
            while (!file.eof()) {
                getline(file, line);
                if (count != n - 1)
                    outfile << line << endl;
                count++;
            }
            outfile.close();
            file.close();
            ofstream outfile1("BookList.txt", ios::out | ios::trunc);
            fstream file1("test.txt");
            while (!file1.eof()) {
                getline(file1, line);
                outfile1 << line << endl;
            }
            outfile1.close();
            file1.close();
            system("del test.txt");
            Ron.open("BookList.txt", ios::app);
            switch (xh) {
                case 1:{
                    string NewName;
                    cout<<"Please enter the name you want to change: ";
                    string str="\n";
                    getline(cin,str);
                    getline(cin,NewName);
                    NewName=Deal_Black(NewName);
                    Ron << id << "\t" << NewName << "\t" << totalnumber << "\t" << freenumber<<endl;
                    break;
                }
                case 2:{
                    int NewNumber;
                    cout<<"Please enter the total number you want to change: ";
                    cin>>NewNumber;
                    Ron << id << "\t" << name << "\t" << NewNumber << "\t" << freenumber<<endl;
                    break;
                }
                case 3:{
                    int NewNumber;
                    cout<<"Please enter the free number you want to change: ";
                    cin>>NewNumber;
                    Ron << id << "\t" << name << "\t" << totalnumber << "\t" << NewNumber<<endl;
                    break;
                }
                default:{
                    cout << "Abnormal input, the system has automatically quit!" << endl;
                    exit(0);
                }
            }
            cout<<"Operation is successful!"<<endl;
            Ron.close();
            break;
        }
        a++;
    }
    Rin.close();
    RemoveBlack("BookList.txt");
}
void Manager::Delete_BookInfo() {
    string targetid,id,name,totalnumber,freenumber;int xh,a=1;
    cout<<"Please enter the book ID number you want to delete: ";
    cin>>targetid;
    fstream Rin;
    Rin.open("BookList.txt", ios::in);
    if (!Rin) {
        cout << "No file found, please create file first!" << endl;
        return;
    }
    while (!Rin.eof()){
        Rin >> id >> name >> totalnumber >> freenumber;
        if (id==targetid){
            ifstream file("BookList.txt");
            string line;
            int n=a, count = 0;
            ofstream outfile("test.txt", ios::out | ios::trunc);
            while (!file.eof()) {
                getline(file, line);
                if (count != n - 1)//如果要修改内容就在这修改line的内容,再存到文件中就行了
                    outfile << line << endl;
                count++;
            }
            outfile.close();
            file.close();
            ofstream outfile1("BookList.txt", ios::out | ios::trunc);
            fstream file1("test.txt");
            while (!file1.eof()) {
                getline(file1, line);
                outfile1 << line << endl;
            }
            outfile1.close();
            file1.close();
            system("del test.txt");
        }
        a++;
    }
    cout<<"Operation is successful!"<<endl;
    Rin.close();
    RemoveBlack("BookList.txt");
}
void Manager::Alter_Borrowing_Record() {
    string targetid,id,book_name,targetname;int xh,a=1,number,NewNumber;
    cout<<"Please enter the reader ID number you want to modify: ";
    cin>>targetid;
    cout<<"Please enter the book name you want to modify: ";
    string str="\n";
    getline(cin,str);
    getline(cin,targetname);
    targetname=Deal_Black(targetname);
    fstream Rin;
    ofstream Ron;
    Rin.open("ReaderInfoRecord.txt", ios::in);
    if (!Rin) {
        cout << "No file found, please create file first!" << endl;
        return;
    }
    while (!Rin.eof()){
        Rin >> id >>book_name>>number;
        if(id==targetid&&book_name==targetname){
            cout<<"ID:"<<id <<" | Name:"<< p_reader[id]<<" | Book Name:"<< book_name <<" | Number:"<< number<<endl;
            cout<<"-------------------------------"<<endl;
            cout<<"<1> Modify the Amount Borrowed"<<endl;
            cout<<"--------------------------------"<<endl;
            cout<<"Select the modification options you want to perform: ";
            cin>>xh;
            ifstream file("ReaderInfoRecord.txt");
            string line;
            int n=a,count = 0;
            ofstream outfile("test.txt", ios::out | ios::trunc);
            while (!file.eof()) {
                getline(file, line);
                if (count != n - 1)
                    outfile << line << endl;
                count++;
            }
            outfile.close();
            file.close();
            ofstream outfile1("ReaderInfoRecord.txt", ios::out | ios::trunc);
            fstream file1("test.txt");
            while (!file1.eof()) {
                getline(file1, line);
                outfile1 << line << endl;
            }
            outfile1.close();
            file1.close();
            system("del test.txt");
            Ron.open("ReaderInfoRecord.txt", ios::app);
            switch (xh) {
                case 1:{
                    cout<<"Please enter the number you want to change: ";
                    cin>>NewNumber;
                    if(Update_BookFreeNumber(book_name,2,NewNumber-number)){
                        Ron << id << "\t" << book_name << "\t" << NewNumber<<endl;
                        cout<<"Operation is successful!"<<endl;
                    }
                    break;
                }
                default:{
                    cout << "Abnormal input, the system has automatically quit!" << endl;
                    exit(0);
                }
            }
            Ron.close();
            break;
        }
        a++;
    }
    Rin.close();
    RemoveBlack("ReaderInfoRecord.txt");
}
void Manager::Enter_ReaderInfo() {
    string id,name,password;
    cout<<"Please enter the reader ID number: ";
    cin>>id;
    if(JudgeRepeat(id)){
        cout<<"The current ID already exists!"<<endl;
        return;
    }
    cout<<"Please enter the password: ";
    cin>>password;
    cout<<"Please enter the reader name: ";
    string str="\n";
    getline(cin,str);
    getline(cin,name);
    name=Deal_Black(name);
    WriteReaderPassword(new ReaderPassword(id,password,name));
    UpdataMap();
}
void Manager::Delete_ReaderInfo() {
    string targetid,id,name,password;int xh,a=1;
    cout<<"Please enter the reader ID number you want to delete: ";
    cin>>targetid;
    fstream Rin;
    Rin.open("Reader.txt", ios::in);
    if (!Rin) {
        cout << "No file found, please create file first!" << endl;
        return;
    }
    while (!Rin.eof()){
        Rin >> id >>password >>name;
        if (id==targetid){
            ifstream file("Reader.txt");
            string line;
            int n=a, count = 0;
            ofstream outfile("test.txt", ios::out | ios::trunc);
            while (!file.eof()) {
                getline(file, line);
                if (count != n - 1)//如果要修改内容就在这修改line的内容,再存到文件中就行了
                    outfile << line << endl;
                count++;
            }
            outfile.close();
            file.close();
            ofstream outfile1("Reader.txt", ios::out | ios::trunc);
            fstream file1("test.txt");
            while (!file1.eof()) {
                getline(file1, line);
                outfile1 << line << endl;
            }
            outfile1.close();
            file1.close();
            system("del test.txt");
        }
        a++;
    }
    cout<<"Operation is successful!"<<endl;
    Rin.close();
    RemoveBlack("Reader.txt");
    UpdataMap();
}
void Manager::Search_BorrowHistory() {
    string id,book_name,number;
    int a=1;
    fstream Rin;
    ofstream Ron;
    Rin.open("ReaderInfoRecord.txt", ios::in);
    if (!Rin) {
        cout << "No file found, please create file first!" << endl;
        return;
    }
    while (!Rin.eof()){
        Rin>>id>>book_name>>number;
        if(id!="-1"){
            cout<<"Reader ID:"<<id<<" | Reader Name:"<<p_reader[id]<<" | Book Name:"<<book_name<<" | Borrowing amount:"<<number<<endl;
            id="-1";
        }
    }
}
void Manager::Search_ReaderInfo() {
    string reader_id,reader_password,reader_name;
    int a=1;
    fstream Rin;
    ofstream Ron;
    Rin.open("Reader.txt", ios::in);
    if (!Rin) {
        cout << "No file found, please create file first!" << endl;
        return;
    }
    while (!Rin.eof()){
        Rin>>reader_id>>reader_password>>reader_name;
        if(reader_id!="-1") {
            cout<<"Reader ID:"<<reader_id<<" | Reader Name:"<<reader_name<<" | Reader Password:"<<reader_password<<endl;
            reader_id="-1";
        }
    }
}
/*-------------------Main函数-----------------------*/
/*int main(int argc,char **argv) {
    UpdataMap();
    if(strcmp(argv[1],"-a")==0){
        Manager manager;
        if(manager.Login_Judge(argv[2],1)){
            manager.Search_Menu();
        }
    } else if (strcmp(argv[1],"-u")==0){
        Reader reader;
        if(reader.Login_Judge(argv[2],2)){
            reader.Search_Menu();
        }
    }
    return 0;
}*/
int main(){
    UpdataMap();
    string a,b;
    cin>>a>>b;
    if(a=="-a"){
        Manager manager;
        if(manager.Login_Judge(b,1)){
            manager.Search_Menu();
        }
    } else if (a=="-u"){
        Reader reader;
        if(reader.Login_Judge(b,2)){
            reader.Search_Menu();
        }
    }
    return 0;
}
