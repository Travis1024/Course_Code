package Second_Experiment;

public class Insertion_Sort {
    public Insertion_Sort(int[] a) {
        int N = a.length;
        for(int i = 1;i < N;i++) {
            for(int j =i;j>0 &&less(a[j],a[j-1]) ; j--) {
                exch(a, j, j-1);
            }
        }
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
