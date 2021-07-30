class MD5{
    //初始化MD5参数
    static final int S11 = 7;
    static final int S12 = 12;
    static final int S13 = 17;
    static final int S14 = 22;
    static final int S21 = 5;
    static final int S22 = 9;
    static final int S23 = 14;
    static final int S24 = 20;
    static final int S31 = 4;
    static final int S32 = 11;
    static final int S33 = 16;
    static final int S34 = 23;
    static final int S41 = 6;
    static final int S42 = 10;
    static final int S43 = 15;
    static final int S44 = 21;
    //定义标准幻数
    private static final long A=0x67452301L;
    private static final long B=0xefcdab89L;
    private static final long C=0x98badcfeL;
    private static final long D=0x10325476L;
    
    private long [] result={A,B,C,D};  //存储结果
    static final String hexs[]={"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"};
    private String str;

    public MD5(String str) {
        this.str=str;
        String result=Main_process();
        System.out.println("| MD5---<"+str+">"+" | \nResult: "+result);
    }
    
    private String Main_process() {
        byte [] inputBytes=str.getBytes();
        int str_length=inputBytes.length;   //字符串字节长度
        int group_number=0;                 //分组个数，每组512位（64字节）
        group_number=str_length/64;         //字节长度/64字节
        long []groups=null;                 //每组按4字节继续细分

        for(int i=0;i<group_number;i++){  //处理分组，循环运算，循环次数位分组个数
            groups=Create_NewGroup(inputBytes,i*64);
            Transform(groups);                     //处理分组，核心算法
        }

        int rest=str_length%64;            //处理512位分组后的余数
        byte [] tempBytes=new byte[64];
        if(rest<=56){
            for(int i=0;i<rest;i++)
                tempBytes[i]=inputBytes[str_length-rest+i];
            if(rest<56){
                tempBytes[rest]=(byte)(1<<7);
                for(int i=1;i<56-rest;i++)
                    tempBytes[rest+i]=0;
            }
            long len=(long)(str_length<<3);
            for(int i=0;i<8;i++){
                tempBytes[56+i]=(byte)(len&0xFFL);
                len=len>>8;
            }
            groups=Create_NewGroup(tempBytes,0);
            Transform(groups);
        }else{
            for(int i=0;i<rest;i++)
                tempBytes[i]=inputBytes[str_length-rest+i];
            tempBytes[rest]=(byte)(1<<7);
            for(int i=rest+1;i<64;i++)
                tempBytes[i]=0;
            groups=Create_NewGroup(tempBytes,0);
            Transform(groups);

            for(int i=0;i<56;i++)
                tempBytes[i]=0;
            long len=(long)(str_length<<3);
            for(int i=0;i<8;i++){
                tempBytes[56+i]=(byte)(len&0xFFL);
                len=len>>8;
            }
            groups=Create_NewGroup(tempBytes,0);
            Transform(groups);
        }
        return ToHexString();
    }

    private String ToHexString(){  //Hash值转换成十六进制的字符串
        String resStr="";
        long temp=0;
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                temp=result[i]&0x0FL;
                String a=hexs[(int)(temp)];
                result[i]=result[i]>>4;
                temp=result[i]&0x0FL;
                resStr+=hexs[(int)(temp)]+a;
                result[i]=result[i]>>4;
            }
        }
        return resStr;
    }
    private void Transform(long[] groups) {  //对分组进行处理，每个分组64字节
        long a = result[0], b = result[1], c = result[2], d = result[3];
        //第一轮
        a = FF(a, b, c, d, groups[0], S11, 0xd76aa478L);
        d = FF(d, a, b, c, groups[1], S12, 0xe8c7b756L);
        c = FF(c, d, a, b, groups[2], S13, 0x242070dbL);
        b = FF(b, c, d, a, groups[3], S14, 0xc1bdceeeL);
        a = FF(a, b, c, d, groups[4], S11, 0xf57c0fafL);
        d = FF(d, a, b, c, groups[5], S12, 0x4787c62aL);
        c = FF(c, d, a, b, groups[6], S13, 0xa8304613L);
        b = FF(b, c, d, a, groups[7], S14, 0xfd469501L);
        a = FF(a, b, c, d, groups[8], S11, 0x698098d8L);
        d = FF(d, a, b, c, groups[9], S12, 0x8b44f7afL);
        c = FF(c, d, a, b, groups[10], S13, 0xffff5bb1L);
        b = FF(b, c, d, a, groups[11], S14, 0x895cd7beL);
        a = FF(a, b, c, d, groups[12], S11, 0x6b901122L);
        d = FF(d, a, b, c, groups[13], S12, 0xfd987193L);
        c = FF(c, d, a, b, groups[14], S13, 0xa679438eL);
        b = FF(b, c, d, a, groups[15], S14, 0x49b40821L);
        //第二轮
        a = GG(a, b, c, d, groups[1], S21, 0xf61e2562L);
        d = GG(d, a, b, c, groups[6], S22, 0xc040b340L);
        c = GG(c, d, a, b, groups[11], S23, 0x265e5a51L);
        b = GG(b, c, d, a, groups[0], S24, 0xe9b6c7aaL);
        a = GG(a, b, c, d, groups[5], S21, 0xd62f105dL);
        d = GG(d, a, b, c, groups[10], S22, 0x2441453L);
        c = GG(c, d, a, b, groups[15], S23, 0xd8a1e681L);
        b = GG(b, c, d, a, groups[4], S24, 0xe7d3fbc8L);
        a = GG(a, b, c, d, groups[9], S21, 0x21e1cde6L);
        d = GG(d, a, b, c, groups[14], S22, 0xc33707d6L);
        c = GG(c, d, a, b, groups[3], S23, 0xf4d50d87L);
        b = GG(b, c, d, a, groups[8], S24, 0x455a14edL);
        a = GG(a, b, c, d, groups[13], S21, 0xa9e3e905L);
        d = GG(d, a, b, c, groups[2], S22, 0xfcefa3f8L);
        c = GG(c, d, a, b, groups[7], S23, 0x676f02d9L);
        b = GG(b, c, d, a, groups[12], S24, 0x8d2a4c8aL);
        //第三轮
        a = HH(a, b, c, d, groups[5], S31, 0xfffa3942L);
        d = HH(d, a, b, c, groups[8], S32, 0x8771f681L);
        c = HH(c, d, a, b, groups[11], S33, 0x6d9d6122L);
        b = HH(b, c, d, a, groups[14], S34, 0xfde5380cL);
        a = HH(a, b, c, d, groups[1], S31, 0xa4beea44L);
        d = HH(d, a, b, c, groups[4], S32, 0x4bdecfa9L);
        c = HH(c, d, a, b, groups[7], S33, 0xf6bb4b60L);
        b = HH(b, c, d, a, groups[10], S34, 0xbebfbc70L);
        a = HH(a, b, c, d, groups[13], S31, 0x289b7ec6L);
        d = HH(d, a, b, c, groups[0], S32, 0xeaa127faL);
        c = HH(c, d, a, b, groups[3], S33, 0xd4ef3085L);
        b = HH(b, c, d, a, groups[6], S34, 0x4881d05L);
        a = HH(a, b, c, d, groups[9], S31, 0xd9d4d039L);
        d = HH(d, a, b, c, groups[12], S32, 0xe6db99e5L);
        c = HH(c, d, a, b, groups[15], S33, 0x1fa27cf8L);
        b = HH(b, c, d, a, groups[2], S34, 0xc4ac5665L);
        //第四轮
        a = II(a, b, c, d, groups[0], S41, 0xf4292244L);
        d = II(d, a, b, c, groups[7], S42, 0x432aff97L);
        c = II(c, d, a, b, groups[14], S43, 0xab9423a7L);
        b = II(b, c, d, a, groups[5], S44, 0xfc93a039L);
        a = II(a, b, c, d, groups[12], S41, 0x655b59c3L);
        d = II(d, a, b, c, groups[3], S42, 0x8f0ccc92L);
        c = II(c, d, a, b, groups[10], S43, 0xffeff47dL);
        b = II(b, c, d, a, groups[1], S44, 0x85845dd1L);
        a = II(a, b, c, d, groups[8], S41, 0x6fa87e4fL);
        d = II(d, a, b, c, groups[15], S42, 0xfe2ce6e0L);
        c = II(c, d, a, b, groups[6], S43, 0xa3014314L);
        b = II(b, c, d, a, groups[13], S44, 0x4e0811a1L);
        a = II(a, b, c, d, groups[4], S41, 0xf7537e82L);
        d = II(d, a, b, c, groups[11], S42, 0xbd3af235L);
        c = II(c, d, a, b, groups[2], S43, 0x2ad7d2bbL);
        b = II(b, c, d, a, groups[9], S44, 0xeb86d391L);

        result[0] += a; //加到先前计算结果中
        result[1] += b;
        result[2] += c;
        result[3] += d;
        result[0]=result[0]&0xFFFFFFFFL;
        result[1]=result[1]&0xFFFFFFFFL;
        result[2]=result[2]&0xFFFFFFFFL;
        result[3]=result[3]&0xFFFFFFFFL;
    }
    private long DealHi(byte b){                   //需要对符号位进行处理
        return b < 0 ? b & 0x7F + 128 : b;
    }
    private long[] Create_NewGroup(byte[] inputBytes,int index){
        long [] temp=new long[16];                 //将每一个512位的分组再细分成16个小组，每个小组64位（8个字节）
        for(int i=0;i<16;i++){
            temp[i]=DealHi(inputBytes[4*i+index])|
                    (DealHi(inputBytes[4*i+1+index]))<<8|
                    (DealHi(inputBytes[4*i+2+index]))<<16|
                    (DealHi(inputBytes[4*i+3+index]))<<24;
        }
        return temp;
    }
    private long F(long x, long y, long z) {
        return (x & y) | ((~x) & z);
    }
    private long G(long x, long y, long z) {
        return (x & z) | (y & (~z));
    }
    private static long H(long x, long y, long z) {
        return x ^ y ^ z;
    }
    private long I(long x, long y, long z) {
        return y ^ (x | (~z));
    }
    private long FF(long a, long b, long c, long d, long x, long s,
                           long ac) {
        a += (F(b, c, d)&0xFFFFFFFFL) + x + ac;
        a = ((a&0xFFFFFFFFL)<< s) | ((a&0xFFFFFFFFL) >>> (32 - s));
        a += b;
        return (a&0xFFFFFFFFL);
    }
    private long GG(long a, long b, long c, long d, long x, long s,
                           long ac) {
        a += (G(b, c, d)&0xFFFFFFFFL) + x + ac;
        a = ((a&0xFFFFFFFFL) << s) | ((a&0xFFFFFFFFL) >>> (32 - s));
        a += b;
        return (a&0xFFFFFFFFL);
    }
    private long HH(long a, long b, long c, long d, long x, long s,
                           long ac) {
        a += (H(b, c, d)&0xFFFFFFFFL) + x + ac;
        a = ((a&0xFFFFFFFFL) << s) | ((a&0xFFFFFFFFL) >>> (32 - s));
        a += b;
        return (a&0xFFFFFFFFL);
    }
    private long II(long a, long b, long c, long d, long x, long s,
                           long ac) {
        a += (I(b, c, d)&0xFFFFFFFFL) + x + ac;
        a = ((a&0xFFFFFFFFL) << s) | ((a&0xFFFFFFFFL) >>> (32 - s));
        a += b;
        return (a&0xFFFFFFFFL);
    }
}
public class IOT_md5 {
    public static void main(String []args){
        String str="Embrace the glorious mess that you are";
        MD5 md=new MD5(str);
    }
}
