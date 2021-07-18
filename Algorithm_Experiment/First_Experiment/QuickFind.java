package First_Experiment;

public class QuickFind {
    private int[] id;
    private int count;
    public QuickFind(int N){
        count = N;
        id = new int[N];
        for (int i = 0; i < N; i++)
            id[i] = i;
    }
    public int Find(int p){
        return id[p];
    }
    public boolean Connected(int p, int q) {
        return id[p] == id[q];
    }
    public void Union(int p, int q) {//将p和q归并到相同的分量中
        int pID = id[p],qID = id[q];
        if (pID == qID) return;
        for (int i = 0; i < id.length; i++)
            if (id[i] == pID) id[i] = qID;
        count--;
    }
}
