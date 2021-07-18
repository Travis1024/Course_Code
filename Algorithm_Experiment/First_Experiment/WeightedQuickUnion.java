package First_Experiment;

public class WeightedQuickUnion {
    private int[] id;      //父链接数组
    private int[] size;    //各个根节点所对应的分量大小
    private int count;     //连通分量数量
    public WeightedQuickUnion(int N){
        count=N;
        id=new int[N];
        size=new int[N];
        for (int i=0;i<N;i++){
            id[i]=i;
            size[i]=1;
        }
    }
    public int Find(int p){    //找到根结点
        while (p!=id[p]){
            p=id[p];
        }
        return p;
    }
    public boolean Connected(int p,int q){
        return Find(p)==Find(q);
    }
    public void Union(int p,int q){
        int i=Find(p),j=Find(q);
        if(i==j) return;
        if (size[i]<size[j]){   //将小树的根节点连接到大树的根节点
            id[i]=j;
            size[j]+=size[i];
        }else {
            id[j]=i;
            size[i]+=size[j];
        }
        count--;
    }
}
