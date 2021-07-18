package Second_Experiment;

public class Random_QuickSort {
    public Random_QuickSort(int[] a) {
        sort(a,0,a.length-1);
    }
    private void sort(int[] a,int lo,int hi) {
        ///if(hi <= lo) return;
        if(lo<hi){
            int j = partition(a,lo,hi);
            sort(a,lo,j -1);
            sort(a,j+1,hi);
        }
    }
    private int partition(int[] a, int lo, int hi) {
        int i = lo,j = hi+1;
        int v = a[lo];
        while(true) {
            while(less(a[++i],v)) if(i == hi) break;
            while(less(v,a[--j])) if(j == lo) break;
            if(i >=j) break;
            exch(a,i,j);
        }
        exch(a, lo, j);
        return j;
    }
    private boolean less(int a,int b) {
        if(a < b) return true;
        else return false;
    }
    private void exch(int[] a,int i,int j) {
        int t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
}
