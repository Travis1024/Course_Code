package First_Experiment;

public class Percolation {
    private int N,count,choice;
    private boolean[] sites;
    private WeightedQuickUnion WQU;
    private QuickFind QF;
    public Percolation(int n,int choice) {
        this.N = n;
        this.choice=choice;
        this.sites = new boolean[N*N+2];
        sites[0] = true;      //初始化所有格点为blocked
        sites[N*N+1] = true;  //开头和末尾为open
        count = 0;
        WQU = new WeightedQuickUnion(N*N+2);
        QF=new QuickFind(N*N+2);
    }
    //将节点变为open节点
    public void Open(int row, int col) {
        if(row < 1 || row > N || col < 1 || col > N)
            throw new IndexOutOfBoundsException("Index Out Of Bounds Exception");
        int a=0;
        if(sites[(row-1)*N+col]) return;
        sites[(row-1)*N+col] = true;
        count++;

        if(choice==2){
            //如果在第一行，将其与开头连接
            if(row == 1)
                WQU.Union(0,(row-1)*N+col);
            //如果在最后一行，将其与结尾连接
            if(row == N)
                WQU.Union((row-1)*N+col,N*N+1);

            //连接上下左右的open节点
            if(col != 1 && sites[(row-1)*N+col-1])
                WQU.Union((row-1)*N+col,(row-1)*N+col-1);
            if(col != N && sites[(row-1)*N+col+1])
                WQU.Union((row-1)*N+col,(row-1)*N+col+1);
            if(row != 1 && sites[(row-2)*N+col])
                WQU.Union((row-1)*N+col,(row-2)*N+col);
            if(row != N && sites[row*N+col])
                WQU.Union((row-1)*N+col,row*N+col);
        }else if(choice==1){
            if(row == 1)
                QF.Union(0,(row-1)*N+col);
            //如果在最后一行，将其与结尾连接
            if(row == N)
                QF.Union((row-1)*N+col,N*N+1);

            //连接上下左右的open节点
            if(col != 1 && sites[(row-1)*N+col-1])
                QF.Union((row-1)*N+col,(row-1)*N+col-1);
            if(col != N && sites[(row-1)*N+col+1])
                QF.Union((row-1)*N+col,(row-1)*N+col+1);
            if(row != 1 && sites[(row-2)*N+col])
                QF.Union((row-1)*N+col,(row-2)*N+col);
            if(row != N && sites[row*N+col])
                QF.Union((row-1)*N+col,row*N+col);
        }
    }
    //判断节点是否为open节点
    public boolean IsOpen(int row, int col) {
        if(row < 1 || row > N || col < 1 || col > N)
            throw new IndexOutOfBoundsException("Index Out Of Bounds Exception");
        return sites[(row-1)*N+col];
    }
    //判断首尾是否已经连通
    public boolean Percolates(){
        if (choice==1) return QF.Find(0)==QF.Find(N*N+1);
        return WQU.Find(0)==WQU.Find(N*N+1);
    }
}
