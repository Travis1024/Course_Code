package First_Experiment;

import edu.princeton.cs.algs4.StdRandom;
import edu.princeton.cs.algs4.StdStats;

public class PercolationStats {
    private int sidelength;
    private double mean,stddev,confidenceLo,confidenceHi;
    private double[] experiments;

    public PercolationStats(int n, int T_Iteration,int choice) {
        if(n <= 0 || T_Iteration <= 0)
            throw new IllegalArgumentException("Illegal Argument Exception");
        sidelength = n;
        if(sidelength == 1){
            mean = 1;            //渗透阈值的平均值
            stddev = Double.NaN; //渗透阈值的样本标准差
            confidenceLo = Double.NaN;  //95%置信区间下限
            confidenceHi = Double.NaN;  //95%置信区间上限
        }
        else{
            experiments = new double[T_Iteration];  //记录每一次迭代的渗透阈值
            for(int i=0; i<T_Iteration; i++){       //进行T_Iteration次迭代
                Percolation percolation = new Percolation(n,choice);
                double count = 0;
                while(!percolation.Percolates()) {     //判断首尾是否已经连通
                    int row = StdRandom.uniform(n) + 1;//随机选择节点进行open
                    int col = StdRandom.uniform(n) + 1;
                    if(!percolation.IsOpen(row,col)){
                        percolation.Open(row,col);
                        count++;                    //记录open节点数量
                    }
                }
                experiments[i] = count*1.0/(n*n);   //记录此次迭代空置概率p
            }
            mean = StdStats.mean(experiments);      //渗透阈值的样本均值
            stddev = StdStats.stddev(experiments);  //渗透阈值的样本标准差
            confidenceLo = mean - (1.96 * stddev) / Math.sqrt(T_Iteration); //95％置信区间下限
            confidenceHi = mean + (1.96 * stddev) / Math.sqrt(T_Iteration); //95％置信区间上限
        }
    }

    public double Mean() {
        return mean;
    }
    public double Stddev() {
        return stddev;
    }
    public double ConfidenceLo() {
        return confidenceLo;
    }
    public double ConfidenceHi() {
        return confidenceHi;
    }

    public static void main(String[] args) {
        long start_time=System.nanoTime();      //记录开始时间
        int n=320,T_Iteration=200;               //n为网格数，T_Iteration为迭代次数
        int choice=2;                           //选择quickfind or weighted quickunion
        System.out.println("--------------------------------------------------------");
        if(choice==1) {
            System.out.println("The algorithm currently used is：Quick_Find");
        }
        else if(choice==2) {
            System.out.println("The algorithm currently used is：Weighted Quick Union");
        }
        PercolationStats percolationStats = new PercolationStats(n,T_Iteration,choice);
        long consumingtime=System.nanoTime()-start_time;  //计算总耗费时间
        System.out.println("Creating PercolationStats( "+n+" , "+T_Iteration+" )");
        System.out.println("mean:\t\t\t\t= " + percolationStats.Mean());
        System.out.println("stddev:\t\t\t\t= " + percolationStats.Stddev());
        System.out.println("confidence_Low:\t\t= " + percolationStats.ConfidenceLo());
        System.out.println("confidence_High:\t= " + percolationStats.ConfidenceHi());
        System.out.println("The cost of the time is "+consumingtime*1.0/1000000+"ms");
    }
}
