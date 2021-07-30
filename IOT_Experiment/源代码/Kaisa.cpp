#include <iostream>
using namespace std;
class Function{
public:
    Function(string str){
        this->str=str;
    }
    void Encrypt(int);
    void Decode(int);
    void Decode_violent();

private:
    string str,res;
    string result[26];
};

void Function::Encrypt(int key) {
    for (int i = 0; i < str.size(); ++i) {
        if((str[i]<='Z'&&str[i]>='A')||(str[i]<='z'&&str[i]>='a')){
            if((str[i]<='Z'&&str[i]>='A'&&str[i]+key>'Z')||(str[i]<='z'&&str[i]>='a'&&str[i]+key>'z')) res+=str[i]+key-26;
            else res+=str[i]+key;
        } else{
            res+=str[i];
        }
    }
    cout<<"-----------Encryption Result-----------"<<endl;
    cout<<res<<endl;
}

void Function::Decode(int key) {
    for (int i = 0; i < str.size(); ++i) {
        if((str[i]<='Z'&&str[i]>='A')||(str[i]<='z'&&str[i]>='a')){
            if((str[i]<='Z'&&str[i]>='A'&&str[i]-key<'A')||(str[i]<='z'&&str[i]>='a'&&str[i]-key<'a')) res+=str[i]-key+26;
            else res+=str[i]-key;
        } else{
            res+=str[i];
        }
    }
    cout<<"-----------Decryption Result----------"<<endl;
    cout<<res<<endl;
}

void Function::Decode_violent() {
    for (int i = 0; i < str.size(); ++i) {
        for (int j = 1; j < 26; ++j) {
            if((str[i]<='Z'&&str[i]>='A')||(str[i]<='z'&&str[i]>='a')){
                if((str[i]<='Z'&&str[i]>='A'&&str[i]-j<'A')||(str[i]<='z'&&str[i]>='a'&&str[i]-j<'a')) result[j-1]+=str[i]-j+26;
                else result[j-1]+=str[i]-j;
            } else{
                result[j-1]+=str[i];
            }
        }
    }
    for (int i = 0; i < 25; ++i) {
        cout<<"----------KEY: "<<i+1<<"----------"<<endl;
        cout<<result[i]<<endl<<endl;
    }
}

int main() {
    char n[1000]={0};
    int choice=0,a;
    string by="\n";
    cout<<"--------------------Caesar Cipher------------------"<<endl;
    cout<<"<1> Encrypt"<<endl;
    cout<<"<2> Decode"<<endl;
    cout<<"<3> Violence to decrypt"<<endl;
    cout<<"Please select the function you want: ";
    cin>>choice;
    getline(cin,by);
    cout<<"Please enter the string you want to operate on: "<<endl;
    cin.getline(n,1000);
    string str(n);
    Function fc(str);
    switch (choice) {
        case 1:{
            cout<<"Please enter the key: ";
            cin>>a;
            fc.Encrypt(a);
            break;
        }
        case 2:{
            cout<<"Please enter the key: ";
            cin>>a;
            fc.Decode(a);
            break;
        }
        case 3:{
            fc.Decode_violent();
            break;
        }
    }
    return 0;
}
