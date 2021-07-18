package Second_Experiment;

public class QuickSort_With_D3P {
    public QuickSort_With_D3P(int[] a) {
        sort(a,0,a.length-1);
    }
    private void sort(int[] a,int lo,int hi) {
        if(hi<=lo) return;
        int lt = lo,i = lo +1,gt = hi;
        int v = a[lo];
        while(i <= gt) {
            int cmp = compare(a[i],v);
            if(cmp < 0) exch(a, lt++, i++);
            else if(cmp > 0) exch(a, i, gt--);
            else i++;
        }
        sort(a,lo,lt-1);
        sort(a,gt+1,hi);
    }
    private int compare(int i,int j) {
        if(i < j) return -1;
        else return 1;
    }
    private void exch(int[] a,int i,int j) {
        int t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
}
