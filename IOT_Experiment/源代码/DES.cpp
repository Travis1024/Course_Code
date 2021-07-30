#include <iostream>
#include <string>
using namespace std;
/*---------------------------变换表初始化-----------------------------*/
//---S盒，8组6bit
int S_Box[8][4][16]={
        {
                {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
                {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
                {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
                {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
        },
        {
                {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
                {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
                {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
                {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
        },
        {
                {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
                {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
                {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
                {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
        },
        {
                {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
                {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
                {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
                {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
        },
        {
                {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
                {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
                {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
                {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
        },
        {
                {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
                {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
                {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
                {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
        },
        {
                {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
                {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
                {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
                {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
        },
        {
                {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
                {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
                {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
                {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
        }
};
//E扩展，拓展每个Rn-1，将其从32位拓展到48位，通过此表来重复实现
int E_Table[48]={
        32,1,2,3,4,5,4,5,6,7,8,9,
        8,9, 10, 11,12,13,12,13,14,15,16,17,
        16,17,18,19,20,21,20,21,22,23,24,25,
        24,25,26,27,28,29,28,29,30,31,32,1
};
//对于明文数据M，通过IP进行重新变换
int IP[8][8]={
        {58,50,42,34,26,18,10,2},
        {60,52,44,36,28,20,12,4},
        {62,54,46,38,30,22,14,6},
        {64,56,48,40,32,24,16,8},
        {57,49,41,33,25,17,9, 1},
        {59,51,43,35,27,19,11,3},
        {61,53,45,37,29,21,13,5},
        {63,55,47,39,31,23,15,7}
};
//密文逆置换
int IP_Inverse[8][8]={
        {40,8,48,16,56,24,64,32},
        {39,7,47,15,55,23,63,31},
        {38,6,46,14,54,22,62,30},
        {37,5,45,13,53,21,61,29},
        {36,4,44,12,52,20,60,28},
        {35,3,43,11,51,19,59,27},
        {34,2,42,10,50,18,58,26},
        {33,1,41, 9,49,17,57,25}
};
//64->56选择，得到k+，分割为C\D
int PC_1[8][7]={
        {57,49,41,33,25,17, 9},
        {1,58,50, 42,34,26,18},
        {10, 2,59,51,43,35,27},
        {19,11, 3,60,52,44,36},
        {63,55,47,39,31,23,15},
        {7,62,54, 46,38,30,22},
        {14, 6,61,53,45,37,29},
        {21,13, 5,28,20,12, 4}
};
//秘钥置换，56->48，使得CD组合变为新密钥Kn
int PC_2[8][6]={
        {14,17,11,24,1,5},
        {3,28,15,6,21,10},
        {23,19,12,4,26,8},
        {16,7,27,20,13,2},
        {41,52,31,37,47,55},
        {30,40,51,45,33,48},
        {44,49,39,56,34,53},
        {46,42,50,36,29,32}
};
//P置换及左移操作
int P[32] = {16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25};
int Left_Shift[16]={1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

/*------------------------------全局参数声明------------------------------------*/
int L[32]={0},R[32]={0},C[28]={0},D[28]={0};
int SubKey_ByPC2[16][48]={0};

/*--------------------------------函数部分--------------------------------------*/
//十六进制字符串，转换成二进制数组
int** HexToBinary(string original){
    int x[original.size()],middle_data[4*original.size()],num=0;
    int** result=new int*[8];
    for (int i = 0; i < 8; ++i)
        *(result+i)=new int[8];
    for (int i = 0; i < original.size(); ++i) {
        if(original[i]>'F'&&original[i]<='Z') original[i]='F';
        else if(original[i]>'f'&&original[i]<='z') original[i]='f';
        if(original[i]>='0'&&original[i]<='9'){
            x[i]=original[i]-48;    //0的ASCLL为48
        }else if(original[i]>='A'&&original[i]<='F'){
            x[i]=original[i]-55;    //A的ASCLL为65，减10
        }else if(original[i]>='a'&&original[i]<='f'){
            x[i]=original[i]-87;    //a的ASCLL为97，减10
        }
        middle_data[4*i] = x[i] / 8;
        middle_data[4*i+1] = (x[i] - middle_data[4*i] * 8) / 4;
        middle_data[4*i+2] = (x[i] - middle_data[4*i] * 8 - middle_data[4*i+1] * 4) / 2;
        middle_data[4*i+3] = x[i] - middle_data[4*i] * 8 - middle_data[4*i+1] * 4 - middle_data[4*i+2] * 2;
    }
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            result[i][j]= middle_data[num++];
    return result;
}
//-------二进制转化成十六进制
string BinaryToHex(int a[]){
    string result;
    result.resize(16);
    for (int i = 0,j=0; i < 64; i+=4,++j) {
        int sum=a[i] * 8 + a[i + 1] * 4 + a[i + 2] * 2 + a[i + 3];
        if(sum<10) result[j]=sum+'0';
        else if(sum==10)  result[j]='A';
        else if(sum==11)  result[j]='B';
        else if(sum==12)  result[j]='C';
        else if(sum==13)  result[j]='D';
        else if(sum==14)  result[j]='E';
        else if(sum==15)  result[j]='F';
    }
    return result;
}

//通过PC-1变换产生第一轮秘钥（64位密钥->56位)，同时生成C0/D0
int** Create_SubKey(int** key){
    int** SubKey=new int*[8];
    for (int i = 0; i < 8; ++i)
        *(SubKey+i)=new int[7];
    int middle_data[64],num=0,C_num=0,D_num=0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            middle_data[num++]=key[i][j];
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 7; ++j) {
            SubKey[i][j]=middle_data[PC_1[i][j] - 1];
            if(C_num<28) C[C_num++]=SubKey[i][j]; //56位分为28+28，分别存到C/D中
            else D[D_num++]=SubKey[i][j];
        }
    }
    return SubKey;
}

//循环产生子密钥，C1-C16,D1-D16,由C0、D0推导
void Cycle_SubKey(int n){
    int times=Left_Shift[n],C0,D0;
    for (int i = 0; i < times; ++i) {
        C0=C[0];
        D0=D[0];
        for (int j = 0; j < 28; ++j) {
            C[j - 1] = C[j];
            D[j - 1] = D[j];
        }
        C[27] = C0;
        D[27] = D0;
    }
}
//通过PC-2,使56位子密钥变为48位，并记录
void TurnInto_NewKey(int n){
    int** NewSubKey=new int*[8];
    for (int i = 0; i < 8; ++i)
        *(NewSubKey+i)=new int[6];
    int NowCD[56],NewKey[48],num=0;
    for (int i = 0; i < 56; ++i) {
        if(i<28) NowCD[i]=C[i];
        else NowCD[i]=D[i-28];
    }
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 6; ++j) {
            NewKey[num++]=NowCD[PC_2[i][j]-1];
            SubKey_ByPC2[15-n][num-1]=NewKey[num-1];
        }
    }
}
//通过IP变换初始化L,R
void IP_Transform(int** original){
    int ori_data[64],num=0,middle_data[64];
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            ori_data[num++]=original[i][j];
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            middle_data[8*i+j]=ori_data[IP[i][j]-1];
            if(8*i+j<32) L[8*i+j]=middle_data[8*i+j];
            else R[8*i+j-32]=middle_data[8*i+j];
        }
    }
}
/*为了计算F，我们首先拓展每个Rn-1，将其从32位拓展到48位。这是通过使用表E
 * 来重复Rn-1中的一些位来实现。也就是说函数E(Rn-1)输入32位输出48位。*/
int* F(int n,int flag){
    int *result=new int[32],** B=new int*[8];
    for (int i = 0; i < 8; ++i)
        *(B+i)=new int[6];
    int middle_ER[48],NowK[48],a[48],first,second,middle_all_s[8],result_s[32];
    for (int i = 0; i < 48; ++i) {
        middle_ER[i]=R[E_Table[i]-1];  //使用表E扩展位数
        NowK[i]=SubKey_ByPC2[16-n][i]; //取原先记录的子密钥
    }
    //逆变换，为了实现解密过程准备
    if(flag==1){
        for (int i = 0; i < 48; ++i) {
            NowK[i]=SubKey_ByPC2[n-1][i];
        }
    }
    for (int i = 0; i < 48; ++i)
        a[i]=(NowK[i]+middle_ER[i])%2;   //进行XOR运算
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 6; ++j)      //将8组6比特数据记录在B中
            B[i][j]=a[6*i+j];
    /*B的第一位和最后一位组合起来的二进制数决定一个介于0和3之间的十进制数（或者二进制00到11之间）。
     * 设这个数为i。B的中间4位二进制数代表一个介于0到15之间的十进制数（二进制0000到1111）。
     * 设这个数为j。查表找到第i行第j列的那个数*/
    for (int i = 0; i < 8; ++i) {
        first=B[i][0]*2+B[i][5];         //将8组数据输入S函数
        second=B[i][1] * 8 + B[i][2] * 4 + B[i][3] * 2 + B[i][4];
        middle_all_s[i]=S_Box[i][first][second];
    }
    for (int i = 0; i < 8; ++i) {
        result_s[i*4]=middle_all_s[i]/8;
        result_s[i*4+1]=(middle_all_s[i]-result_s[i*4]*8)/4;
        result_s[i*4+2]=(middle_all_s[i]-result_s[i*4]*8-result_s[i*4+1]*4)/2;
        result_s[i*4+3]=middle_all_s[i]-result_s[i*4]*8-result_s[i*4+1]*4-result_s[i*4+2]*2;
    }
    //对S盒的输出转换成二进制之后，进行P变换，得到32位数据
    for (int i = 0; i < 32; ++i)
        result[i]=result_s[P[i]-1];
    return result;
}
/*通次此函数进行16次迭代后得到L1-L16,R1-R16
* Ln = Rn-1 ；Rn = Ln-1 + F(Rn-1,Kn)；*/
void Update_LR(int n,int flag){
    int middle_L[32],middle_R[32];
    for (int i = 0; i < 32; ++i) {
        middle_L[i]=L[i];
        middle_R[i]=R[i];
    }
    int* result=new int[32];
    //在F函数中实现F(Rn-1,Kn)
    result=F(n,flag);
    for (int i = 0; i < 32; ++i) {
        L[i]=middle_R[i];
        R[i]=(result[i]+middle_L[i])%2;
    }
    /*在第16个迭代之后，我们有了区块L16 and R16。
     * 接着我们逆转两个区块的顺序得到一个64位的区块*/
    if(n==16){
        for (int i = 0; i < 32; i++) {
            int temp = L[i];
            L[i] = R[i];
            R[i] = temp;
        }
    }
}

//实现DES的解密过程，只是中间存在一步子密钥顺序的调换，其他相同
void Decode(string original){
    int** middle=new int*[8];
    int result[64],Combine_LR[64];
    for (int i = 0; i < 8; ++i)
        middle[i]=new int[8];
    middle=HexToBinary(original);
    IP_Transform(middle);
    for (int i = 1; i <= 16; ++i) {
        Update_LR(i,1);
    }
    for (int j = 0; j < 64; ++j) {
        if(j<32) Combine_LR[j]=L[j];
        else Combine_LR[j]=R[j-32];
    }
    for (int j = 0; j < 8; ++j) {
        for (int k = 0; k < 8; ++k) {
            result[j*8+k]=Combine_LR[IP_Inverse[j][k]-1];
        }
    }
    string Final_result=BinaryToHex(result);
    cout<<"Decryption result: "<<Final_result<<endl;
}
/*------------------------------Main函数，程序入口------------------------------*/
int main() {
    string text="1803040002100000";
    string key="23A4Z77995BC0FF1";
    string cipher_text[16];
    int Result_Cipher_Text[16][64],middle_result[8][8],Combine_LR[64];
    cout << "Text: " << text << endl;
    cout << "Key: " << key << endl;
    int** Target_text=new int*[8],**Target_key=new int*[8],**SubKey=new int*[8];
    for (int i = 0; i < 8; ++i) {
        *(Target_text+i)=new int[8];
        *(Target_key+i)=new int[8];
        *(SubKey+i)=new int[7];
    }
    //将明文转换成二进制，并将64位二进制结果存成8*8的矩阵
    Target_text=HexToBinary(text);
    //将密钥转换成二进制，并将64位二进制结果存成8*8的矩阵
    Target_key=HexToBinary(key);
    //将64位密钥根据表格PC-1进行变换，得到56位新密钥K+，再分为C0,D0
    SubKey=Create_SubKey(Target_key);

    for (int i = 1; i <= 16; ++i) {
        //根据C0,D0J进行16次循环移位，依次得到C1-C16,D1-D16
        Cycle_SubKey(i-1);
        //对子密钥进行拼合，形成CnDn，并按照PC-2执行变换，得到48位密钥，循环16次，得到K1-K16
        TurnInto_NewKey(i-1);
    }   //子密钥处理告一段落
    //对明文数据通过IP进行变换，并将64位二进制数据分为L和R两部分，各32
    IP_Transform(Target_text);
    //进行16次迭代
    for (int i = 1; i <= 16; ++i)  {
        /*通次此函数进行16次迭代后得到L1-L16,R1-R16
         * Ln = Rn-1 ；
         * Rn = Ln-1 + F(Rn-1,Kn)；
        */
        Update_LR(i,0);
        for (int j = 0; j < 64; ++j) {
            if(j<32) Combine_LR[j]=L[j];
            else Combine_LR[j]=R[j-32];
        }
        //对翻转过后的LR执行最终一步IP-1变换
        for (int j = 0; j < 8; ++j) {
            for (int k = 0; k < 8; ++k) {
                Result_Cipher_Text[i-1][j*8+k]=Combine_LR[IP_Inverse[j][k]-1];
            }
        }
        cipher_text[i-1]=BinaryToHex(Result_Cipher_Text[i-1]);
        cout<<"--------------- "<<i<<" -------------"<<endl;
        cout<<"Cipher Text: "<<cipher_text[i-1]<<endl;
    }
    Decode(cipher_text[15]); //对最终第16次加密结果进行解密，查看是否对应初始明文。
    return 0;
}
