#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
typedef struct point {
    int point_x;
    int point_y;
} Point;
typedef struct ecc {
    struct point p[100];
    int len;
} ECCPoint;
typedef struct generator {
    Point p;
    int p_class;
} Generetor_Create;

Point P1, P2, Pt, G, PB, Pm;
ECCPoint eccPoint;
Generetor_Create generetor_create[100];

int a = 4, b = 20, p = 29, geneLen, nB;   //nB为私钥
int m[100], C[100];
char text[] = "xidian university";
char alphabet[27] = "abcdefghijklmnopqrstuvwxyz";

Point point(int k, Point p);
Point add_pairpoints(Point p1, Point p2);
void Get_Points();
void Print_Points();
int Sqrt(int s);
int Solve_InversaElement(int n, int b);
int P_Mod(int s);
int Judge_Prime(int n);

//加密
void Encrypt() {
    int num, i, j, gene_class, num_t, k;
    srand(time(NULL));
    //明文转换过程
    for (i = 0; i < strlen(text); i++) {
        for (j = 0; j < 26; j++) {
            if (text[i] == alphabet[j]) {
                m[i] = j;//将字符串明文换成数字，并存到整型数组m里面
            }
        }
    }
    //选择生成元
    num = rand() % geneLen;
    gene_class = generetor_create[num].p_class;
    while (Judge_Prime(gene_class) == -1) {//不是素数
        num = rand() % (geneLen - 3) + 3;
        gene_class = generetor_create[num].p_class;
    }
    G = generetor_create[num].p;
    nB = rand() % (gene_class - 1) + 1;//选择私钥
    PB = point(nB, G);
    printf("\nPublic key: ");
    printf("<y^2=x^3+%d*x+%d> | Order: %d | G: (%d,%d) | (%d,%d)\n", a, b, gene_class, G.point_x, G.point_y, PB.point_x, PB.point_y);
    printf("Private key: %d\n",nB);
    //加密
    k = rand() % (gene_class - 2) + 1;
    P1 = point(k, G);
    num_t = rand() % eccPoint.len; //选择映射点
    Pt = eccPoint.p[num_t];
    P2 = point(k, PB);
    Pm = add_pairpoints(Pt, P2);
    printf("Enciphered data:\n");
    printf("kG=(%d,%d) | Pt+kPB=(%d,%d) | C= ", P1.point_x, P1.point_y, Pm.point_x, Pm.point_y);
    for (i = 0; i < strlen(text); i++) {
        C[i] = m[i] * Pt.point_x + Pt.point_y;
        printf("<%d>", C[i]);
    }
    printf(" \n");
}

//解密
void Decrypt() {
    Point temp, temp1;
    int m, i;
    temp = point(nB, P1);
    temp.point_y = 0 - temp.point_y;
    temp1 = add_pairpoints(Pm, temp);//求解Pt
    printf("\nDecryption result:\n");
    for (i = 0; i < strlen(text); i++) {
        m = (C[i] - temp1.point_y) / temp1.point_x;
        printf("%c", alphabet[m]);//输出密文
    }
    printf("\n");
}

//判断是否为素数
int Judge_Prime(int n) {
    int i, k;
    k = sqrt(n);
    for (i = 2; i <= k; i++)
        if (n % i == 0) break;
    if (i <= k) return -1;
    else return 0;
}

//求生成元以及阶
void Get_Generetor() {
    int i, j = 0;
    int count = 1;
    Point p1, p2;
    Get_Points();
    printf("\n\n");
    for (i = 0; i < eccPoint.len; i++) {
        count = 1;
        p1.point_x = p2.point_x = eccPoint.p[i].point_x;
        p1.point_y = p2.point_y = eccPoint.p[i].point_y;
        while (1) {
            p2 = add_pairpoints(p1, p2);
            if (p2.point_x == -1 && p2.point_y == -1)
                break;
            count++;
            if (p2.point_x == p1.point_x) {
                break;
            }
        }
        count++;
        if (count <= eccPoint.len + 1) {
            generetor_create[j].p.point_x = p1.point_x;
            generetor_create[j].p.point_y = p1.point_y;
            generetor_create[j].p_class = count;
            printf("(%d,%d)-[%d]\t",generetor_create[j].p.point_x,generetor_create[j].p.point_y,generetor_create[j].p_class);
            j++;
            if(j % 6 == 0) printf("\n");
        }
        geneLen = j;
    }
}

//t倍点运算的递归算法
Point point(int k, Point p0) {
    if (k == 1) return p0;
    else if (k == 2) return add_pairpoints(p0, p0);
    else return add_pairpoints(p0, point(k - 1, p0));
}

//两点的加法运算
Point add_pairpoints(Point p1, Point p2) {
    long t;
    int x1 = p1.point_x, y1 = p1.point_y;
    int x2 = p2.point_x, y2 = p2.point_y;
    int tx, ty, x3, y3, flag = 0;
    //求
    if ((x2 == x1) && (y2 == y1)) {
        //相同点相加
        if (y1 == 0) flag = 1;
        else t = (3 * x1 * x1 + a) * Solve_InversaElement(p, 2 * y1) % p;
    } else {
        //不同点相加
        ty = y2 - y1;
        tx = x2 - x1;
        while (ty < 0)
            ty += p;
        while (tx < 0)
            tx += p;
        if (tx == 0 && ty != 0) flag = 1;
        else t = ty * Solve_InversaElement(p, tx) % p;
    }
    if (flag == 1) {
        p2.point_x = -1;
        p2.point_y = -1;
    } else {
        x3 = (t * t - x1 - x2) % p;
        y3 = (t * (x1 - x3) - y1) % p;
        //使结果在有限域GF(P)上
        while (x3 < 0)
            x3 += p;
        while (y3 < 0)
            y3 += p;
        p2.point_x = x3;
        p2.point_y = y3;
    }
    return p2;
}

//求b关于n的逆元
int Solve_InversaElement(int n, int b) {
    int q, r, r1 = n, r2 = b, t, t1 = 0, t2 = 1, i = 1;
    while (r2 > 0) {
        q = r1 / r2;
        r = r1 % r2;
        r1 = r2;
        r2 = r;
        t = t1 - q * t2;
        t1 = t2;
        t2 = t;
    }
    if (t1 >= 0)
        return t1 % n;
    else {
        while ((t1 + i * n) < 0)
            i++;
        return t1 + i * n;
    }
}

//求出椭圆曲线上所有点
void Get_Points() {
    int i = 0, j = 0, s, y = 0, n = 0, q = 0;
    int modsqrt = 0, flag = 0;
    if (4 * a * a * a + 27 * b * b != 0) {
        for (i = 0; i <= p - 1; i++) {
            flag = 0;
            n = 1;
            y = 0;
            s = i * i * i + a * i + b;
            while (s < 0) {
                s += p;
            }
            s = P_Mod(s);
            modsqrt = Sqrt(s);
            if (modsqrt != -1) {
                flag = 1;
                y = modsqrt;
            } else {
                while (n <= p - 1) {
                    q = s + n * p;
                    modsqrt = Sqrt(q);
                    if (modsqrt != -1) {
                        y = modsqrt;
                        flag = 1;
                        break;
                    }
                    flag = 0;
                    n++;
                }
            }
            if (flag == 1) {
                eccPoint.p[j].point_x = i;
                eccPoint.p[j].point_y = y;
                j++;
                if (y != 0) {
                    eccPoint.p[j].point_x = i;
                    eccPoint.p[j].point_y = (p - y) % p;
                    j++;
                }
            }
        }
        eccPoint.len = j;//点集个数
        Print_Points(); //打印点集
    }
}

//取模函数
int P_Mod(int s) {
    int i,result;    //i保存s/p的倍数,result保存模运算的结果
    i = s / p;
    result = s - i * p;
    if (result >= 0) return result;
    else return result + p;
}

//判断平方根是否为整数
int Sqrt(int s) {
    int temp;
    temp = (int) sqrt(s);//转为整型
    if (temp * temp == s) return temp;
    else return -1;
}

//打印点集
void Print_Points() {
    int i, len = eccPoint.len;
    printf("--------------------------------------ECC------------------------------------------");
    printf("\nThere are a total of %d points on the elliptic curve. (including infinity points)", len + 1);
    for (i = 0; i < len; i++) {
        if (i % 8 == 0) {
            printf("\n");
        }
        printf("(%2d,%2d)\t", eccPoint.p[i].point_x, eccPoint.p[i].point_y);
    }
    printf("\n");
}